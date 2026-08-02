import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"
ACTOR_LABEL = "Camera_DOF_Settings"

unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()

matches = [actor for actor in actors if actor.get_actor_label() == ACTOR_LABEL]
if matches:
    dof_actor = matches[0]
else:
    dof_class = unreal.load_class(None, "/Script/HouseOfAtonal.HouseCameraDOFSettings")
    if not dof_class:
        raise RuntimeError("HouseCameraDOFSettings class was not found")
    dof_actor = actor_subsystem.spawn_actor_from_class(
        dof_class,
        unreal.Vector(0.0, 0.0, 0.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    dof_actor.set_actor_label(ACTOR_LABEL)

if not unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).save_current_level():
    raise RuntimeError(f"Failed to save {MAP_PATH}")

unreal.log_warning(f"CAMERA_DOF_DONE actor={dof_actor.get_actor_label()}")
