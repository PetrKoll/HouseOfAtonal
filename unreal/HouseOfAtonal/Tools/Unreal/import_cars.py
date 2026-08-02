import unreal


SOURCE_FILE = r"D:\GIT\HouseOfAtonal\blender\cars.fbx"
DESTINATION = "/Game/HouseOfAtonal/models/cars"

options = unreal.FbxImportUI()
options.set_editor_property("import_mesh", True)
options.set_editor_property("import_as_skeletal", False)
options.set_editor_property("mesh_type_to_import", unreal.FBXImportType.FBXIT_STATIC_MESH)
options.set_editor_property("import_materials", False)
options.set_editor_property("import_textures", False)

static_options = options.get_editor_property("static_mesh_import_data")
static_options.set_editor_property("combine_meshes", False)
static_options.set_editor_property("convert_scene", True)
static_options.set_editor_property("generate_lightmap_u_vs", True)
static_options.set_editor_property("auto_generate_collision", False)

task = unreal.AssetImportTask()
task.set_editor_property("filename", SOURCE_FILE)
task.set_editor_property("destination_path", DESTINATION)
task.set_editor_property("automated", True)
task.set_editor_property("replace_existing", True)
task.set_editor_property("save", True)
task.set_editor_property("options", options)

unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])

for path in task.get_editor_property("imported_object_paths"):
    asset = unreal.load_asset(path)
    if isinstance(asset, unreal.StaticMesh):
        bounds = asset.get_bounds()
        size = bounds.box_extent * 2.0
        unreal.log_warning(
            f"CAR_IMPORT asset={path} size_cm=({size.x:.2f},{size.y:.2f},{size.z:.2f})"
        )

unreal.log_warning("CAR_IMPORT_DONE")
