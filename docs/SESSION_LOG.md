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
