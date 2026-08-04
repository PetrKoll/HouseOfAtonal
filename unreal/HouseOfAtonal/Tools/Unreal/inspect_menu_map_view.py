import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"
level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

if not level_subsystem.load_level(MAP_PATH):
    raise RuntimeError(f"Could not load {MAP_PATH}")

actors = actor_subsystem.get_all_level_actors()
unreal.log(f"MAP_VIEW_ACTOR_COUNT={len(actors)}")

for actor in actors:
    label = actor.get_actor_label()
    folder = str(actor.get_folder_path())
    if any(token in label.lower() for token in ("view", "hero", "house", "player")):
        unreal.log(
            f"MAP_VIEW_KEY_ACTOR={label}|{actor.get_class().get_name()}|"
            f"{actor.get_actor_location()}|{actor.get_actor_rotation()}|{folder}"
        )

locations = [actor.get_actor_location() for actor in actors if not actor.is_hidden_ed()]
if locations:
    min_v = unreal.Vector(
        min(v.x for v in locations), min(v.y for v in locations), min(v.z for v in locations)
    )
    max_v = unreal.Vector(
        max(v.x for v in locations), max(v.y for v in locations), max(v.z for v in locations)
    )
    unreal.log(f"MAP_VIEW_ACTOR_BOUNDS={min_v}|{max_v}")
