import csv
import os

import unreal


MAPS = {
    "menu": "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable",
    "view": "/Game/HouseOfAtonal/Maps/L_ViewLevel",
}


registry = unreal.AssetRegistryHelpers.get_asset_registry()
options = unreal.AssetRegistryDependencyOptions(
    include_soft_package_references=True,
    include_hard_package_references=True,
    include_searchable_names=False,
    include_soft_management_references=True,
    include_hard_management_references=True,
)


def dependency_closure(roots):
    seen = set(roots)
    pending = list(roots)
    parent = {}
    while pending:
        package = pending.pop()
        for dependency in registry.get_dependencies(package, options):
            name = str(dependency)
            if name.startswith("/Game/") and name not in seen:
                seen.add(name)
                pending.append(name)
                parent[name] = package
    return seen, parent


def dependency_path(package, parent):
    path = [package]
    while package in parent:
        package = parent[package]
        path.append(package)
    return " <- ".join(path)


def prop(obj, name, default=""):
    try:
        value = obj.get_editor_property(name)
        return str(value)
    except Exception:
        return default


closures = {name: dependency_closure((path,)) for name, path in MAPS.items()}
map_dependencies = {name: value[0] for name, value in closures.items()}
map_parents = {name: value[1] for name, value in closures.items()}
referenced = set().union(*map_dependencies.values())
rows = []
for package in sorted(referenced):
    assets = registry.get_assets_by_package_name(package)
    for asset_data in assets:
        if str(asset_data.asset_class_path.asset_name) not in ("Texture2D", "TextureCube"):
            continue
        texture = asset_data.get_asset()
        try:
            width = texture.blueprint_get_size_x()
            height = texture.blueprint_get_size_y()
        except Exception:
            width = height = 0
        try:
            memory_bytes = texture.calc_texture_memory_size_enum(unreal.TextureMipCount.TMC_ALL_MIPS)
        except Exception:
            memory_bytes = 0
        rows.append({
            "menu": package in map_dependencies["menu"],
            "view": package in map_dependencies["view"],
            "menu_path": dependency_path(package, map_parents["menu"]) if package in map_dependencies["menu"] else "",
            "view_path": dependency_path(package, map_parents["view"]) if package in map_dependencies["view"] else "",
            "memory_mb": round(memory_bytes / 1048576.0, 3),
            "width": width,
            "height": height,
            "max_texture_size": prop(texture, "max_texture_size", "0"),
            "lod_bias": prop(texture, "lod_bias", "0"),
            "never_stream": prop(texture, "never_stream", ""),
            "virtual_texture_streaming": prop(texture, "virtual_texture_streaming", ""),
            "lod_group": prop(texture, "lod_group", ""),
            "compression": prop(texture, "compression_settings", ""),
            "package": package,
        })

rows.sort(key=lambda row: row["memory_mb"], reverse=True)
output_dir = os.path.join(unreal.Paths.project_saved_dir(), "TextureAudit")
os.makedirs(output_dir, exist_ok=True)
output_path = os.path.join(output_dir, "referenced_textures.csv")
with open(output_path, "w", newline="", encoding="utf-8-sig") as handle:
    writer = csv.DictWriter(handle, fieldnames=rows[0].keys() if rows else ["package"])
    writer.writeheader()
    writer.writerows(rows)

unreal.log("TEXTURE_AUDIT maps={} packages={} textures={} total_mb={:.1f} output={}".format(
    len(MAPS), len(referenced), len(rows), sum(row["memory_mb"] for row in rows), output_path
))
unreal.log("TEXTURE_AUDIT pool_size_mb={}".format(
    unreal.SystemLibrary.get_console_variable_int_value("r.Streaming.PoolSize")
))
with open(os.path.join(output_dir, "pool_size_mb.txt"), "w", encoding="utf-8") as handle:
    handle.write(str(unreal.SystemLibrary.get_console_variable_int_value("r.Streaming.PoolSize")))
