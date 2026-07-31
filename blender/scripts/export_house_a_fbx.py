from pathlib import Path

import bpy


blend_path = Path(bpy.data.filepath)
output_path = blend_path.with_suffix(".fbx")

# Export all mesh objects from the saved architectural source. Cameras, lights,
# empties and animation are intentionally excluded from the Unreal exchange.
bpy.ops.object.select_all(action="DESELECT")
for obj in bpy.context.view_layer.objects:
    if obj.type == "MESH" and not obj.hide_render:
        obj.select_set(True)

bpy.ops.export_scene.fbx(
    filepath=str(output_path),
    use_selection=True,
    object_types={"MESH"},
    global_scale=1.0,
    apply_unit_scale=True,
    apply_scale_options="FBX_SCALE_ALL",
    use_mesh_modifiers=True,
    mesh_smooth_type="FACE",
    use_triangles=False,
    axis_forward="-Z",
    axis_up="Y",
    path_mode="AUTO",
    bake_anim=False,
)

print(f"HOUSE_A_EXPORT_OK={output_path}")
