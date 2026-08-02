import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_ViewLevel"
ASSET_PATH = "/Game/HouseOfAtonal/Blueprints/Time/BP_HouseTimeController"
ASSET_DIR = "/Game/HouseOfAtonal/Blueprints/Time"
ASSET_NAME = "BP_HouseTimeController"
ACTOR_LABEL = "TIME_HouseTimeController"
UDS_CLASS_PATH = "/Game/UltraDynamicSky/Blueprints/Ultra_Dynamic_Sky.Ultra_Dynamic_Sky_C"


level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

if not level_subsystem.load_level(MAP_PATH):
    raise RuntimeError(f"Could not load {MAP_PATH}")

controller_class = unreal.load_class(None, "/Script/HouseOfAtonal.HouseTimeController")
if controller_class is None:
    raise RuntimeError("AHouseTimeController class is unavailable; build the editor target first")

blueprint = unreal.load_asset(ASSET_PATH)
if blueprint is None:
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    factory = unreal.BlueprintFactory()
    factory.set_editor_property("parent_class", controller_class)
    blueprint = asset_tools.create_asset(ASSET_NAME, ASSET_DIR, unreal.Blueprint, factory)
    if blueprint is None:
        raise RuntimeError(f"Could not create {ASSET_PATH}")

generated_class = blueprint.generated_class()
uds_class = unreal.load_class(None, UDS_CLASS_PATH)
if uds_class is None:
    raise RuntimeError(f"Could not load {UDS_CLASS_PATH}")

actors = actor_subsystem.get_all_level_actors()
uds_actor = next((actor for actor in actors if actor.get_class() == uds_class), None)
if uds_actor is None:
    raise RuntimeError("L_ViewLevel contains no Ultra_Dynamic_Sky actor")

controller = next(
    (
        actor
        for actor in actors
        if actor.get_class() == generated_class or actor.get_actor_label() == ACTOR_LABEL
    ),
    None,
)
controller_was_created = controller is None
if controller is None:
    controller = actor_subsystem.spawn_actor_from_class(
        generated_class, unreal.Vector(0.0, 0.0, 0.0), unreal.Rotator(0.0, 0.0, 0.0)
    )

controller.set_actor_label(ACTOR_LABEL)
controller.set_folder_path("HouseOfAtonal/Time")
controller.set_editor_property("ultra_dynamic_sky", uds_actor)

# Preserve the existing look on first placement and migrate the earlier
# limited implementation if its state contains no complete snapshot.
state_01 = controller.get_editor_property("time_state01")
state_01_snapshot = state_01.get_editor_property("snapshot")
if controller_was_created or len(state_01_snapshot) < 10:
    controller.capture_current_uds_time()

unreal.EditorAssetLibrary.save_loaded_asset(blueprint)
level_subsystem.save_current_level()

unreal.log(f"TIME_CONTROLLER_ASSET={blueprint.get_path_name()}")
unreal.log(f"TIME_CONTROLLER_ACTOR={controller.get_path_name()}")
unreal.log(f"TIME_CONTROLLER_UDS={uds_actor.get_path_name()}")
