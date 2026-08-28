import csv
import os

import unreal


AUDIT = os.path.join(unreal.Paths.project_saved_dir(), "TextureAudit", "referenced_textures.csv")
MANIFEST = os.path.join(unreal.Paths.project_saved_dir(), "TextureAudit", "texture_budget_changes.csv")


def target_size(row):
    package = row["package"]
    width = int(row["width"] or 0)
    if row["view"].lower() != "true":
        return None, ""

    # Project-authored architecture and surface textures remain untouched.
    if package.startswith("/Game/HouseOfAtonal/"):
        if "/models/B_e/AM291_005_fabric_" in package and width >= 4096:
            if "Roughness" in package:
                return 1024, "sofa roughness has low visible spatial detail"
            return 2048, "sofa fabric map"
        return None, ""

    if package == "/Game/MSPresets/MS_Foliage_Material/MaterialFunctions/Textures/T_WindNoise":
        return 512, "procedural wind utility noise"

    if package == "/Game/UltraDynamicSky/Textures/Sky/Real_Stars":
        return 2048, "background star field"

    if package.startswith("/Game/BlackAlder/") and width >= 8192:
        if "/Impostor/" in package or "Normal" in package or "Mask" in package:
            return 2048, "foliage impostor, normal, or mask"
        return 4096, "foliage albedo"

    if package.startswith("/Game/Fab/") and (width >= 8192 or "_8K_" in package):
        if "/Decals/" in package:
            return 2048, "surface decal"
        if package.endswith("_N") or package.endswith("_ORM") or package.endswith("_ORT"):
            return 2048, "Megascans normal or packed data"
        return 4096, "Megascans base color"

    return None, ""


with open(AUDIT, newline="", encoding="utf-8-sig") as handle:
    rows = list(csv.DictReader(handle))

changes = []
existing_changes = []
if os.path.exists(MANIFEST):
    with open(MANIFEST, newline="", encoding="utf-8-sig") as handle:
        existing_changes = list(csv.DictReader(handle))
for row in rows:
    limit, reason = target_size(row)
    if not limit:
        continue
    texture = unreal.EditorAssetLibrary.load_asset(row["package"])
    if not texture:
        unreal.log_warning("TEXTURE_BUDGET could not load {}".format(row["package"]))
        continue
    old_limit = int(texture.get_editor_property("max_texture_size"))
    effective_old = old_limit if old_limit > 0 else int(row["width"] or 0)
    if effective_old <= limit:
        continue
    texture.set_editor_property("max_texture_size", limit)
    unreal.EditorAssetLibrary.save_loaded_asset(texture, only_if_is_dirty=False)
    changes.append({
        "package": row["package"],
        "width": row["width"],
        "height": row["height"],
        "old_max_texture_size": old_limit,
        "new_max_texture_size": limit,
        "reason": reason,
    })

os.makedirs(os.path.dirname(MANIFEST), exist_ok=True)
all_changes = existing_changes + changes
with open(MANIFEST, "w", newline="", encoding="utf-8-sig") as handle:
    writer = csv.DictWriter(handle, fieldnames=(all_changes[0].keys() if all_changes else ["package"]))
    writer.writeheader()
    writer.writerows(all_changes)

unreal.log("TEXTURE_BUDGET changed={} manifest={}".format(len(changes), MANIFEST))
