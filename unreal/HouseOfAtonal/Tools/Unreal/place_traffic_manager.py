import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"
LABEL = "Traffic_Manager"

unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
manager_class = unreal.load_class(None, "/Script/HouseOfAtonal.HouseTrafficManager")
if not manager_class:
    raise RuntimeError("HouseTrafficManager class was not found")

managers = [
    actor
    for actor in subsystem.get_all_level_actors()
    if actor.get_class() == manager_class
]
manager = managers[0] if managers else subsystem.spawn_actor_from_class(
    manager_class,
    unreal.Vector(0.0, 0.0, 0.0),
    unreal.Rotator(roll=0.0, pitch=0.0, yaw=0.0),
)
manager.set_actor_label(LABEL)
manager.set_folder_path("MenuLevel/Traffic")

if not unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).save_current_level():
    raise RuntimeError(f"Failed to save {MAP_PATH}")

unreal.log_warning(f"TRAFFIC_MANAGER_DONE actor={manager.get_actor_label()}")
