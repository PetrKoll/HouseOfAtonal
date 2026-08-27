import unreal


MAPS = (
    "/Game/EasyRain/Levels/L_EasyRain_ExampleDemo",
    "/Game/EasyRain/Levels/L_EasyRain_Showcase_Demo",
    "/Game/HouseOfAtonal/Maps/L_ViewLevel",
)


def safe_property(obj, name):
    try:
        return obj.get_editor_property(name)
    except Exception as exc:
        return f"<unavailable:{exc}>"


for map_path in MAPS:
    unreal.EditorLoadingAndSavingUtils.load_map(map_path)
    unreal.log(f"EASY_RAIN_AUDIT MAP={map_path}")
    for actor in unreal.EditorLevelLibrary.get_all_level_actors():
        if "/easyrain/blueprints/bp_easyrain" not in actor.get_class().get_path_name().lower():
            continue
        unreal.log(
            f"EASY_RAIN_AUDIT ACTOR={actor.get_actor_label()} "
            f"location={actor.get_actor_location()} scale={actor.get_actor_scale3d()}"
        )
        for component in actor.get_components_by_class(unreal.NiagaraComponent):
            system = safe_property(component, "asset")
            system_path = system.get_path_name() if hasattr(system, "get_path_name") else system
            unreal.log(
                f"EASY_RAIN_AUDIT NIAGARA={component.get_name()} asset={system_path} "
                f"relative_location={safe_property(component, 'relative_location')} "
                f"relative_scale={safe_property(component, 'relative_scale3d')} "
                f"visible={safe_property(component, 'visible')} "
                f"auto_activate={safe_property(component, 'auto_activate')} "
                f"allow_scalability={safe_property(component, 'allow_scalability')} "
                f"force_solo={safe_property(component, 'force_solo')}"
            )

unreal.log("EASY_RAIN_AUDIT COMPLETE")
