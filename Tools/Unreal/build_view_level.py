"""Create the editable full-scale View Level and its three arrival points."""

import unreal


MAP = "/Game/HouseOfAtonal/Maps/L_ViewLevel"
HOUSE = "/Game/HouseOfAtonal/Architecture/SM_HeroHouse"

if unreal.EditorAssetLibrary.does_asset_exist(MAP):
    unreal.EditorLevelLibrary.load_level(MAP)
    for actor in unreal.EditorLevelLibrary.get_all_level_actors():
        unreal.EditorLevelLibrary.destroy_actor(actor)
elif not unreal.EditorLevelLibrary.new_level(MAP):
    raise RuntimeError(f"Could not create {MAP}")

cube = unreal.EditorAssetLibrary.load_asset("/Engine/BasicShapes/Cube")
house_mesh = unreal.EditorAssetLibrary.load_asset(HOUSE)
ground_material = unreal.EditorAssetLibrary.load_asset(
    "/Game/HouseOfAtonal/MenuLevel/Materials/M_MenuGround_Dark"
)
if not cube or not house_mesh:
    raise RuntimeError("View Level source assets are missing")

ground = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.StaticMeshActor, unreal.Vector(0, 0, -10)
)
ground.set_actor_label("VIEW_GROUND")
ground.set_folder_path("ViewLevel/Ground")
ground.static_mesh_component.set_static_mesh(cube)
if ground_material:
    ground.static_mesh_component.set_material(0, ground_material)
ground.set_actor_scale3d(unreal.Vector(40.0, 40.0, 0.2))

hero = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.StaticMeshActor, unreal.Vector(0, 0, 0)
)
hero.set_actor_label("HERO_HOUSE_VIEW")
hero.set_folder_path("ViewLevel/House")
hero.static_mesh_component.set_static_mesh(house_mesh)

fallback = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.PlayerStart, unreal.Vector(-1200, -1200, 100), unreal.Rotator(0, 0, 45)
)
fallback.set_actor_label("VIEW_FALLBACK_PLAYERSTART")
fallback.set_folder_path("ViewLevel/Viewpoints")

arrival_class = unreal.load_class(
    None, "/Script/HouseOfAtonal.HouseViewArrivalPoint"
)
if not arrival_class:
    raise RuntimeError("HouseViewArrivalPoint class is missing")

arrival_data = [
    (
        "ARRIVAL_ATRIUM",
        unreal.Vector(0, 0, 100),
        unreal.Rotator(0, 0, 0),
        unreal.HouseLocation.ATRIUM,
    ),
    (
        "ARRIVAL_ROOM_1",
        unreal.Vector(-420, 260, 100),
        unreal.Rotator(0, 0, -35),
        unreal.HouseLocation.ROOM_ONE,
    ),
    (
        "ARRIVAL_ROOM_2",
        unreal.Vector(420, 260, 100),
        unreal.Rotator(0, 0, -145),
        unreal.HouseLocation.ROOM_TWO,
    ),
]
for label, location, rotation, destination in arrival_data:
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        arrival_class, location, rotation
    )
    actor.set_actor_label(label)
    actor.set_folder_path("ViewLevel/Viewpoints")
    actor.set_editor_property("destination", destination)

world = unreal.EditorLevelLibrary.get_editor_world()
game_mode = unreal.load_class(None, "/Script/HouseOfAtonal.HouseMenuLevelGameMode")
world.get_world_settings().set_editor_property("default_game_mode", game_mode)
world.get_world_settings().set_editor_property("force_no_precomputed_lighting", True)
unreal.EditorLevelLibrary.save_current_level()
unreal.log(f"[FORGE] View Level ready: {MAP}")
