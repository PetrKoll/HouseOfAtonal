import unreal


ASSET_PATH = "/Game/HouseOfAtonal/Architecture/SM_HeroHouse"
SOURCE_FILE = r"D:\GIT\HouseOfAtonal\blender\house_A.fbx"

if unreal.load_asset(ASSET_PATH) is None:
    raise RuntimeError(f"Could not load existing asset {ASSET_PATH}")

task = unreal.AssetImportTask()
task.set_editor_property("filename", SOURCE_FILE)
task.set_editor_property("destination_path", "/Game/HouseOfAtonal/Architecture")
task.set_editor_property("destination_name", "SM_HeroHouse")
task.set_editor_property("automated", True)
task.set_editor_property("replace_existing", True)
task.set_editor_property("replace_existing_settings", False)
task.set_editor_property("save", True)

unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])

asset = unreal.load_asset(ASSET_PATH)
imported_paths = list(task.get_editor_property("imported_object_paths"))
imported_packages = [path.split(".", 1)[0] for path in imported_paths]
if asset is None or ASSET_PATH not in imported_packages:
    raise RuntimeError(
        f"Reimport failed for {ASSET_PATH}; imported={imported_paths}"
    )

unreal.EditorAssetLibrary.save_loaded_asset(asset, only_if_is_dirty=False)
unreal.log_warning(f"HOUSE_A_REIMPORT_OK={asset.get_path_name()}")
