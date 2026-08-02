import unreal


MAP_PATH = "/Game/HouseOfAtonal/Maps/L_MenuLevel_Editable"


def set_property(obj, name, value):
    try:
        obj.set_editor_property(name, value)
        unreal.log(f"MENU_ATMOSPHERE set {name}={value}")
        return True
    except Exception as error:
        unreal.log_warning(f"MENU_ATMOSPHERE could not set {name}: {error}")
        return False


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
actors = unreal.EditorLevelLibrary.get_all_level_actors()

height_fogs = [actor for actor in actors if isinstance(actor, unreal.ExponentialHeightFog)]
if height_fogs:
    height_fog = height_fogs[0]
else:
    height_fog = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.ExponentialHeightFog,
        unreal.Vector(0.0, 0.0, 0.0),
    )

height_fog.set_actor_label("ATMOSPHERE_VolumetricFog")
fog_component = height_fog.get_component_by_class(unreal.ExponentialHeightFogComponent)

# FogArea needs the volumetric froxel system to be active. Keep the global fog
# practically invisible so the authored local FogArea volumes remain in control.
set_property(fog_component, "fog_density", 0.0001)
set_property(fog_component, "enable_volumetric_fog", True)
set_property(fog_component, "volumetric_fog_start_distance", 0.0)
set_property(fog_component, "volumetric_fog_distance", 20000.0)
set_property(fog_component, "volumetric_fog_albedo", unreal.Color(255, 255, 255, 255))
set_property(fog_component, "volumetric_fog_extinction_scale", 1.0)


for actor in actors:
    if not isinstance(actor, unreal.Light):
        continue

    component = actor.get_component_by_class(unreal.LightComponent)
    label = actor.get_actor_label()

    try:
        casts_shadows = component.get_editor_property("cast_shadows")
    except Exception:
        casts_shadows = False

    # The fill light is intentionally shadowless. Preserve the authored light
    # intensities, colors and transforms; only improve shadow fidelity.
    if not casts_shadows:
        continue

    set_property(component, "shadow_bias", 0.25)
    set_property(component, "shadow_slope_bias", 0.25)
    set_property(component, "contact_shadow_length", 0.03)
    set_property(component, "shadow_resolution_scale", 2.0 if "Hero" in label else 1.5)
    set_property(component, "cast_volumetric_shadow", True)


if not unreal.EditorLevelLibrary.save_current_level():
    raise RuntimeError(f"Failed to save {MAP_PATH}")

unreal.log_warning(
    "MENU_ATMOSPHERE_DONE Added the volumetric fog driver and quality-focused shadow settings."
)
