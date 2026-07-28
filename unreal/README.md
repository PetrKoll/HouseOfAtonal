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

## Menu Level V1

Open:

`Content/HouseOfAtonal/Maps/L_MenuLevel_V1`

The level contains an editable `AHouseMenuLevelBlockout` actor. Its Details
panel exposes the city layout, hero-house scale/rotation, context density and
golden-hour lighting. `AHouseMenuLevelGameMode` uses a stationary XR pawn:
head tracking supplies subtle parallax, but locomotion and teleport are not
bound.

The current hero-house asset is:

`Content/HouseOfAtonal/Architecture/SM_HeroHouse`

After overwriting `blender/house_A.fbx`, use Reimport on this static mesh. To
recreate the complete V1 map and import from scratch, close Unreal and run:

```text
"C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "D:\GIT\HouseOfAtonal\unreal\HouseOfAtonal\HouseOfAtonal.uproject" -ExecutePythonScript="D:\GIT\HouseOfAtonal\Tools\Unreal\setup_menu_level.py" -unattended -nop4
```

## Active editable levels

Author: [FORGE]

The active startup map is now:

`Content/HouseOfAtonal/Maps/L_MenuLevel_Editable`

Its city, roads, parcels, buildings, lights and `MENU_VIEWPOINT_MOVE_ME` are
normal individually editable actors. The older generated `L_MenuLevel_V1`
remains only as an obsolete prototype.

The full-scale environment is:

`Content/HouseOfAtonal/Maps/L_ViewLevel`

Its `ARRIVAL_ATRIUM`, `ARRIVAL_ROOM_1` and `ARRIVAL_ROOM_2` actors define
player placement after selection in the Menu Level. The Menu Level opens a
three-destination selector; View Level opens the full locked Control Menu.

Traffic and pedestrian placeholders are organized under their own Outliner
folders. Each moving car or pedestrian references an editable
`AHouseRouteSpline`. Parked cars have no tick. Current known input issue:
right-stick locomotion in View Level responds only to forward movement in the
headset and must be stabilized next.
