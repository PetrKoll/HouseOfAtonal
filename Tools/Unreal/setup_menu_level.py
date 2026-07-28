"""Create the editable Menu Level V1 and import the current hero-house FBX."""

from pathlib import Path

import unreal


REPO_ROOT = Path(__file__).resolve().parents[2]
FBX_FILE = REPO_ROOT / "blender" / "house_A.fbx"
HOUSE_FOLDER = "/Game/HouseOfAtonal/Architecture"
HOUSE_ASSET = f"{HOUSE_FOLDER}/SM_HeroHouse"
MAP_ASSET = "/Game/HouseOfAtonal/Maps/L_MenuLevel_V1"


def import_house() -> unreal.StaticMesh:
    if not FBX_FILE.exists():
        raise RuntimeError(f"Hero-house export is missing: {FBX_FILE}")

    options = unreal.FbxImportUI()
    options.set_editor_property("import_mesh", True)
    options.set_editor_property("import_as_skeletal", False)
    options.set_editor_property("import_materials", True)
    options.set_editor_property("import_textures", True)

    static_options = options.get_editor_property("static_mesh_import_data")
    static_options.set_editor_property("combine_meshes", True)
    static_options.set_editor_property("convert_scene", True)
    static_options.set_editor_property("convert_scene_unit", True)
    static_options.set_editor_property("generate_lightmap_u_vs", True)

    task = unreal.AssetImportTask()
    task.set_editor_property("filename", str(FBX_FILE))
    task.set_editor_property("destination_path", HOUSE_FOLDER)
    task.set_editor_property("destination_name", "SM_HeroHouse")
    task.set_editor_property("automated", True)
    task.set_editor_property("replace_existing", True)
    task.set_editor_property("save", True)
    task.set_editor_property("options", options)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])

    house = unreal.EditorAssetLibrary.load_asset(HOUSE_ASSET)
    if not isinstance(house, unreal.StaticMesh):
        raise RuntimeError(f"Static mesh import failed: {HOUSE_ASSET}")
    return house


def create_level(house: unreal.StaticMesh) -> None:
    if unreal.EditorAssetLibrary.does_asset_exist(MAP_ASSET):
        unreal.EditorAssetLibrary.delete_asset(MAP_ASSET)

    if not unreal.EditorLevelLibrary.new_level(MAP_ASSET):
        raise RuntimeError(f"Could not create level: {MAP_ASSET}")

    blockout_class = unreal.load_class(
        None, "/Script/HouseOfAtonal.HouseMenuLevelBlockout"
    )
    game_mode_class = unreal.load_class(
        None, "/Script/HouseOfAtonal.HouseMenuLevelGameMode"
    )
    if not blockout_class or not game_mode_class:
        raise RuntimeError("Menu Level native classes are unavailable")

    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        blockout_class, unreal.Vector(0.0, 0.0, 0.0)
    )
    actor.set_actor_label("MenuLevel_Diorama_V1")
    actor.set_editor_property("hero_house_mesh", house)
    actor.set_editor_property(
        "hero_house_rotation", unreal.Rotator(roll=0.0, pitch=0.0, yaw=35.0)
    )
    actor.rebuild_diorama()

    world = unreal.EditorLevelLibrary.get_editor_world()
    world_settings = world.get_world_settings()
    world_settings.set_editor_property("default_game_mode", game_mode_class)
    world_settings.set_editor_property("force_no_precomputed_lighting", True)

    unreal.EditorLevelLibrary.save_current_level()
    unreal.EditorAssetLibrary.save_directory("/Game/HouseOfAtonal", only_if_is_dirty=False)


house_mesh = import_house()
create_level(house_mesh)
unreal.log(f"[FORGE] Menu Level V1 ready: {MAP_ASSET}")
