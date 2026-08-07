import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"
BP_DIR = "/Game/HouseOfAtonal/Blueprints/MapView"
TAG_NEIGHBORHOOD = "House.MapView.Neighborhood"
TAG_MAP = "House.MapView.Map"
TAG_CLOUD = "House.MapView.TransitionCloud"
TAG_LOCAL_LABEL = "House.MapView.NeighborhoodLabel"
TAG_MAP_LABEL = "House.MapView.MapLabel"


level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
asset_tools = unreal.AssetToolsHelpers.get_asset_tools()

if not level_subsystem.load_level(MAP_PATH):
    raise RuntimeError(f"Could not load {MAP_PATH}")


def create_blueprint(name, native_class_path):
    asset_path = f"{BP_DIR}/{name}"
    blueprint = unreal.load_asset(asset_path)
    if blueprint is not None:
        return blueprint
    parent_class = unreal.load_class(None, native_class_path)
    if parent_class is None:
        raise RuntimeError(f"Could not load {native_class_path}")
    factory = unreal.BlueprintFactory()
    factory.set_editor_property("parent_class", parent_class)
    blueprint = asset_tools.create_asset(name, BP_DIR, unreal.Blueprint, factory)
    if blueprint is None:
        raise RuntimeError(f"Could not create {asset_path}")
    return blueprint


controller_bp = create_blueprint(
    "BP_HouseMapViewController", "/Script/HouseOfAtonal.HouseMapViewController"
)
viewpoint_bp = create_blueprint(
    "BP_HouseMapViewpoint", "/Script/HouseOfAtonal.HouseMapViewpoint"
)
label_bp = create_blueprint(
    "BP_HouseLocationLabel", "/Script/HouseOfAtonal.HouseLocationLabel"
)
button_bp = create_blueprint(
    "BP_HouseMapToggleButton", "/Script/HouseOfAtonal.HouseMapToggleButton"
)


def all_actors():
    return actor_subsystem.get_all_level_actors()


def find(label):
    return next((actor for actor in all_actors() if actor.get_actor_label() == label), None)


def spawn_or_find(label, actor_class, location, rotation, folder):
    actor = find(label)
    if actor is None:
        actor = actor_subsystem.spawn_actor_from_class(actor_class, location, rotation)
    actor.set_actor_label(label)
    actor.set_folder_path(folder)
    actor.set_actor_location(location, False, False)
    actor.set_actor_rotation(rotation, False)
    return actor


def add_tag(actor, tag):
    tags = list(actor.get_editor_property("tags"))
    tag_name = unreal.Name(tag)
    if tag_name not in tags:
        tags.append(tag_name)
        actor.set_editor_property("tags", tags)


controller_class = controller_bp.generated_class()
viewpoint_class = viewpoint_bp.generated_class()
label_class = label_bp.generated_class()
button_class = button_bp.generated_class()

original_view = find("MENU_VIEWPOINT_MOVE_ME")
normal_location = (
    original_view.get_actor_location()
    if original_view
    else unreal.Vector(-283.0, -283.0, 184.0)
)
normal_rotation = (
    original_view.get_actor_rotation()
    if original_view
    else unreal.Rotator(-37.0, 45.0, 0.0)
)

controller = spawn_or_find(
    "MAP_VIEW_CONTROLLER",
    controller_class,
    unreal.Vector(0.0, 0.0, 0.0),
    unreal.Rotator(0.0, 0.0, 0.0),
    "MenuLevel/MapView/00_Controller",
)
normal_viewpoint = spawn_or_find(
    "MAPVIEW_NEIGHBORHOOD_VIEWPOINT_EDIT_ME",
    viewpoint_class,
    normal_location,
    normal_rotation,
    "MenuLevel/MapView/01_Viewpoints",
)
map_viewpoint = spawn_or_find(
    "MAPVIEW_AERIAL_VIEWPOINT_EDIT_ME",
    viewpoint_class,
    unreal.Vector(0.0, 0.0, 1250.0),
    unreal.Rotator(-90.0, 45.0, 0.0),
    "MenuLevel/MapView/01_Viewpoints",
)

controller.set_editor_property("neighborhood_viewpoint", normal_viewpoint)
controller.set_editor_property("map_viewpoint", map_viewpoint)

button = spawn_or_find(
    "MAPVIEW_TOGGLE_MAP_HOME_EDIT_ME",
    button_class,
    normal_location - unreal.Vector(0.0, 0.0, 110.0),
    unreal.Rotator(0.0, 0.0, 0.0),
    "MenuLevel/MapView/02_Toggle",
)
button.set_editor_property("controller", controller)

# Tag the existing editable diorama content without touching its lighting,
# atmosphere, UDS, FogArea or viewpoint actors.
neighborhood_folders = (
    "MenuLevel/Ground",
    "MenuLevel/Roads",
    "MenuLevel/Parcels",
    "MenuLevel/Buildings",
    "MenuLevel/Hero",
    "MenuLevel/Traffic",
    "MenuLevel/Pedestrians",
    "MenuLevel/tree",
)
neighborhood_folder_names = {
    "tree", "trees", "lampy", "house lights", "house_lights", "fog_neighbourhood"
}
for actor in all_actors():
    folder = str(actor.get_folder_path())
    folder_parts = {
        part.strip().lower() for part in folder.replace("\\", "/").split("/")
    }
    if (any(folder.lower().startswith(prefix.lower()) for prefix in neighborhood_folders)
            or folder_parts.intersection(neighborhood_folder_names)):
        add_tag(actor, TAG_NEIGHBORHOOD)

for actor in all_actors():
    folder = str(actor.get_folder_path())
    folder_parts = {
        part.strip().lower() for part in folder.replace("\\", "/").split("/")
    }
    if (folder_parts.intersection({"fog_map", "map_geometry"})
            or actor.get_actor_label().strip().lower() == "map_geometry"):
        add_tag(actor, TAG_MAP)

for actor in all_actors():
    if actor.get_actor_label().lower() in {
        "01_hero_sun_beam",
        "02_outer_core",
        "03_ambient",
    }:
        add_tag(actor, "House.MapView.NeighborhoodLight")

map_light = spawn_or_find(
    "MAPVIEW_GLOBAL_DIRECTIONAL_LIGHT_EDIT_ME",
    unreal.DirectionalLight,
    unreal.Vector(0.0, 0.0, 900.0),
    unreal.Rotator(-55.0, -30.0, 0.0),
    "MenuLevel/MapView/06_Lighting",
)
map_light_component = map_light.get_component_by_class(unreal.DirectionalLightComponent)
map_light_component.set_editor_property("intensity", 2.0)
map_light_component.set_mobility(unreal.ComponentMobility.MOVABLE)
add_tag(map_light, "House.MapView.MapLight")

# Editable anchors make each future art layer visible and selectable in Outliner.
anchor_class = unreal.TargetPoint
map_anchor = spawn_or_find(
    "MAPVIEW_MAP_CONTENT_ATTACH_HERE",
    anchor_class,
    unreal.Vector(0.0, 0.0, 0.0),
    unreal.Rotator(0.0, 0.0, 0.0),
    "MenuLevel/MapView/03_MapContent",
)
add_tag(map_anchor, TAG_MAP)
cloud_anchor = spawn_or_find(
    "MAPVIEW_TRANSITION_CLOUDS_ATTACH_HERE",
    anchor_class,
    unreal.Vector(0.0, 0.0, 650.0),
    unreal.Rotator(0.0, 0.0, 0.0),
    "MenuLevel/MapView/04_TransitionClouds",
)
add_tag(cloud_anchor, TAG_CLOUD)

# Simple map blockout. It is intentionally ordinary geometry and can be moved,
# replaced or deleted independently in the viewport.
cube = unreal.load_asset("/Engine/BasicShapes/Cube.Cube")
cylinder = unreal.load_asset("/Engine/BasicShapes/Cylinder.Cylinder")
map_base = spawn_or_find(
    "MAPVIEW_MAP_BASE_EDIT_ME",
    unreal.StaticMeshActor,
    unreal.Vector(0.0, 0.0, -35.0),
    unreal.Rotator(0.0, 0.0, 0.0),
    "MenuLevel/MapView/03_MapContent",
)
map_base.static_mesh_component.set_static_mesh(cube)
map_base.set_actor_scale3d(unreal.Vector(35.0, 35.0, 0.05))
add_tag(map_base, TAG_MAP)

hero_marker = spawn_or_find(
    "MAPVIEW_HOUSE_MARKER_EDIT_ME",
    unreal.StaticMeshActor,
    unreal.Vector(26.0, 3.0, 5.0),
    unreal.Rotator(0.0, 0.0, 0.0),
    "MenuLevel/MapView/03_MapContent",
)
hero_marker.static_mesh_component.set_static_mesh(cylinder)
hero_marker.set_actor_scale3d(unreal.Vector(0.55, 0.55, 0.12))
add_tag(hero_marker, TAG_MAP)


def configure_label(label, place, distance, location, rotation, tag, folder, size):
    actor = spawn_or_find(label, label_class, location, rotation, folder)
    actor.set_editor_property("place_name", unreal.Text(place))
    actor.set_editor_property("distance", unreal.Text(distance))
    actor.set_editor_property("world_size", size)
    add_tag(actor, tag)
    return actor


configure_label(
    "LABEL_LOCAL_SCHOOL_EDIT_ME",
    "School",
    "340 m",
    unreal.Vector(-420.0, 120.0, 125.0),
    unreal.Rotator(0.0, 45.0, 0.0),
    TAG_LOCAL_LABEL,
    "MenuLevel/MapView/05_Labels/Neighborhood",
    4.0,
)
configure_label(
    "LABEL_LOCAL_CITY_CENTER_EDIT_ME",
    "City Center",
    "2.6 km",
    unreal.Vector(390.0, 330.0, 150.0),
    unreal.Rotator(0.0, -135.0, 0.0),
    TAG_LOCAL_LABEL,
    "MenuLevel/MapView/05_Labels/Neighborhood",
    4.0,
)
configure_label(
    "LABEL_MAP_SCHOOL_EDIT_ME",
    "School",
    "340 m",
    unreal.Vector(-900.0, 250.0, 25.0),
    unreal.Rotator(90.0, 0.0, 0.0),
    TAG_MAP_LABEL,
    "MenuLevel/MapView/05_Labels/Map",
    12.0,
)
configure_label(
    "LABEL_MAP_CITY_CENTER_EDIT_ME",
    "City Center",
    "2.6 km",
    unreal.Vector(1100.0, 850.0, 25.0),
    unreal.Rotator(90.0, 0.0, 0.0),
    TAG_MAP_LABEL,
    "MenuLevel/MapView/05_Labels/Map",
    12.0,
)

# Start in the normal neighborhood editing mode.
controller.apply_editor_preview()

for blueprint in (controller_bp, viewpoint_bp, label_bp, button_bp):
    unreal.EditorAssetLibrary.save_loaded_asset(blueprint)
level_subsystem.save_current_level()

unreal.log(f"MAP_VIEW_CONTROLLER={controller.get_path_name()}")
unreal.log(f"MAP_VIEW_NORMAL={normal_viewpoint.get_path_name()}")
unreal.log(f"MAP_VIEW_AERIAL={map_viewpoint.get_path_name()}")
unreal.log("MAP_VIEW_SETUP_COMPLETE")
