import unreal


ASSET_PATH = "/Game/UltraDynamicSky/Blueprints/Ultra_Dynamic_Sky"


blueprint = unreal.load_asset(ASSET_PATH)
if blueprint is None:
    raise RuntimeError(f"Could not load {ASSET_PATH}")

generated_class = blueprint.generated_class()
default_object = unreal.get_default_object(generated_class)

unreal.log(f"UDS_ASSET={blueprint.get_path_name()}")
unreal.log(f"UDS_CLASS={generated_class.get_path_name()}")

keywords = ("time", "day", "sun", "date", "hour", "animate", "update")
for name in sorted(dir(default_object)):
    if any(keyword in name.lower() for keyword in keywords):
        unreal.log(f"UDS_MEMBER={name}")

world = unreal.EditorLevelLibrary.get_editor_world()
for actor in unreal.EditorLevelLibrary.get_all_level_actors():
    if actor.get_class() == generated_class:
        unreal.log(f"UDS_LEVEL_ACTOR={actor.get_actor_label()}::{actor.get_path_name()}")
