import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"
NEIGHBORHOOD_TAG = unreal.Name("House.MapView.Neighborhood")
LOCAL_LIGHT_TAG = unreal.Name("House.MapView.NeighborhoodLight")
MAP_LIGHT_TAG = unreal.Name("House.MapView.MapLight")

level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

if not level_subsystem.load_level(MAP_PATH):
    raise RuntimeError(f"Could not load {MAP_PATH}")


def add_tag(actor, tag):
    tags = list(actor.get_editor_property("tags"))
    if tag not in tags:
        tags.append(tag)
        actor.set_editor_property("tags", tags)


actors = actor_subsystem.get_all_level_actors()
for actor in actors:
    if str(actor.get_folder_path()).lower().startswith("menulevel/tree"):
        add_tag(actor, NEIGHBORHOOD_TAG)
    if actor.get_actor_label().lower() in {
        "01_hero_sun_beam",
        "02_outer_core",
        "03_ambient",
    }:
        add_tag(actor, LOCAL_LIGHT_TAG)

map_light = next(
    (
        actor
        for actor in actors
        if actor.get_actor_label() == "MAPVIEW_GLOBAL_DIRECTIONAL_LIGHT_EDIT_ME"
    ),
    None,
)
if map_light is None:
    map_light = actor_subsystem.spawn_actor_from_class(
        unreal.DirectionalLight,
        unreal.Vector(0.0, 0.0, 900.0),
        unreal.Rotator(-55.0, -30.0, 0.0),
    )
    map_light.set_actor_label("MAPVIEW_GLOBAL_DIRECTIONAL_LIGHT_EDIT_ME")
    map_light.set_folder_path("MenuLevel/MapView/06_Lighting")
    light_component = map_light.get_component_by_class(unreal.DirectionalLightComponent)
    light_component.set_editor_property("intensity", 2.0)
    light_component.set_mobility(unreal.ComponentMobility.MOVABLE)
add_tag(map_light, MAP_LIGHT_TAG)

controller = next(
    (actor for actor in actors if actor.get_actor_label() == "MAP_VIEW_CONTROLLER"), None
)
if controller:
    controller.apply_editor_preview()

level_subsystem.save_current_level()
unreal.log("MAP_VIEW_LIGHTING_UPDATE_COMPLETE")
