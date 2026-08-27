import unreal


MAP = "/Game/HouseOfAtonal/Maps/L_ViewLevel"
unreal.EditorLoadingAndSavingUtils.load_map(MAP)

folder_counts = {"time_01": 0, "time_02": 0, "time_03": 0}
material_uses = []
for actor in unreal.EditorLevelLibrary.get_all_level_actors():
    folder = str(actor.get_folder_path()).replace("\\", "/").lower()
    for key in folder_counts:
        if key in folder.split("/"):
            folder_counts[key] += 1
            unreal.log(f"TIME_CONTENT {key} actor={actor.get_actor_label()} folder={folder}")

    for component in actor.get_components_by_class(unreal.MeshComponent):
        for slot, material in enumerate(component.get_materials()):
            if material and "mi_light_bulb_a" in material.get_path_name().lower():
                material_uses.append((actor, component, slot, material))
                unreal.log(
                    f"TIME_CONTENT LIGHTBULB actor={actor.get_actor_label()} "
                    f"component={component.get_name()} slot={slot} material={material.get_path_name()}"
                )
                try:
                    value = unreal.MaterialEditingLibrary.get_material_instance_scalar_parameter_value(
                        material, "Intensity"
                    )
                    unreal.log(f"TIME_CONTENT LIGHTBULB_INTENSITY_DEFAULT={value}")
                except Exception as exc:
                    unreal.log_warning(f"TIME_CONTENT LIGHTBULB_INTENSITY_CHECK_FAILED={exc}")

unreal.log(f"TIME_CONTENT COUNTS={folder_counts} LIGHTBULB_USES={len(material_uses)}")
