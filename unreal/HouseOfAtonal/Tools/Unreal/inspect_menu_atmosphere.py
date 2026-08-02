import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"

unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)


def read(obj, name):
    try:
        return obj.get_editor_property(name)
    except Exception:
        return "<unavailable>"


for actor in unreal.EditorLevelLibrary.get_all_level_actors():
    class_name = actor.get_class().get_name()
    if class_name == "BP_FogArea_C":
        unreal.log_warning(f"ATMOSPHERE_INSPECT FogArea actor={actor.get_actor_label()}")
    elif isinstance(actor, unreal.ExponentialHeightFog):
        component = actor.get_component_by_class(unreal.ExponentialHeightFogComponent)
        unreal.log_warning(
            "ATMOSPHERE_INSPECT HeightFog "
            f"actor={actor.get_actor_label()} "
            f"density={read(component, 'fog_density')} "
            f"volumetric={read(component, 'enable_volumetric_fog')} "
            f"distance={read(component, 'volumetric_fog_distance')}"
        )
    elif isinstance(actor, unreal.Light):
        component = actor.get_component_by_class(unreal.LightComponent)
        unreal.log_warning(
            "ATMOSPHERE_INSPECT Light "
            f"actor={actor.get_actor_label()} class={class_name} "
            f"mobility={read(component, 'mobility')} intensity={read(component, 'intensity')} "
            f"cast_shadows={read(component, 'cast_shadows')} "
            f"volumetric={read(component, 'volumetric_scattering_intensity')} "
            f"shadow_bias={read(component, 'shadow_bias')} "
            f"slope_bias={read(component, 'shadow_slope_bias')} "
            f"contact={read(component, 'contact_shadow_length')}"
        )
