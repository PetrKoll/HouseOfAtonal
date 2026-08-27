import unreal


MAP = "/Game/HouseOfAtonal/Maps/L_ViewLevel"
unreal.EditorLoadingAndSavingUtils.load_map(MAP)
arrival_class = unreal.load_class(None, "/Script/HouseOfAtonal.HouseViewArrivalPoint")
if not arrival_class:
    raise RuntimeError("HouseViewArrivalPoint class is unavailable")

expected = {
    unreal.HouseLocation.ATRIUM: "ARRIVAL_ATRIUM",
    unreal.HouseLocation.ROOM_ONE: "ARRIVAL_ROOM_1",
    unreal.HouseLocation.ROOM_TWO: "ARRIVAL_ROOM_2",
}
world = unreal.EditorLevelLibrary.get_editor_world()
arrivals = unreal.GameplayStatics.get_all_actors_of_class(world, arrival_class)

for destination, label in expected.items():
    matches = [
        actor for actor in arrivals
        if actor.get_editor_property("destination") == destination
    ]
    if len(matches) != 1:
        raise RuntimeError("{} has {} arrival points".format(label, len(matches)))
    actor = matches[0]
    unreal.log_warning("ARRIVAL_OK {} location={} rotation={}".format(
        actor.get_actor_label(), actor.get_actor_location(), actor.get_actor_rotation()
    ))
