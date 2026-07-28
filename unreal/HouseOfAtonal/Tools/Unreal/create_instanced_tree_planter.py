import unreal


ASSET_PATH = "/Game/HouseOfAtonal/World/Vegetation"
ASSET_NAME = "BP_InstancedTreePlanter"


def main():
    full_asset_path = f"{ASSET_PATH}/{ASSET_NAME}"
    if unreal.EditorAssetLibrary.does_asset_exist(full_asset_path):
        unreal.log(f"{full_asset_path} already exists; leaving it unchanged.")
        return

    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    factory = unreal.BlueprintFactory()
    factory.set_editor_property("parent_class", unreal.HouseInstancedMeshPlanter)

    blueprint = asset_tools.create_asset(
        ASSET_NAME,
        ASSET_PATH,
        unreal.Blueprint,
        factory,
    )
    if blueprint is None:
        raise RuntimeError(f"Could not create {full_asset_path}")

    unreal.EditorAssetLibrary.save_loaded_asset(blueprint)
    unreal.log(f"Created {full_asset_path}")


main()
