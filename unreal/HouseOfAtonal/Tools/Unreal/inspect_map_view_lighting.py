import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"
level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

if not level_subsystem.load_level(MAP_PATH):
    raise RuntimeError(f"Could not load {MAP_PATH}")

for actor in actor_subsystem.get_all_level_actors():
    label = actor.get_actor_label()
    folder = str(actor.get_folder_path())
    if "tree" in folder.lower() or label.lower().startswith(("01_", "02_", "03_")):
        intensity = ""
        component = actor.get_component_by_class(unreal.LightComponent)
        if component:
            intensity = str(component.get_editor_property("intensity"))
        unreal.log(
            f"MAP_LIGHT_INSPECT={label}|{actor.get_class().get_name()}|{folder}|{intensity}"
        )
