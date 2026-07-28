"""Build a hand-editable Menu Level from ordinary Unreal actors."""

from math import atan2, cos, degrees, hypot, radians, sin
from pathlib import Path
import random

import unreal


MAP = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"
TEMP_MAP = "/Game/HouseOfAtonal/Maps/L_MenuLevel_BuildTemp"
CONTENT = "/Game/HouseOfAtonal/MenuLevel"
HOUSE = "/Game/HouseOfAtonal/Architecture/SM_HeroHouse"


def color_material(name, color):
    path = f"{CONTENT}/Materials/{name}"
    existing = unreal.EditorAssetLibrary.load_asset(path)
    if existing:
        return existing
    tools = unreal.AssetToolsHelpers.get_asset_tools()
    material = tools.create_asset(
        name, f"{CONTENT}/Materials", unreal.Material, unreal.MaterialFactoryNew()
    )
    base = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionConstant3Vector, -250, 0
    )
    base.set_editor_property("constant", unreal.LinearColor(*color, 1.0))
    unreal.MaterialEditingLibrary.connect_material_property(
        base, "", unreal.MaterialProperty.MP_BASE_COLOR
    )
    rough = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionConstant, -250, 120
    )
    rough.set_editor_property("r", 0.82)
    unreal.MaterialEditingLibrary.connect_material_property(
        rough, "", unreal.MaterialProperty.MP_ROUGHNESS
    )
    unreal.MaterialEditingLibrary.recompile_material(material)
    unreal.EditorAssetLibrary.save_loaded_asset(material)
    return material


def folder(actor, path):
    actor.set_folder_path(path)
    return actor


def box(label, center, size, yaw, material, actor_folder):
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.StaticMeshActor, unreal.Vector(*center), unreal.Rotator(0.0, 0.0, yaw)
    )
    actor.set_actor_label(label)
    folder(actor, actor_folder)
    component = actor.static_mesh_component
    component.set_static_mesh(cube)
    component.set_material(0, material)
    component.set_editor_property("mobility", unreal.ComponentMobility.STATIC)
    actor.set_actor_scale3d(
        unreal.Vector(size[0] / 100.0, size[1] / 100.0, size[2] / 100.0)
    )
    return actor


def segment(label, start, end, width, z, material, actor_folder):
    dx, dy = end[0] - start[0], end[1] - start[1]
    length = hypot(dx, dy)
    return box(
        label,
        ((start[0] + end[0]) * 0.5, (start[1] + end[1]) * 0.5, z),
        (length + width, width, 0.35),
        degrees(atan2(dy, dx)),
        material,
        actor_folder,
    )


def local_to_world(block, lx, ly):
    cx, cy, _, _, yaw = block
    angle = radians(yaw)
    return (
        cx + lx * cos(angle) - ly * sin(angle),
        cy + lx * sin(angle) + ly * cos(angle),
    )


if unreal.EditorAssetLibrary.does_asset_exist(MAP):
    unreal.EditorLevelLibrary.load_level(MAP)
    for existing_actor in unreal.EditorLevelLibrary.get_all_level_actors():
        unreal.EditorLevelLibrary.destroy_actor(existing_actor)
elif not unreal.EditorLevelLibrary.new_level(MAP):
    raise RuntimeError(f"Could not create {MAP}")

cube = unreal.EditorAssetLibrary.load_asset("/Engine/BasicShapes/Cube")
hero_mesh = unreal.EditorAssetLibrary.load_asset(HOUSE)
if not cube or not hero_mesh:
    raise RuntimeError("Cube or SM_HeroHouse is missing")

white = color_material("M_MenuContext_White", (0.72, 0.72, 0.68))
road = color_material("M_MenuRoad_Dark", (0.055, 0.06, 0.065))
ground = color_material("M_MenuGround_Dark", (0.008, 0.009, 0.012))

box("Ground_Dark", (0, 0, -2.0), (520, 520, 3.0), 0, ground, "MenuLevel/Ground")

# Explicit, intentionally non-parallel street paths. Every segment remains
# individually movable in the level.
road_paths = [
    [(-205, -205), (-190, -120), (-198, -30), (-182, 65), (-190, 205)],
    [(-116, -210), (-105, -135), (-118, -45), (-103, 45), (-112, 120), (-95, 208)],
    [(-34, -208), (-44, -128), (-31, -48), (-40, 34), (-25, 118), (-34, 205)],
    [(45, -205), (38, -132), (52, -52), (42, 30), (56, 112), (48, 205)],
    [(132, -208), (119, -126), (137, -38), (124, 50), (141, 130), (128, 208)],
    [(205, -195), (192, -112), (205, -18), (191, 75), (205, 195)],
    [(-208, -188), (-125, -176), (-42, -190), (45, -178), (126, -192), (208, -180)],
    [(-210, -105), (-126, -116), (-43, -101), (43, -112), (127, -96), (207, -108)],
    [(-208, -25), (-119, -12), (-38, -31), (47, -18), (132, -32), (208, -16)],
    [(-207, 57), (-128, 43), (-38, 62), (48, 48), (125, 66), (208, 52)],
    [(-205, 140), (-116, 126), (-31, 146), (55, 132), (137, 149), (207, 135)],
    [(-197, 205), (-110, 194), (-25, 208), (58, 195), (142, 207), (205, 194)],
    [(-205, 92), (-135, 25), (-62, -40), (4, -105)],
]
road_index = 0
for path in road_paths:
    for start, end in zip(path, path[1:]):
        segment(
            f"Road_{road_index:03d}",
            start,
            end,
            9.0 if road_index % 7 else 12.0,
            0.2,
            road,
            "MenuLevel/Roads",
        )
        road_index += 1

# Authored irregular blocks. They are ordinary actors and serve only as an
# editable starting composition, not a runtime generator.
blocks = [
    (-158, -148, 56, 48, 7), (-76, -151, 62, 45, -5), (8, -148, 58, 48, 6),
    (91, -151, 61, 44, -7), (169, -146, 48, 50, 8),
    (-158, -67, 52, 55, -6), (-76, -65, 59, 51, 8), (6, -68, 54, 53, -5),
    (91, -63, 62, 49, 7), (169, -65, 48, 55, -7),
    (-158, 18, 57, 54, 8), (-77, 18, 55, 52, -7), (0, 16, 48, 46, 3),
    (88, 20, 62, 52, -8), (167, 20, 48, 56, 6),
    (-158, 101, 55, 52, -7), (-75, 101, 61, 47, 6), (8, 101, 58, 52, -5),
    (94, 103, 59, 48, 8), (171, 101, 46, 53, -6),
    (-151, 172, 62, 38, 5), (-64, 174, 58, 37, -6), (20, 172, 62, 39, 5),
    (108, 174, 62, 37, -5), (179, 171, 42, 38, 7),
]

rng = random.Random(4127)
for block_index, block in enumerate(blocks):
    cx, cy, sx, sy, yaw = block
    box(
        f"Parcel_{block_index:02d}",
        (cx, cy, 0.65),
        (sx, sy, 0.9),
        yaw,
        white,
        "MenuLevel/Parcels",
    )
    if block_index == 12:
        continue
    columns = rng.choice((2, 2, 3))
    rows = rng.choice((1, 2, 2))
    cell_x, cell_y = sx / columns, sy / rows
    for column in range(columns):
        for row_index in range(rows):
            if rng.random() < 0.13:
                continue
            footprint_x = cell_x * rng.uniform(0.58, 0.84)
            footprint_y = cell_y * rng.uniform(0.58, 0.84)
            floors = rng.choices((1, 2, 3, 4), weights=(3, 6, 4, 0.35))[0]
            height = floors * rng.uniform(4.6, 5.4)
            lx = -sx * 0.5 + (column + 0.5) * cell_x + rng.uniform(-2.2, 2.2)
            ly = -sy * 0.5 + (row_index + 0.5) * cell_y + rng.uniform(-2.2, 2.2)
            wx, wy = local_to_world(block, lx, ly)
            box(
                f"Building_{block_index:02d}_{column}_{row_index}",
                (wx, wy, 1.1 + height * 0.5),
                (footprint_x, footprint_y, height),
                yaw,
                white,
                "MenuLevel/Buildings",
            )

hero = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.StaticMeshActor, unreal.Vector(0.0, 16.0, 1.2), unreal.Rotator(0, 0, 3)
)
hero.set_actor_label("HERO_HOUSE")
folder(hero, "MenuLevel/Hero")
hero.static_mesh_component.set_static_mesh(hero_mesh)
hero.static_mesh_component.set_editor_property("mobility", unreal.ComponentMobility.STATIC)
hero.set_actor_scale3d(unreal.Vector(0.01, 0.01, 0.01))

player_start = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.PlayerStart,
    unreal.Vector(-285.0, -285.0, 310.0),
    unreal.Rotator(0.0, -37.0, 45.0),
)
player_start.set_actor_label("MENU_VIEWPOINT_MOVE_ME")
folder(player_start, "MenuLevel/View")

directional = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.DirectionalLight,
    unreal.Vector(0, 0, 180),
    unreal.Rotator(0, -28, -38),
)
directional.set_actor_label("GoldenHour_Fill")
folder(directional, "MenuLevel/Lighting")
directional_component = directional.get_component_by_class(
    unreal.DirectionalLightComponent
)
directional_component.set_editor_property("intensity", 0.25)
directional_component.set_editor_property(
    "light_color", unreal.Color(255, 176, 112, 255)
)
directional_component.set_editor_property("light_source_angle", 5.0)

spot = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.SpotLight, unreal.Vector(-150, -130, 230)
)
spot.set_actor_label("Hero_Sun_Beam")
folder(spot, "MenuLevel/Lighting")
spot.set_actor_rotation(
    unreal.MathLibrary.find_look_at_rotation(spot.get_actor_location(), hero.get_actor_location()),
    False,
)
component = spot.get_component_by_class(unreal.SpotLightComponent)
component.set_editor_property("intensity", 5200.0)
component.set_editor_property("attenuation_radius", 430.0)
component.set_editor_property("inner_cone_angle", 13.0)
component.set_editor_property("outer_cone_angle", 32.0)
component.set_editor_property("source_radius", 22.0)
component.set_editor_property("soft_source_radius", 42.0)
component.set_editor_property("light_color", unreal.Color(255, 167, 92, 255))

world = unreal.EditorLevelLibrary.get_editor_world()
game_mode = unreal.load_class(None, "/Script/HouseOfAtonal.HouseMenuLevelGameMode")
world.get_world_settings().set_editor_property("default_game_mode", game_mode)
world.get_world_settings().set_editor_property("force_no_precomputed_lighting", True)
unreal.EditorLevelLibrary.save_current_level()
unreal.EditorAssetLibrary.save_directory("/Game/HouseOfAtonal", only_if_is_dirty=False)
if unreal.EditorAssetLibrary.does_asset_exist(TEMP_MAP):
    unreal.EditorAssetLibrary.delete_asset(TEMP_MAP)
unreal.log(f"[FORGE] Editable Menu Level ready: {MAP}")
