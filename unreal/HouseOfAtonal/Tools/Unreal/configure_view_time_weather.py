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
time_groups = {"time_state01_actors": [], "time_state02_actors": [], "time_state03_actors": []}
for actor in unreal.EditorLevelLibrary.get_all_level_actors():
    folder_parts = str(actor.get_folder_path()).replace("\\", "/").lower().split("/")
    for state_index in range(1, 4):
        if f"time_0{state_index}" in folder_parts:
            time_groups[f"time_state0{state_index}_actors"].append(actor)

for property_name, actors in time_groups.items():
    controller.set_editor_property(property_name, actors)
controller.modify()
for rain_actor in rain_actors:
    location = rain_actor.get_actor_location()
    rain_actor.set_actor_location(
        unreal.Vector(location.x, location.y, 300.0),
        False,
        False,
    )
    rain_actor.modify()
unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
unreal.log(
    "HOUSE_WEATHER_CONFIG controller={} rain_actors={}".format(
        controller.get_actor_label(),
        ",".join(actor.get_actor_label() for actor in rain_actors),
    )
)
unreal.log(
    "HOUSE_TIME_CONTENT_CONFIG " + " ".join(
        f"{name}={len(actors)}" for name, actors in time_groups.items()
    )
)
