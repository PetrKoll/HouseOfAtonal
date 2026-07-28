"""Add individually editable traffic and pedestrian placeholders."""

import unreal


MAP = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"


def native(name):
    result = unreal.load_class(None, f"/Script/HouseOfAtonal.{name}")
    if not result:
        raise RuntimeError(f"Native class missing: {name}")
    return result


def make_route(label, points, route_folder, closed=True):
    route = unreal.EditorLevelLibrary.spawn_actor_from_class(
        route_class, unreal.Vector(0, 0, 0)
    )
    route.set_actor_label(label)
    route.set_folder_path(route_folder)
    route.set_editor_property("closed_loop", closed)
    spline = route.get_component_by_class(unreal.SplineComponent)
    spline.clear_spline_points(False)
    for x, y, z in points:
        spline.add_spline_point(
            unreal.Vector(x, y, z), unreal.SplineCoordinateSpace.WORLD, False
        )
    spline.set_closed_loop(closed, True)
    return route


def spawn_mover(actor_class, label, route, distance, folder, speed, reverse=False):
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        actor_class, unreal.Vector(0, 0, 2)
    )
    actor.set_actor_label(label)
    actor.set_folder_path(folder)
    actor.set_editor_property("route", route)
    actor.set_editor_property("start_distance", distance)
    actor.set_editor_property("speed", speed)
    actor.set_editor_property("reverse_direction", reverse)
    actor.snap_to_route()
    return actor


unreal.EditorLevelLibrary.load_level(MAP)

route_class = native("HouseRouteSpline")
vehicle_class = native("HouseTrafficVehicle")
pedestrian_class = native("HousePedestrian")
parked_class = native("HouseParkedVehicle")

# Remove only the previously generated traffic foundation. User-owned city,
# camera, lighting and architecture actors remain untouched.
for actor in unreal.EditorLevelLibrary.get_all_level_actors():
    actor_folder = str(actor.get_folder_path())
    if actor_folder.startswith("MenuLevel/Traffic") or actor_folder.startswith(
        "MenuLevel/Pedestrians"
    ):
        unreal.EditorLevelLibrary.destroy_actor(actor)

car_routes = [
    make_route(
        "CarRoute_CentralLoop",
        [
            (-119, -105, 2.0), (-38, -101, 2.0), (47, -112, 2.0),
            (132, -96, 2.0), (124, -32, 2.0), (125, 66, 2.0),
            (48, 48, 2.0), (-38, 62, 2.0), (-128, 43, 2.0),
            (-119, -12, 2.0),
        ],
        "MenuLevel/Traffic/Routes",
    ),
    make_route(
        "CarRoute_OuterLoop",
        [
            (-190, -176, 2.0), (-42, -190, 2.0), (126, -192, 2.0),
            (192, -112, 2.0), (191, 75, 2.0), (137, 149, 2.0),
            (-31, 146, 2.0), (-190, 140, 2.0), (-198, -30, 2.0),
        ],
        "MenuLevel/Traffic/Routes",
    ),
    make_route(
        "CarRoute_CrossTown",
        [
            (-205, 92, 2.0), (-135, 25, 2.0), (-62, -40, 2.0),
            (4, -105, 2.0), (45, -178, 2.0), (119, -126, 2.0),
            (137, -38, 2.0), (124, 50, 2.0), (55, 132, 2.0),
            (-31, 146, 2.0), (-128, 43, 2.0),
        ],
        "MenuLevel/Traffic/Routes",
    ),
]

vehicle_index = 0
for route_index, route in enumerate(car_routes):
    length = route.get_route_length()
    count = (3, 3, 2)[route_index]
    for index in range(count):
        spawn_mover(
            vehicle_class,
            f"MovingCar_{vehicle_index:02d}",
            route,
            length * index / count,
            "MenuLevel/Traffic/MovingCars",
            8.5 + route_index * 1.1,
            reverse=False,
        )
        vehicle_index += 1

pedestrian_routes = [
    make_route(
        "PedestrianRoute_HeroBlock",
        [
            (-31, -18, 2.0), (42, -18, 2.0), (48, 48, 2.0),
            (-38, 62, 2.0), (-40, 34, 2.0),
        ],
        "MenuLevel/Pedestrians/Routes",
    ),
    make_route(
        "PedestrianRoute_WestBlocks",
        [
            (-118, -45, 2.0), (-103, 45, 2.0), (-112, 120, 2.0),
            (-31, 146, 2.0), (-38, 62, 2.0), (-119, 43, 2.0),
        ],
        "MenuLevel/Pedestrians/Routes",
    ),
]

pedestrian_index = 0
for route in pedestrian_routes:
    length = route.get_route_length()
    for index in range(5):
        spawn_mover(
            pedestrian_class,
            f"Pedestrian_{pedestrian_index:02d}",
            route,
            length * index / 5.0,
            "MenuLevel/Pedestrians/Actors",
            2.0 + (index % 3) * 0.25,
            reverse=index % 2 == 1,
        )
        pedestrian_index += 1

parked_positions = [
    (-84, -108, 2.0, -5), (-64, -105, 2.0, -5),
    (67, -108, 2.0, 7), (93, -105, 2.0, 7),
    (-115, 72, 2.0, 82), (-112, 95, 2.0, 82),
    (128, 18, 2.0, 82), (126, 42, 2.0, 82),
    (-24, 138, 2.0, -5), (4, 135, 2.0, -5),
    (68, -184, 2.0, -7), (96, -188, 2.0, -7),
]
for index, (x, y, z, yaw) in enumerate(parked_positions):
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        parked_class,
        unreal.Vector(x, y, z),
        unreal.Rotator(roll=0.0, pitch=0.0, yaw=yaw),
    )
    actor.set_actor_label(f"ParkedCar_{index:02d}")
    actor.set_folder_path("MenuLevel/Traffic/ParkedCars")

unreal.EditorLevelLibrary.save_current_level()
unreal.log("[FORGE] Traffic placeholders added to editable Menu Level")
