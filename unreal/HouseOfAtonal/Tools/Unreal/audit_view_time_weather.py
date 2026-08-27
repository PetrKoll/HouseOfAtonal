import unreal


MAP = "/Game/HouseOfAtonal/Maps/L_ViewLevel"
unreal.EditorLoadingAndSavingUtils.load_map(MAP)


def safe_property(obj, name):
    try:
        return obj.get_editor_property(name)
    except Exception:
        return None


for actor in unreal.EditorLevelLibrary.get_all_level_actors():
    label = actor.get_actor_label()
    class_path = actor.get_class().get_path_name()
    searchable = (label + " " + class_path).lower()

    if "housetimecontroller" in searchable.replace("_", ""):
        unreal.log(f"HOUSE_AUDIT TIME_CONTROLLER label={label} class={class_path}")
        configured_rain = safe_property(actor, "rain_actors") or []
        unreal.log(
            "HOUSE_AUDIT CONFIGURED_RAIN count={} labels={}".format(
                len(configured_rain),
                ",".join(item.get_actor_label() for item in configured_rain if item),
            )
        )
        for state_name in ("time_state01", "time_state02", "time_state03"):
            state = safe_property(actor, state_name)
            if state is None:
                continue
            snapshot = state.get_editor_property("snapshot")
            unreal.log(
                f"HOUSE_AUDIT {state_name} time={state.get_editor_property('time_of_day')} "
                f"snapshot_count={len(snapshot)}"
            )
            for captured in snapshot:
                prop_name = str(captured.get_editor_property("property_name"))
                if any(word in prop_name.lower() for word in ("cloud", "fog", "rain", "weather")):
                    value = captured.get_editor_property("exported_value")
                    unreal.log(f"HOUSE_AUDIT SNAPSHOT {state_name} {prop_name}={value}")

    if any(word in searchable for word in ("rain", "weather", "fog", "ultra_dynamic_sky")):
        unreal.log(
            f"HOUSE_AUDIT ACTOR label={label} class={class_path} path={actor.get_path_name()} "
            f"location={actor.get_actor_location()} scale={actor.get_actor_scale3d()} "
            f"hidden={actor.is_hidden_ed()}"
        )
        for component in actor.get_components_by_class(unreal.ActorComponent):
            unreal.log(
                f"HOUSE_AUDIT COMPONENT {label} {component.get_name()} "
                f"class={component.get_class().get_path_name()} active={component.is_active()}"
            )
        members = sorted(
            member for member in dir(actor)
            if any(word in member.lower() for word in ("cloud", "fog", "rain", "weather", "coverage"))
        )
        for member in members:
            unreal.log(f"HOUSE_AUDIT MEMBER {label} {member}")

unreal.log("HOUSE_AUDIT COMPLETE")
