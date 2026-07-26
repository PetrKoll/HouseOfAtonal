# Unreal Project

Author: [FORGE]

The Unreal Engine 5.8 C++ project is located at:

`unreal/HouseOfAtonal/HouseOfAtonal.uproject`

## Project-owned content

All new House of Atonal assets belong under:

`Content/HouseOfAtonal`

Do not edit Marketplace assets directly. Create project-owned child assets or
integration Blueprints inside the project namespace instead.

## C++ foundation

`UHouseExperienceSubsystem` is the project-owned state boundary for:

- current experience location
- weather preset
- time-of-day preset

It exposes state and change events to Blueprints. Concrete level loading, menu
behavior and Ultra Dynamic Sky integration remain separate adapters and will be
implemented after their functional behavior is approved.

## Control Menu foundation

The first compiled Control Menu layer contains:

- `UHouseControlMenuComponent`: spawn, toggle and dismiss ownership for the XR
  pawn
- `AHouseControlMenuActor`: world-space widget host with delayed relative
  following
- `UHouseControlMenuWidget`: shared hover, expansion, pinning and outside-click
  state rules for Blueprint visuals

The next editor step is to create a Blueprint widget subclass, add the component
to `BP_XRPawn`, and connect the right-trigger input plus controller pointer and
HMD follow transforms.

## Editor build

From a developer command prompt:

```text
"C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" HouseOfAtonalEditor Win64 Development -Project="D:\GIT\HouseOfAtonal\unreal\HouseOfAtonal\HouseOfAtonal.uproject" -WaitMutex
```

Close and reopen an already-running editor after adding or changing the primary
project module when Live Coding is not being used.
