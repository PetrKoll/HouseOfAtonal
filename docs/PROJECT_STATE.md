# Project State

Last updated: 2026-07-31
Last updated by: [FORGE]

## Project

**House of Atonal** is a flagship interactive architectural experience for
Meta Quest 3, built in Unreal Engine 5. The intended result is a premium client
presentation and a complete portfolio case study, not a game demo.

## Current phase

Phase 01 — Unreal foundation / early architectural blockout

Status: the UE 5.8 VR template runs with the headset, Control Menu V1 is locked,
and the first editable Menu Level diorama map and C++ blockout are ready for
visual review in Unreal.

## Working principles

- Laptop: planning, documentation, design, asset preparation and case study.
- Workstation: Unreal integration, VR testing, lighting, optimization,
  performance profiling, recording and packaging.
- Design decisions should be completed before expensive Unreal sessions.
- Portfolio documentation is produced continuously, not only at the end.

## Completed

- GitHub repository created and cloned.
- Shared repository-memory system established.
- Cross-computer pull/push workflow verified on both development computers.
- Standing end-of-session documentation, commit and push workflow established.
- Persistent documentation author tags and a cross-computer identity registry
  established.
- Unreal Engine `.gitignore` prepared for generated data, local builds and IDE
  state while keeping project sources and assets versioned.
- Git LFS configured for Unreal packages and production binary source assets.
- The active Blender files are stored through Git LFS without rewriting shared
  history.
- UE 5.8 VR template created and headset preview verified by the user.
- Project converted from Blueprint-only to a compiled C++ project.
- Project-owned `Content/HouseOfAtonal` namespace and initial experience-state
  subsystem established.
- Local Marketplace content separated from version-controlled project assets.
- Menu Level and spatial Control Menu terminology and first functional behavior
  approved and documented.
- Control Menu follow behavior defined as a delayed, damped relative anchor to
  the user rather than fixed world-space or head-locked UI.
- First Control Menu C++ greybox foundation compiled successfully, including
  spawn/toggle ownership, delayed following and shared radial interaction state.
- Initial visual references added under `res/` by the user.
- Created `HouseOfAtonal_blockout_v01.blend`, its repeatable Blender Python
  generator and a verified preview render.
- Added the user's current manual model as `blender/house_A.blend` and its plan
  reference as `res/blueprint.png`.
- Added the confirmed `house_A.blend -> FBX -> Unreal Reimport` workflow and
  imported the current combined house as `SM_HeroHouse`.
- Replaced the generated Menu Level composition with
  `L_MenuLevel_Editable`: roads, parcels, buildings, hero, lights and viewpoint
  are ordinary individually movable level actors organized in Outliner folders.
- Added a stationary XR Menu Level pawn with natural HMD parallax, no
  locomotion in Menu Level and a manually movable `MENU_VIEWPOINT_MOVE_ME`.
- Added individually editable traffic route, moving-car, parked-car and
  pedestrian actors. V1 uses cubes for cars and cylinders for pedestrians.
- Added `L_ViewLevel` with the full-scale reimportable house, ground and three
  manually movable arrival points for Atrium, Room 1 and Room 2.
- Added two Control Menu modes: a three-destination selector in Menu Level and
  the locked full radial menu in View Level.
- Added right-controller widget interaction with a visible pointer and fixed
  `3DWidget` collision-channel handling.
- Added `BP_InstancedTreePlanter`, a manually art-directed vegetation actor.
  Each assigned tree mesh uses one HISM component while its instances retain
  individually editable position, rotation and scale handles in the viewport.
- Imported 30 surrounding-house Static Mesh assets under
  `Content/HouseOfAtonal/models/houses` and began replacing the Menu Level
  context blockout with those buildings.
- Re-exported the current saved `house_A.blend` and reimported it over the
  existing `SM_HeroHouse` asset while preserving all level references. Added
  repeatable Blender export and Unreal reimport scripts for future revisions.
- Re-exported the cleaned house source after removing the accidentally included
  heavy geometry. The exchange FBX dropped from 288 MB to 4.5 MB and the
  rebuilt `SM_HeroHouse` package dropped from 260 MB to 2.7 MB.

## In progress

- Developing the house geometry in `blender/house_A.blend`.
- Art-directing `L_MenuLevel_Editable`, lighting and atmosphere.
- Tuning arrival points and basic navigation in `L_ViewLevel`.
- Replacing blockout context with the user's imported surrounding buildings
  and manually placing instanced vegetation.

## Next actions

1. Continue the manual model in `blender/house_A.blend`.
2. Resolve architectural dimensions and stair geometry from human-scale design
   requirements and the available plan.
3. Add interior partitions, stairs, openings and basement.
4. Add the existing production roadmap to the repository.
5. Write the one-page product definition and complete experience flow.
6. Refine the manually editable Menu Level city, roads, sidewalks and lighting.
7. Place `BP_InstancedTreePlanter`, assign the production tree meshes and
   validate its viewport transform workflow on the current Menu Level.
8. Correct View Level right-thumbstick locomotion. Current headset test reads
   only forward movement reliably; lateral and reverse input are unresolved.
9. Move and rotate `ARRIVAL_ATRIUM`, `ARRIVAL_ROOM_1` and `ARRIVAL_ROOM_2`
   inside the finished house spaces.
10. Define the final weather and time option sets.
11. Replace the locked V1 number labels with final icons without changing its
    approved geometry, hover, pinning or motion.
12. Connect the locked menu selections to destination, weather and time state.
13. Add direct object interaction for lights, doors and curtains when those
    environment objects exist.
14. Test spawn placement, delayed following and category behavior in headset.
15. Record exact versions of Ultra Dynamic Sky and other local dependencies.
16. Connect approved weather and time behavior to Ultra Dynamic Sky.

## Blockers

- [FORGE] View Level right-thumbstick locomotion is incomplete: forward works,
  but reverse and lateral movement do not currently respond reliably in the
  headset.

## Risks to validate early

- PC rendering performance budget for high-end VR; Quest is used as the
  headset/display, not as the standalone compute target.
- Lighting strategy and feasibility of time-of-day changes.
- Vegetation, weather and seasonal variants within the performance budget.
- Scope growth before an MVP is defined.
- Git LFS storage pressure if Marketplace libraries are committed.
- Ultra Dynamic Sky performance and feature parity on standalone Quest 3.
- `Content/RayMarchCloud` is a local 352 MB vendor package and is intentionally
  excluded from Git. Any map references require the package to be installed on
  the other Unreal workstation as well.
- Current architectural dimensions are inferred from a 1536 × 1024 concept
  board and are not suitable for construction-level accuracy.

## Current Blender source

`blender/house_A.blend` remains the active user-authored house model.
`blender/house__X.blend` is the current alternate house working copy and
`blender/cars.blend` contains the new vehicle source work. `blender/trees.blend`
contains the current vegetation source collection. The older
`HouseOfAtonal_blockout_v01.blend` is retained only as an experiment and must
not be treated as the architectural source of truth.

## Handoff note

Before continuing on either computer, pull the latest changes and read this
file together with `docs/DECISIONS.md` and the latest entry in
`docs/SESSION_LOG.md`.
