import bpy
import math
from pathlib import Path
from mathutils import Vector


# HOUSE OF ATONAL — PARAMETRIC ARCHITECTURAL BLOCKOUT
# Units are metres. These are design assumptions inferred from the reference
# board and should be replaced when dimensioned drawings are available.
OUTER_W = 12.0
OUTER_D = 14.0
COURT_W = 5.2
COURT_D = 7.0
COURT_Y = 1.0
FLOOR_H = 3.2
FLOORS = 3
SLAB_T = 0.24
EXT_WALL = 0.28
PARAPET_H = 0.55
FRAME = 0.12

ROOT = Path(__file__).resolve().parents[2]
BLEND_PATH = ROOT / "blender" / "HouseOfAtonal_blockout_v01.blend"
PREVIEW_PATH = ROOT / "blender" / "previews" / "HouseOfAtonal_blockout_v01.png"


def reset_scene():
    bpy.ops.object.select_all(action="SELECT")
    bpy.ops.object.delete(use_global=False)
    for datablocks in (
        bpy.data.meshes,
        bpy.data.curves,
        bpy.data.materials,
        bpy.data.cameras,
        bpy.data.lights,
    ):
        pass


def collection(name):
    col = bpy.data.collections.get(name)
    if not col:
        col = bpy.data.collections.new(name)
        bpy.context.scene.collection.children.link(col)
    return col


def move_to_collection(obj, col):
    for current in list(obj.users_collection):
        current.objects.unlink(obj)
    col.objects.link(obj)


def material(name, color, metallic=0.0, roughness=0.5, transmission=0.0):
    mat = bpy.data.materials.get(name) or bpy.data.materials.new(name)
    mat.diffuse_color = (*color, 1.0)
    mat.use_nodes = True
    bsdf = mat.node_tree.nodes.get("Principled BSDF")
    bsdf.inputs["Base Color"].default_value = (*color, 1.0)
    bsdf.inputs["Metallic"].default_value = metallic
    bsdf.inputs["Roughness"].default_value = roughness
    if "Transmission Weight" in bsdf.inputs:
        bsdf.inputs["Transmission Weight"].default_value = transmission
    return mat


def box(name, size, location, mat, col, bevel=0.0):
    bpy.ops.mesh.primitive_cube_add(location=location)
    obj = bpy.context.object
    obj.name = name
    obj.dimensions = size
    bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)
    if bevel:
        modifier = obj.modifiers.new("Edge softening", "BEVEL")
        modifier.width = bevel
        modifier.segments = 2
    obj.data.materials.append(mat)
    move_to_collection(obj, col)
    return obj


def ring_segments(prefix, z, thickness, mat, col):
    cy = COURT_Y
    left_w = (OUTER_W - COURT_W) / 2
    front_d = (OUTER_D - COURT_D) / 2 + cy
    back_d = (OUTER_D - COURT_D) / 2 - cy
    box(f"{prefix}_West", (left_w, OUTER_D, thickness),
        (-(COURT_W + left_w) / 2, 0, z), mat, col)
    box(f"{prefix}_East", (left_w, OUTER_D, thickness),
        ((COURT_W + left_w) / 2, 0, z), mat, col)
    box(f"{prefix}_South", (COURT_W, front_d, thickness),
        (0, -OUTER_D / 2 + front_d / 2, z), mat, col)
    box(f"{prefix}_North", (COURT_W, back_d, thickness),
        (0, OUTER_D / 2 - back_d / 2, z), mat, col)


def wall_y(name, x, y, length, z, height, mat, col):
    return box(name, (EXT_WALL, length, height), (x, y, z + height / 2), mat, col)


def wall_x(name, x, y, length, z, height, mat, col):
    return box(name, (length, EXT_WALL, height), (x, y, z + height / 2), mat, col)


def framed_glazing(name, x, y, width, height, z, orientation, glass, metal, col):
    depth = 0.06
    if orientation == "X":
        box(f"{name}_Glass", (width, depth, height), (x, y, z + height / 2), glass, col)
        box(f"{name}_FrameL", (FRAME, depth * 1.8, height), (x - width / 2, y, z + height / 2), metal, col)
        box(f"{name}_FrameR", (FRAME, depth * 1.8, height), (x + width / 2, y, z + height / 2), metal, col)
        box(f"{name}_FrameT", (width, depth * 1.8, FRAME), (x, y, z + height), metal, col)
        box(f"{name}_FrameB", (width, depth * 1.8, FRAME), (x, y, z), metal, col)
        box(f"{name}_Mullion", (FRAME * 0.7, depth * 1.8, height), (x, y, z + height / 2), metal, col)
    else:
        box(f"{name}_Glass", (depth, width, height), (x, y, z + height / 2), glass, col)
        box(f"{name}_FrameL", (depth * 1.8, FRAME, height), (x, y - width / 2, z + height / 2), metal, col)
        box(f"{name}_FrameR", (depth * 1.8, FRAME, height), (x, y + width / 2, z + height / 2), metal, col)
        box(f"{name}_FrameT", (depth * 1.8, width, FRAME), (x, y, z + height), metal, col)
        box(f"{name}_FrameB", (depth * 1.8, width, FRAME), (x, y, z), metal, col)
        box(f"{name}_Mullion", (depth * 1.8, FRAME * 0.7, height), (x, y, z + height / 2), metal, col)


def build_house():
    structure = collection("A_STRUCTURE")
    facade = collection("B_FACADE")
    glazing = collection("C_GLAZING")
    site = collection("D_SITE")
    presentation = collection("Z_PRESENTATION")

    concrete = material("Board-formed concrete", (0.58, 0.56, 0.52), roughness=0.78)
    slab_mat = material("Concrete slabs", (0.34, 0.34, 0.32), roughness=0.72)
    bronze = material("Bronze aluminium", (0.16, 0.11, 0.065), metallic=0.7, roughness=0.3)
    glass = material("Clear glass", (0.18, 0.31, 0.34), roughness=0.12, transmission=0.72)
    paving = material("Concrete paving", (0.40, 0.39, 0.37), roughness=0.9)
    greenery = material("Courtyard greenery", (0.13, 0.23, 0.09), roughness=0.95)
    dark = material("Garage door", (0.10, 0.085, 0.07), metallic=0.5, roughness=0.35)

    # Site and lowered courtyard ground.
    box("Site", (24, 26, 0.20), (0, 0, -0.20), paving, site)
    box("Courtyard ground", (COURT_W - 0.3, COURT_D - 0.3, 0.10),
        (0, COURT_Y, -0.03), greenery, site)

    # Floor and roof slabs expressed as four editable pieces around the atrium.
    for level in range(FLOORS + 1):
        z = level * FLOOR_H - SLAB_T / 2
        ring_segments(f"L{level:02d}_Slab", z, SLAB_T, slab_mat, structure)

    # Exterior wall bands. The south/front ground floor is composed around entry
    # and garage openings; upper floors are a calm monolithic envelope.
    for level in range(FLOORS):
        z = level * FLOOR_H
        h = FLOOR_H - SLAB_T
        wall_y(f"L{level+1:02d}_WestWall", -OUTER_W / 2 + EXT_WALL / 2, 0,
               OUTER_D, z, h, concrete, facade)
        wall_y(f"L{level+1:02d}_EastWall", OUTER_W / 2 - EXT_WALL / 2, 0,
               OUTER_D, z, h, concrete, facade)
        wall_x(f"L{level+1:02d}_NorthWall", 0, OUTER_D / 2 - EXT_WALL / 2,
               OUTER_W, z, h, concrete, facade)
        if level == 0:
            wall_x("L01_SouthLeft", -4.6, -OUTER_D / 2 + EXT_WALL / 2,
                   2.8, z, h, concrete, facade)
            wall_x("L01_SouthPier", 0.1, -OUTER_D / 2 + EXT_WALL / 2,
                   0.8, z, h, concrete, facade)
            wall_x("L01_SouthRight", 4.7, -OUTER_D / 2 + EXT_WALL / 2,
                   2.6, z, h, concrete, facade)
            box("Garage door", (3.6, 0.12, 2.55), (2.35, -OUTER_D / 2 - 0.01, 1.275),
                dark, facade)
            framed_glazing("Main entrance", -2.1, -OUTER_D / 2 - 0.02,
                           2.2, 2.75, 0, "X", glass, bronze, glazing)
        else:
            side_width = (OUTER_W - COURT_W) / 2
            wall_x(f"L{level+1:02d}_SouthLeft", -(COURT_W + side_width) / 2,
                   -OUTER_D / 2 + EXT_WALL / 2, side_width, z, h,
                   concrete, facade)
            wall_x(f"L{level+1:02d}_SouthRight", (COURT_W + side_width) / 2,
                   -OUTER_D / 2 + EXT_WALL / 2, side_width, z, h,
                   concrete, facade)
            framed_glazing(f"L{level+1:02d}_SouthAtrium", 0,
                           -OUTER_D / 2 + EXT_WALL / 2, COURT_W, h - 0.16,
                           z + 0.08, "X", glass, bronze, glazing)

    # Courtyard is the architectural hero: continuous glazed elevations.
    for level in range(FLOORS):
        z = level * FLOOR_H + 0.08
        gh = FLOOR_H - SLAB_T - 0.16
        framed_glazing(f"L{level+1:02d}_CourtSouth", 0, COURT_Y - COURT_D / 2,
                       COURT_W, gh, z, "X", glass, bronze, glazing)
        framed_glazing(f"L{level+1:02d}_CourtNorth", 0, COURT_Y + COURT_D / 2,
                       COURT_W, gh, z, "X", glass, bronze, glazing)
        framed_glazing(f"L{level+1:02d}_CourtWest", -COURT_W / 2, COURT_Y,
                       COURT_D, gh, z, "Y", glass, bronze, glazing)
        framed_glazing(f"L{level+1:02d}_CourtEast", COURT_W / 2, COURT_Y,
                       COURT_D, gh, z, "Y", glass, bronze, glazing)

    # Selective punched exterior windows establish façade scale.
    window_specs = [
        ("West_L2", -OUTER_W / 2 - 0.02, -2.5, 1.4, 1.65, FLOOR_H + 0.85, "Y"),
        ("West_L3", -OUTER_W / 2 - 0.02, 2.8, 1.2, 1.8, 2 * FLOOR_H + 0.75, "Y"),
        ("East_L2", OUTER_W / 2 + 0.02, 2.7, 1.35, 1.75, FLOOR_H + 0.8, "Y"),
    ]
    for name, x, y, width, height, z, orient in window_specs:
        framed_glazing(name, x, y, width, height, z, orient, glass, bronze, glazing)

    # Parapet around outer roof edge.
    roof_z = FLOORS * FLOOR_H
    box("Parapet_W", (EXT_WALL, OUTER_D, PARAPET_H),
        (-OUTER_W / 2 + EXT_WALL / 2, 0, roof_z + PARAPET_H / 2), concrete, facade)
    box("Parapet_E", (EXT_WALL, OUTER_D, PARAPET_H),
        (OUTER_W / 2 - EXT_WALL / 2, 0, roof_z + PARAPET_H / 2), concrete, facade)
    box("Parapet_N", (OUTER_W, EXT_WALL, PARAPET_H),
        (0, OUTER_D / 2 - EXT_WALL / 2, roof_z + PARAPET_H / 2), concrete, facade)
    box("Parapet_S", (OUTER_W, EXT_WALL, PARAPET_H),
        (0, -OUTER_D / 2 + EXT_WALL / 2, roof_z + PARAPET_H / 2), concrete, facade)

    # Simple courtyard tree placeholder.
    box("Tree trunk", (0.32, 0.32, 4.8), (0.3, COURT_Y + 0.2, 2.4),
        bronze, site, bevel=0.08)
    bpy.ops.mesh.primitive_ico_sphere_add(subdivisions=2, radius=1.65,
                                         location=(0.3, COURT_Y + 0.2, 4.9))
    crown = bpy.context.object
    crown.name = "Courtyard tree crown"
    crown.scale = (0.85, 1.0, 1.25)
    crown.data.materials.append(greenery)
    move_to_collection(crown, site)

    # Camera and neutral daylight preview.
    bpy.ops.object.camera_add(location=(20.5, -24.0, 17.0))
    camera = bpy.context.object
    camera.name = "Preview camera"
    move_to_collection(camera, presentation)
    bpy.context.scene.camera = camera
    direction = Vector((0, 0, 4.2)) - camera.location
    camera.rotation_euler = direction.to_track_quat("-Z", "Y").to_euler()
    camera.data.lens = 48

    bpy.ops.object.light_add(type="SUN", location=(4, -6, 16))
    sun = bpy.context.object
    sun.name = "Preview sun"
    sun.rotation_euler = (math.radians(28), math.radians(-18), math.radians(-35))
    sun.data.energy = 2.4
    sun.data.angle = math.radians(12)
    move_to_collection(sun, presentation)

    bpy.ops.object.light_add(type="AREA", location=(-5, -6, 14))
    area = bpy.context.object
    area.name = "Preview softbox"
    area.data.energy = 1500
    area.data.shape = "DISK"
    area.data.size = 8
    direction = Vector((0, 0, 3)) - area.location
    area.rotation_euler = direction.to_track_quat("-Z", "Y").to_euler()
    move_to_collection(area, presentation)


def configure_and_save():
    scene = bpy.context.scene
    scene.unit_settings.system = "METRIC"
    scene.unit_settings.length_unit = "METERS"
    scene.render.engine = "BLENDER_EEVEE"
    scene.render.resolution_x = 1200
    scene.render.resolution_y = 1200
    scene.render.resolution_percentage = 100
    scene.render.image_settings.file_format = "PNG"
    scene.render.filepath = str(PREVIEW_PATH)
    scene.render.film_transparent = False
    scene.world.color = (0.055, 0.055, 0.055)
    scene.view_settings.look = "AgX - Medium High Contrast"
    PREVIEW_PATH.parent.mkdir(parents=True, exist_ok=True)
    BLEND_PATH.parent.mkdir(parents=True, exist_ok=True)
    bpy.ops.wm.save_as_mainfile(filepath=str(BLEND_PATH))
    bpy.ops.render.render(write_still=True)
    bpy.ops.wm.save_as_mainfile(filepath=str(BLEND_PATH))


reset_scene()
build_house()
configure_and_save()
