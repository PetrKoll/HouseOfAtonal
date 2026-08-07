import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"
TAG_NEIGHBORHOOD = unreal.Name("House.MapView.Neighborhood")
TAG_MAP = unreal.Name("House.MapView.Map")

NEIGHBORHOOD_FOLDER_NAMES = {
    "tree", "trees", "lampy", "house lights", "house_lights", "fog_neighbourhood"
}
MAP_FOLDER_NAMES = {"fog_map", "map_geometry"}


level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
if not level_subsystem.load_level(MAP_PATH):
    raise RuntimeError(f"Could not load {MAP_PATH}")


def add_tag(actor, tag):
    tags = list(actor.get_editor_property("tags"))
    if tag not in tags:
        tags.append(tag)
        actor.set_editor_property("tags", tags)


counts = {"neighborhood": 0, "map": 0}
matched_folders = set()
lighting_folders = set()
for actor in actor_subsystem.get_all_level_actors():
    folder = str(actor.get_folder_path())
    label_lower = actor.get_actor_label().strip().lower()
    folder_parts = {part.strip().lower() for part in folder.replace("\\", "/").split("/")}
    if "light" in folder.lower() or "house" in folder.lower():
        lighting_folders.add(folder)
    if folder_parts.intersection(NEIGHBORHOOD_FOLDER_NAMES):
        add_tag(actor, TAG_NEIGHBORHOOD)
        counts["neighborhood"] += 1
        matched_folders.add(folder)
    if folder_parts.intersection(MAP_FOLDER_NAMES) or label_lower == "map_geometry":
        add_tag(actor, TAG_MAP)
        counts["map"] += 1
        matched_folders.add(folder or f"Actor:{actor.get_actor_label()}")

if not level_subsystem.save_current_level():
    raise RuntimeError(f"Could not save {MAP_PATH}")
unreal.log(f"MAP_VIEW_LAYER_FOLDERS={sorted(matched_folders)}")
unreal.log(f"MAP_VIEW_LAYER_COUNTS={counts}")
unreal.log(f"MAP_VIEW_LIGHTING_FOLDER_CANDIDATES={sorted(lighting_folders)}")
unreal.log("MAP_VIEW_LAYER_UPDATE_COMPLETE")
