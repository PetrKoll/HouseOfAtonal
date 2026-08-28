import csv
import os

import unreal


MAP = "/Game/HouseOfAtonal/Maps/L_ViewLevel"
unreal.EditorLoadingAndSavingUtils.load_map(MAP)
world = unreal.EditorLevelLibrary.get_editor_world()
rows = []

for actor in unreal.EditorLevelLibrary.get_all_level_actors():
    actor_path = actor.get_path_name()
    for component in actor.get_components_by_class(unreal.MeshComponent):
        asset = None
        for property_name in ("static_mesh", "skeletal_mesh"):
            try:
                asset = component.get_editor_property(property_name)
            except Exception:
                pass
            if asset:
                break
        materials = []
        try:
            materials = [m.get_path_name() for m in component.get_materials() if m]
        except Exception:
            pass
        rows.append({
            "actor": actor_path,
            "actor_class": actor.get_class().get_path_name(),
            "component": component.get_name(),
            "visible": str(component.get_editor_property("visible")),
            "hidden_in_game": str(component.get_editor_property("hidden_in_game")),
            "mesh": asset.get_path_name() if asset else "",
            "materials": " | ".join(materials),
        })

output_dir = os.path.join(unreal.Paths.project_saved_dir(), "TextureAudit")
os.makedirs(output_dir, exist_ok=True)
output_path = os.path.join(output_dir, "view_level_mesh_components.csv")
with open(output_path, "w", newline="", encoding="utf-8-sig") as handle:
    writer = csv.DictWriter(handle, fieldnames=rows[0].keys())
    writer.writeheader()
    writer.writerows(rows)

unreal.log("VIEW_ACTOR_AUDIT actors={} components={} output={}".format(
    len(unreal.EditorLevelLibrary.get_all_level_actors()), len(rows), output_path
))
