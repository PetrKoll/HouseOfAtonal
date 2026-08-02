import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"

unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
manager_class = unreal.load_class(None, "/Script/HouseOfAtonal.HouseTrafficManager")
vehicle_class = unreal.load_class(None, "/Script/HouseOfAtonal.HouseTrafficVehicle")
if not manager_class or not vehicle_class:
    raise RuntimeError("Traffic classes were not found")

actors = subsystem.get_all_level_actors()
managers = [actor for actor in actors if actor.get_class() == manager_class]
if len(managers) != 1:
    raise RuntimeError(f"Expected one Traffic Manager, found {len(managers)}")
manager = managers[0]

authored_cars = sorted(
    [actor for actor in actors if actor.get_class() == vehicle_class],
    key=lambda actor: actor.get_actor_label(),
)

settings_array = []
for actor in authored_cars:
    route = actor.get_editor_property("route")
    if not route:
        unreal.log_warning(f"TRAFFIC_MIGRATE skipped {actor.get_actor_label()}: no route")
        continue

    start_distance = actor.get_editor_property("start_distance")
    reverse = actor.get_editor_property("reverse_direction")
    spline = route.get_component_by_class(unreal.SplineComponent)
    route_rotation = spline.get_rotation_at_distance_along_spline(
        start_distance,
        unreal.SplineCoordinateSpace.WORLD,
    )
    if reverse:
        route_rotation.yaw += 180.0

    actor_rotation = actor.get_actor_rotation()
    mesh_rotation = actor.get_editor_property("vehicle_mesh_rotation")
    rotation_offset = unreal.Rotator(
        roll=actor_rotation.roll - route_rotation.roll + mesh_rotation.roll,
        pitch=actor_rotation.pitch - route_rotation.pitch + mesh_rotation.pitch,
        yaw=actor_rotation.yaw - route_rotation.yaw + mesh_rotation.yaw,
    )

    settings = unreal.HouseTrafficVehicleSettings()
    settings.set_editor_property("enabled", True)
    settings.set_editor_property("route", route)
    settings.set_editor_property(
        "vehicle_variant", actor.get_editor_property("vehicle_variant")
    )
    settings.set_editor_property("start_distance", start_distance)
    settings.set_editor_property("speed", actor.get_editor_property("speed"))
    settings.set_editor_property("reverse_direction", reverse)
    settings.set_editor_property("scale", actor.get_actor_scale3d())
    settings.set_editor_property("rotation_offset", rotation_offset)
    settings.set_editor_property(
        "minimum_spacing", actor.get_editor_property("minimum_spacing")
    )
    settings.set_editor_property(
        "intersection_safety_radius",
        actor.get_editor_property("intersection_safety_radius"),
    )
    settings_array.append(settings)

manager.set_editor_property("vehicles", settings_array)

if not settings_array:
    route_specs = [
        ("CarRoute_CentralLoop", 3, 8.5),
        ("CarRoute_OuterLoop", 3, 9.6),
        ("CarRoute_CrossTown", 2, 10.7),
    ]
    routes_by_label = {
        actor.get_actor_label(): actor
        for actor in actors
        if actor.get_actor_label().startswith("CarRoute_")
    }
    vehicle_index = 0
    for route_label, count, speed in route_specs:
        route = routes_by_label.get(route_label)
        if not route:
            raise RuntimeError(f"Missing route: {route_label}")
        spline = route.get_component_by_class(unreal.SplineComponent)
        route_length = spline.get_spline_length()
        for index in range(count):
            settings = unreal.HouseTrafficVehicleSettings()
            settings.set_editor_property("enabled", True)
            settings.set_editor_property("route", route)
            settings.set_editor_property("vehicle_variant", vehicle_index % 3)
            settings.set_editor_property(
                "start_distance", route_length * index / count
            )
            settings.set_editor_property("speed", speed)
            settings.set_editor_property("reverse_direction", False)
            settings.set_editor_property(
                "scale", unreal.Vector(0.01, 0.01, 0.01)
            )
            settings.set_editor_property(
                "rotation_offset",
                unreal.Rotator(roll=0.0, pitch=0.0, yaw=-90.0),
            )
            settings.set_editor_property("minimum_spacing", 7.0)
            settings.set_editor_property("intersection_safety_radius", 5.0)
            settings_array.append(settings)
            vehicle_index += 1
    manager.set_editor_property("vehicles", settings_array)

for actor in authored_cars:
    subsystem.destroy_actor(actor)

if not unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).save_current_level():
    raise RuntimeError(f"Failed to save {MAP_PATH}")

unreal.log_warning(
    f"TRAFFIC_MIGRATE_DONE vehicles={len(settings_array)} removed={len(authored_cars)}"
)
