import os

import unreal


SOURCE_DIR = r"D:\GIT\HouseOfAtonal\res\lucide"
DESTINATION = "/Game/HouseOfAtonal/UI/Icons/Lucide"
ICONS = (
    "map-pin", "cloud-sun", "clock-3",
    "trees", "armchair", "bed-double",
    "sun", "cloud", "cloud-rain",
    "sunrise", "moon",
)

tasks = []
for icon in ICONS:
    task = unreal.AssetImportTask()
    task.set_editor_property("filename", os.path.join(SOURCE_DIR, icon + ".png"))
    task.set_editor_property("destination_path", DESTINATION)
    task.set_editor_property("destination_name", "T_Lucide_" + icon.replace("-", "_"))
    task.set_editor_property("automated", True)
    task.set_editor_property("replace_existing", True)
    task.set_editor_property("save", True)
    tasks.append(task)

unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)

for task in tasks:
    for object_path in task.get_editor_property("imported_object_paths"):
        texture = unreal.EditorAssetLibrary.load_asset(str(object_path))
        if not texture:
            raise RuntimeError("Could not load imported icon " + str(object_path))
        texture.set_editor_property("lod_group", unreal.TextureGroup.TEXTUREGROUP_UI)
        texture.set_editor_property("compression_settings", unreal.TextureCompressionSettings.TC_EDITOR_ICON)
        texture.set_editor_property("mip_gen_settings", unreal.TextureMipGenSettings.TMGS_NO_MIPMAPS)
        texture.set_editor_property("never_stream", True)
        texture.set_editor_property("srgb", True)
        unreal.EditorAssetLibrary.save_loaded_asset(texture, only_if_is_dirty=False)

unreal.log("LUCIDE_IMPORT imported={}".format(sum(len(t.imported_object_paths) for t in tasks)))
