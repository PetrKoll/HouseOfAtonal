import unreal


MAP = "/Game/HouseOfAtonal/Maps/L_ViewLevel"
unreal.EditorLoadingAndSavingUtils.load_map(MAP)

controller = None
rain_actors = []
for actor in unreal.EditorLevelLibrary.get_all_level_actors():
    class_path = actor.get_class().get_path_name().lower().replace("_", "")
    if "housetimecontroller" in class_path:
        controller = actor
    elif "/easyrain/" in class_path:
        rain_actors.append(actor)

if controller is None:
    raise RuntimeError("L_ViewLevel has no HouseTimeController")
if not rain_actors:
    raise RuntimeError("L_ViewLevel has no EasyRain actors")

controller.set_editor_property("rain_actors", rain_actors)
controller.modify()
unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
unreal.log(
    "HOUSE_WEATHER_CONFIG controller={} rain_actors={}".format(
        controller.get_actor_label(),
        ",".join(actor.get_actor_label() for actor in rain_actors),
    )
)
