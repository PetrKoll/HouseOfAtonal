import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"

unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

moving_class = unreal.load_class(None, "/Script/HouseOfAtonal.HouseTrafficVehicle")
parked_class = unreal.load_class(None, "/Script/HouseOfAtonal.HouseParkedVehicle")
if not moving_class or not parked_class:
    raise RuntimeError("Vehicle classes were not found")

changed = 0
for actor in actor_subsystem.get_all_level_actors():
    if actor.get_class() in (moving_class, parked_class):
        label = actor.get_actor_label()
        digits = "".join(character for character in label if character.isdigit())
        index = int(digits or "0")
        actor.set_editor_property("vehicle_variant", index % 3)
        actor.set_editor_property(
            "vehicle_mesh_rotation",
            unreal.Rotator(roll=0.0, pitch=0.0, yaw=-90.0),
        )
        visual = actor.get_component_by_class(unreal.StaticMeshComponent)
        if visual:
            visual.set_relative_rotation(
                unreal.Rotator(roll=0.0, pitch=0.0, yaw=-90.0),
                False,
                False,
            )
            visual.set_cast_shadow(True)
            mesh = visual.get_editor_property("static_mesh")
            mesh_path = mesh.get_path_name() if mesh else "None"
            unreal.log_warning(
                f"CAR_VARIANT actor={label} variant={index % 3} mesh={mesh_path} shadow=True"
            )
        changed += 1

if not unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).save_current_level():
    raise RuntimeError(f"Failed to save {MAP_PATH}")

unreal.log_warning(f"CAR_VARIANTS_DONE changed={changed}")
