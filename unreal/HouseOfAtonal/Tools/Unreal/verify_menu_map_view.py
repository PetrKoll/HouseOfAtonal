import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"
level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

if not level_subsystem.load_level(MAP_PATH):
    raise RuntimeError(f"Could not load {MAP_PATH}")

actors = actor_subsystem.get_all_level_actors()
required_labels = {
    "MAP_VIEW_CONTROLLER",
    "MAPVIEW_NEIGHBORHOOD_VIEWPOINT_EDIT_ME",
    "MAPVIEW_AERIAL_VIEWPOINT_EDIT_ME",
    "MAPVIEW_TOGGLE_MAP_HOME_EDIT_ME",
    "MAPVIEW_MAP_CONTENT_ATTACH_HERE",
    "MAPVIEW_TRANSITION_CLOUDS_ATTACH_HERE",
    "MAPVIEW_MAP_BASE_EDIT_ME",
    "MAPVIEW_HOUSE_MARKER_EDIT_ME",
    "MAPVIEW_GLOBAL_DIRECTIONAL_LIGHT_EDIT_ME",
    "LABEL_LOCAL_SCHOOL_EDIT_ME",
    "LABEL_LOCAL_CITY_CENTER_EDIT_ME",
    "LABEL_MAP_SCHOOL_EDIT_ME",
    "LABEL_MAP_CITY_CENTER_EDIT_ME",
}

labels = [actor.get_actor_label() for actor in actors]
missing = sorted(required_labels.difference(labels))
duplicates = sorted(label for label in required_labels if labels.count(label) != 1)

tag_counts = {}
for tag in (
    "House.MapView.Neighborhood",
    "House.MapView.Map",
    "House.MapView.TransitionCloud",
    "House.MapView.NeighborhoodLabel",
    "House.MapView.MapLabel",
    "House.MapView.NeighborhoodLight",
    "House.MapView.MapLight",
):
    tag_name = unreal.Name(tag)
    tag_counts[tag] = sum(
        1 for actor in actors if tag_name in list(actor.get_editor_property("tags"))
    )

unreal.log(f"MAP_VIEW_VERIFY_MISSING={missing}")
unreal.log(f"MAP_VIEW_VERIFY_NON_UNIQUE={duplicates}")
unreal.log(f"MAP_VIEW_VERIFY_TAG_COUNTS={tag_counts}")
if missing or duplicates:
    raise RuntimeError("Map View setup verification failed")
unreal.log("MAP_VIEW_VERIFY_OK")
