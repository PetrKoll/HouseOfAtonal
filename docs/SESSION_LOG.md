# Session Log

Keep entries short. This is a chronological index; the current truth belongs in
`PROJECT_STATE.md`.

## 2026-07-25 — Shared project memory initialized

- Cloned `PetrKoll/HouseOfAtonal`.
- Found the repository empty except for visual references in `res/`.
- Added cross-computer agent instructions and persistent project-state files.
- Added `.DS_Store` to `.gitignore`.
- Next: commit and push this baseline, then continue Phase 01 preproduction.

## 2026-07-25 — House blockout v01

- Interpreted the supplied courtyard-house presentation board.
- Created a parameter-driven Blender 5.2 blockout with three floors, central
  atrium, concrete envelope, courtyard glazing, street entry and garage.
- Organized geometry into structure, façade, glazing, site and presentation
  collections.
- Generated and visually checked a 1200 × 1200 preview render.
- Recorded the inferred dimensions and regeneration workflow in
  `blender/README.md`.
- Next: review proportions and elevation orientation before adding detailed
  plans, stairs, openings and basement.

## 2026-07-26 — User-authored Blender model added

- Added `blender/house_A.blend` as the active architectural model.
- Added `res/blueprint.png` as its plan reference.
- Marked the earlier generated blockout as an experiment, not the current
  architectural source.

## 2026-07-26 — Cross-computer handoff workflow confirmed

- Verified that the repository can be read, written, pulled and pushed from
  this computer.
- Synchronized two additional architectural reference images under `res/`.
- Recorded standing approval to update handoff documentation, commit and push
  at the end of every meaningful session.
- Next: prepare the Unreal Engine `.gitignore` and Git LFS policy before adding
  the Unreal project.

## 2026-07-26 — Unreal ignore policy prepared

- Expanded `.gitignore` for Unreal Engine, Visual Studio, Rider, Xcode, Blender
  recovery files and common operating-system metadata.
- Verified that generated Unreal directories are ignored while `.uproject`,
  `Config/`, `Content/`, `Source/` and `Build/` remain versionable.
- Confirmed Git LFS 3.7.1 is available on the workstation.
- Next: define the LFS-tracked Unreal asset types, then create the initial
  Unreal project skeleton.

## 2026-07-26 — [FORGE] Agent identity tagging established

- Registered the workstation Codex as Forge with the permanent tag `[FORGE]`.
- Added a shared agent-identity registry and tagging rules for persistent
  project documentation.
- Reserved the laptop Codex identity for a different tag of its own choosing.
- Next: configure Git LFS before creating the Unreal project skeleton.

## 2026-07-26 — [FORGE] Git LFS configured

- Initialized Git LFS for the workstation clone.
- Added LFS rules for Unreal packages and production binary source formats.
- Converted the current Blender revisions to LFS without rewriting history.
- Recorded the per-computer setup and recovery procedure.
- Confirmed that the Unreal skeleton will be created as a C++ project.
- Next: verify the Unreal Engine and C++ toolchain versions, then create the
  initial project skeleton.

## 2026-07-26 — [FORGE] Unreal C++ foundation established

- Found the user-created UE 5.8 VR template and confirmed headset preview works.
- Converted the Blueprint-only project to a compiled `HouseOfAtonal` C++ module.
- Added a Blueprint-accessible experience subsystem for location, weather and
  time-of-day state without coupling it to UI or Ultra Dynamic Sky.
- Reserved `Content/HouseOfAtonal` for project-owned assets.
- Excluded more than 2.2 GB of reinstallable Marketplace content from Git.
- Built `HouseOfAtonalEditor Win64 Development` successfully with UE 5.8.
- Next: restart the editor, create the first project-owned blockout map and ask
  for the detailed menu behavior before UI implementation.

## 2026-07-26 — [FORGE] Control Menu behavior defined

- Distinguished the initial overhead Menu Level from the summoned Control Menu.
- Defined right-controller spawning, circular hover expansion, pinning,
  outside-click closing and ray/direct-touch interaction.
- Defined fixed destination points, blended weather and time changes, and
  transient curated session state.
- Recorded house controls for lights, doors, curtains and later presentation
  states.
- Accepted a particle breakup/reformation transition as the target, subject to
  standalone Quest 3 profiling and a cheaper fallback.
- Next: define the first option sets and arrival points, then build the greybox
  Menu Level and Control Menu interaction prototype.

## 2026-07-26 — [FORGE] Control Menu follow behavior defined

- Chose a delayed relative follow model instead of fixed world-space or
  head-locked placement.
- The menu preserves its summon offset and smoothly follows user translation
  and yaw with tunable damping.
- Next: tune follow lag, maximum separation and comfort in the headset during
  the greybox menu prototype.

## 2026-07-26 — [FORGE] Control Menu C++ greybox foundation

- Added an XR-pawn component that spawns, toggles and dismisses the Control Menu
  along a supplied controller ray.
- Added a world-space menu actor with tunable damped position/yaw following and
  accelerated catch-up beyond a comfortable separation.
- Added a Blueprint widget base with consistent hover, expansion, category
  pinning and outside-click dismissal state.
- Bound widget dismissal to actual menu actor cleanup.
- Built `HouseOfAtonalEditor Win64 Development` successfully with UE 5.8.
- Next: create the Blueprint widget, connect it to `BP_XRPawn`, wire input and
  test the greybox interaction in the headset.

## 2026-07-26 — [FORGE] Control Menu headset integration and option greybox

- Connected the C++ Control Menu component, right-trigger input and existing
  right-hand widget interaction to `BP_XRPawn`.
- Added C++-owned category scaling, pinning, outside-click dismissal and
  pitch/yaw menu following.
- Added a generated three-option greybox for Destination, Weather and Time of
  Day; selections update `UHouseExperienceSubsystem`.
- Removed House State from the menu. Lights, doors and curtains will use direct
  interaction with their environment objects.
- Built `HouseOfAtonalEditor Win64 Development` successfully with UE 5.8.
- Known issue: controller-ray hover exit is still not reliable in the headset
  and requires a deterministic hit-test revision.
- Next: test the generated option strip, stabilize hover, then replace greybox
  labels with approved icons and final presets.

## 2026-07-28 — [FORGE] Control Menu V1 locked

- Discarded the previous WBP-based visual attempt and built a self-contained
  C++ radial menu.
- Added a texture-free filled-circle widget rendered as colored Slate geometry.
- Locked three main circles (`01–03`) and three dedicated contextual groups
  (`11–13`, `21–23`, `31–33`).
- Added reliable main and contextual hover scaling plus click-to-pin behavior.
- Positioned each contextual group on its approved outer arc.
- Added simultaneous retract/expand transitions with group delay, stronger
  easing and a subtle damped spring, avoiding visibility flashes.
- Increased the world-space menu scale and added delayed vertical following.
- Built `HouseOfAtonalEditor Win64 Development` successfully with UE 5.8.
- Next: preserve the locked motion and interaction while replacing numbers
  with icons and connecting menu selections to actual experience actions.

## 2026-07-28 — [FORGE] Menu Level V1 foundation

- Imported the confirmed `house_A.fbx` export as the separately reimportable
  `SM_HeroHouse`.
- Created `L_MenuLevel_V1` and a parameter-driven C++ tabletop city blockout
  with 7 x 7 irregular blocks, streets, sidewalks and central hero placement.
- Added tunable building heights, roof placeholders, trees, cars, future
  vehicle/pedestrian splines and golden-hour/hero-beam lighting controls.
- Added a stationary XR pawn and Menu Level game mode with HMD parallax, no
  locomotion or teleport, and one-time view-relative placement.
- Added a repeatable Unreal editor setup script and built the UE 5.8 editor
  target successfully.
- Next: open the map, review it in the headset and tune scale, framing, density
  and light before replacing any placeholders.

## 2026-07-28 — [FORGE] Editable levels, traffic and destination flow

- Replaced the active generated diorama workflow with
  `L_MenuLevel_Editable`, composed of separately movable roads, parcels,
  buildings, lights, hero house and viewpoint.
- Added editable car and pedestrian spline routes, eight moving cube cars,
  twelve zero-tick parked cube cars and ten moving cylinder pedestrians.
- Added lightweight C++ route followers with per-instance speed, start,
  direction, update interval and spacing controls.
- Added `L_ViewLevel` with the full-scale house, ground and editable Atrium,
  Room 1 and Room 2 arrival points.
- Added a three-circle Menu Level destination selector and retained the locked
  full radial menu for View Level.
- Added right-controller input, visible pointer, hover/click interaction and
  corrected widget trace-channel collision.
- Corrected the native menu actor world scale after it initially spawned at
  several-meter size.
- Added initial View Level right-stick locomotion attempts. Known issue:
  headset testing currently produces forward movement only; reverse and
  lateral movement remain unresolved.
- Confirmed the experience targets high-end PC rendering with Quest as the VR
  display/headset, not standalone Quest compute.
- Built `HouseOfAtonalEditor Win64 Development` successfully with UE 5.8.
- Next: stabilize right-stick locomotion, place final arrival points, refine
  the road/sidewalk network, then establish the PC-quality atmosphere and
  shadow rig before replacing traffic placeholders.

## 2026-07-28 — [FORGE] Manual instanced-tree planter

- Added the Blueprintable `AHouseInstancedMeshPlanter` C++ actor and generated
  the ready-to-place `BP_InstancedTreePlanter` asset.
- Each mesh entry owns manually added local transforms exposed as viewport
  widgets for position, rotation and scale editing.
- The actor rebuilds one HISM component per valid mesh and has no runtime tick;
  shadows, distance-field lighting, indirect lighting, collision and minimum
  LOD remain tunable per mesh group.
- Built the UE 5.8 editor target successfully and created the Blueprint asset
  through a repeatable editor Python helper.
- Preserved the user's uncommitted Menu Level, editor-setting and imported-model
  changes outside this focused commit.
- Next: place the planter in `L_MenuLevel_Editable`, assign real tree meshes and
  verify the manual placement ergonomics before scaling up the vegetation pass.

## 2026-07-28 — [FORGE] Surrounding-house placement handoff

- Added the user's 30 imported surrounding-house Static Mesh assets to the
  project-owned `Content/HouseOfAtonal/models/houses` namespace.
- Saved the current `L_MenuLevel_Editable` composition with the first building
  replacements and retained the user's editor preview settings.
- Confirmed the instanced-tree planter remains Static-Mesh-only; the attempted
  Megaplants Procedural/Skeletal integration was discarded cleanly because a
  different vegetation package with complete Static Mesh trees will be used.
- Next: continue composing the neighborhood with the imported houses, then add
  the replacement static-tree package to `BP_InstancedTreePlanter`.

## 2026-07-31 — [FORGE] House, vehicle and Menu Level sync

- Updated the active `house_A.blend`, added the alternate `house__X.blend` and
  added `cars.blend`; all Blender binaries remain managed through Git LFS.
- Saved the current `L_MenuLevel_Editable` art-direction pass.
- Kept the 352 MB `Content/RayMarchCloud` vendor package local in accordance
  with D-009 rather than consuming shared LFS storage.
- Next: install the same RayMarchCloud dependency on any workstation that must
  open the map with those local references, then continue the atmosphere and
  static vegetation passes.

## 2026-07-31 — [FORGE] Hero house FBX refresh

- Exported all renderable mesh objects in the active View Layer of the saved
  `house_A.blend` to a new combined Unreal exchange FBX.
- Reimported the 288 MB FBX over the existing
  `/Game/HouseOfAtonal/Architecture/SM_HeroHouse`; its package was rebuilt and
  saved without changing its asset path or level references.
- Added repeatable Blender 5.0 export and UE 5.8 automated import scripts.
- Blender's background shutdown emitted an unrelated local Megascans-addon
  unregister warning after the FBX export had completed successfully.
- Next: visually check the refreshed mesh, scale, materials and pivots in both
  Menu Level and View Level before continuing architectural edits.

## 2026-07-31 — [FORGE] Clean hero house reimport

- Re-exported the cleaned 3.4 MB `house_A.blend` and successfully reimported it
  over the existing `SM_HeroHouse` path.
- Reduced `house_A.fbx` from approximately 288 MB to 4.5 MB and the Unreal mesh
  package from approximately 260 MB to 2.7 MB, removing the oversized geometry
  that consumed about 183 MiB of always-resident ray-tracing memory.
- Added the user's `trees.blend` vegetation source to the shared Blender assets.
- Next: reopen Unreal and confirm the house scale, pivot, materials and the
  disappearance of the ray-tracing geometry memory warning.

## 2026-08-01 — [FORGE] Menu Level volumetrics and shadow-quality pass

- Diagnosed the local FogArea failure: the vendor demo includes an enabled
  Exponential Height Fog, while `L_MenuLevel_Editable` only contained the
  FogArea actors and lights.
- Added `ATMOSPHERE_VolumetricFog` with an enabled volumetric system and
  effectively invisible global density, leaving local FogArea volumes in
  artistic control.
- Increased volumetric grid quality for PC-rendered VR and improved the
  shadow-casting lights through lower bias, contact shadows, higher local
  resolution and volumetric shadow casting without changing light transforms,
  colors or intensities.
- Added repeatable configuration and inspection scripts and verified the saved
  level values through UE 5.8.
- Next: inspect the result in the viewport/headset and tune each FogArea actor
  and the authored light composition visually.

## 2026-08-01 — [FORGE] Stereo FogArea compatibility test

- Disabled Mobile Multi-View and Instanced Stereo for the PC-rendered VR path
  after FogArea rendered in only one headset eye.
- Kept Forward Shading and the established atmosphere/shadow-quality settings.
- Next: allow the initial shader compilation to finish, test FogArea in both
  eyes, and retain standard stereo if this confirms vendor-material
  incompatibility with the instanced stereo path.

## 2026-08-01 — [FORGE] Editable Menu Level camera DOF

- Added and placed the standalone `Camera_DOF_Settings` actor in the Menu
  Level; the runtime-spawned XR pawn is intentionally not required for editing.
- Exposed enable, focus distance, aperture, focal region, near/far transition
  and blend weight parameters directly in the actor's Details panel.
- Its unbound Post Process component updates during level editing and applies
  to the XR view at runtime without Blueprint wiring.
- Built `HouseOfAtonalEditor Win64 Development` successfully with UE 5.8 and
  verified that the actor was saved into `L_MenuLevel_Editable`.
- Next: tune DOF gently in-headset and verify comfortable stereo focus at the
  intended diorama viewing distance.

## 2026-08-01 — [FORGE] Deferred Lumen rendering path

- Diagnosed the flat lighting and missing reflections as a renderer mismatch:
  Lumen GI and Lumen Reflections were configured while Forward Shading was
  active, where neither feature is supported.
- Switched the PCVR project to Desktop Deferred Rendering and TSR while
  retaining DX12 SM6, Lumen GI, Lumen Reflections, Virtual Shadow Maps and the
  standard stereo path required by FogArea.
- Next: allow the full shader rebuild to finish, evaluate GI/reflections in the
  viewport and both headset eyes, then tune exposure and material response.

## 2026-08-01 — [FORGE] Traffic car mesh variants

- Imported `blender/cars.fbx` as three separate project-owned Static Meshes:
  `car_A`, `car_b` and `car_C`.
- Replaced all eight current moving traffic cubes with cyclic car variants
  while preserving their route, speed, spacing and intersection behavior.
- Added per-actor `Vehicle Variant` control (0–2), diorama scale/orientation
  correction and enabled dynamic shadow casting for moving and future parked
  vehicles.
- Built `HouseOfAtonalEditor Win64 Development` successfully and verified every
  current moving actor's mesh assignment and shadow state in the saved map.
- Next: visually validate orientation, road height and scale, then assign the
  intended car materials and add parked-car actors where required.

## 2026-08-02 — [FORGE] Traffic orientation and manual-offset fix

- Corrected the eight existing car components that retained the old cube scale
  and rotation after their meshes were replaced.
- Added editable `Vehicle Mesh Rotation` and `Vehicle Mesh Scale` controls for
  per-car visual correction.
- Moving vehicles now preserve the manually placed actor rotation as an offset
  from the spline heading instead of discarding it at runtime.
- Built the UE 5.8 editor target successfully and reapplied all mesh variants,
  corrected transforms and shadow settings to the saved Menu Level.
- Next: validate forward direction on the actual routes; if a source mesh faces
  backward rather than sideways, adjust its exposed rotation by 180 degrees.

## 2026-08-02 — [FORGE] Runtime-instanced traffic manager

- Replaced per-car runtime ticking with one `Traffic_Manager` and three HISM
  components, one for each imported car variant.
- Kept each car as an editor-only authoring object so its world scale, world
  rotation, speed, route, start distance and variant remain individually
  editable.
- At runtime the manager copies those authored values, creates HISM instances,
  removes the preview actors and advances every vehicle from one shared tick.
- The runtime heading is spline tangent plus the authoring actor's saved manual
  rotation offset plus the FBX mesh-axis correction. Scale is copied from the
  actor and is never authored or overwritten by the manager.
- Built successfully and saved `Traffic_Manager` into the Menu Level.
- Next: verify route direction, scale preservation and speed in VR/PIE before
  changing any additional traffic behavior.

## 2026-08-02 — [FORGE] Manager-owned traffic configuration

- Superseded the editor-car authoring approach above: individual `MovingCar`
  actors are no longer the traffic source of truth.
- `Traffic_Manager` now owns an eight-element `Vehicles` array. Every entry
  independently exposes route, start distance, mesh variant, scale, rotation
  offset, speed, reverse direction and spacing.
- At runtime the manager creates only HISM instances and advances them from one
  shared update; no individual car actors or per-car ticks are required.
- Built the UE 5.8 editor target successfully and saved all eight vehicle
  records into `L_MenuLevel_Editable`.
- Next: visually validate the eight manager entries in PIE before changing any
  additional traffic behavior.

## 2026-08-04 — [FORGE] Modular Menu Level aerial Map View foundation

- Backed up the original `L_MenuLevel_Editable` outside the repository with
  its Git revision and SHA-256 checksum before modifying the map.
- Added a smooth forward/return Map View controller with a cloud-covered world
  swap, movable neighborhood and aerial viewpoints, and editor preview modes.
- Added an editable MAP/HOME toggle actor and reusable spatial location-label
  actor, with initial School and City Center labels for both view modes.
- Added independent tagged layers and Outliner anchors for neighborhood,
  aerial-map, transition-cloud and label art direction, plus a replaceable map
  base and house marker blockout.
- Built `HouseOfAtonalEditor Win64 Development` successfully and verified all
  required actors, unique labels and group tags through UE 5.8.
- Next: replace the map blockout with authored geography, add the production
  cloud layer, position labels/viewpoint in the viewport and test MAP/HOME
  activation through the existing VR pointer in-headset.
