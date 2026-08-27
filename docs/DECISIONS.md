# Decision Log

Record decisions that should survive across computers and chat sessions.
Do not rewrite history: if a decision changes, add a new entry that supersedes
the previous one.

## D-001 — Repository files are the shared project memory

- Date: 2026-07-25
- Status: accepted
- Decision: Use `PROJECT_STATE.md`, `DECISIONS.md`, `SESSION_LOG.md` and Git
  history as the persistent handoff between computers and Codex sessions.
- Reason: Chat context is not guaranteed to be available in a different session
  or on a different computer.
- Consequence: Every meaningful session must leave the project state current.

## D-002 — Separate laptop and workstation responsibilities

- Date: 2026-07-25
- Status: accepted
- Decision: Planning, design and asset preparation happen primarily on the
  laptop. Unreal integration, VR testing, lighting, optimization, recording and
  packaging happen on the workstation.
- Reason: Desktop time is the most expensive production resource.
- Consequence: Unreal sessions should begin with prepared inputs and defined
  decisions.

## D-003 — Start the house as a parametric proportion blockout

- Date: 2026-07-25
- Status: provisional
- Decision: Build v01 from editable metre-based primitives and keep the
  generating Blender Python script in Git.
- Reason: The presentation board communicates spatial intent but its drawings
  are not sufficiently dimensioned for an accurate final model.
- Consequence: v01 is suitable for proportion review only. Dimensions,
  partitions, openings and circulation must be revised when better plans or
  explicit design decisions are available.

## D-004 — End every meaningful session with a Git handoff

- Date: 2026-07-26
- Status: accepted
- Decision: At the end of every meaningful session, update the shared project
  memory, commit the intended changes and push them to GitHub.
- Reason: The project is developed on two computers and each new session must
  be able to continue from the repository without relying on chat history.
- Consequence: Start sessions with a clean-state check and pull. End sessions
  with current handoff notes, a focused commit, a successful push and a report
  of any work that remains uncommitted.

## D-005 — Version Unreal sources, not generated machine data

- Date: 2026-07-26
- Status: accepted
- Decision: Keep the Unreal project descriptor, `Config/`, `Content/`,
  `Source/`, `Build/` and required project plugins in Git. Ignore generated
  `Binaries/`, `DerivedDataCache/`, `Intermediate/`, `Saved/`, IDE state,
  profiling data and local packaging output.
- Reason: The workstation will build and run Unreal while the laptop must still
  receive the durable project sources, assets and configuration without large
  machine-specific caches.
- Consequence: Generated data is rebuilt locally. Required binary-only plugin
  files must be reviewed explicitly before adding such a plugin.

## D-006 — Tag persistent documentation by Codex identity

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: Each computer's Codex uses a unique permanent tag registered in
  `docs/AGENT_IDENTITIES.md`. Workstation Codex uses `[FORGE]`; laptop Codex
  chooses and registers a different tag.
- Reason: The shared repository must show which collaborator made each
  documentation entry when work moves between two computers and chat sessions.
- Consequence: All future session-log entries, decisions, project-state updates
  and other persistent authored notes carry the responsible Codex tag.

## D-007 — Store production binary assets with Git LFS

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: Track Unreal packages, Blender sources, interchange assets,
  high-resolution source imagery, production audio and video through Git LFS
  according to `.gitattributes`.
- Reason: These binary files do not benefit from normal Git diffs and would
  rapidly increase clone size when revised during production.
- Consequence: Git LFS is a prerequisite on both computers. Existing Blender
  history is not rewritten; current and future Blender revisions use LFS.

## D-008 — Start the Unreal project as C++

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: Create the initial Unreal project as a C++ project rather than a
  Blueprint-only project.
- Reason: The user expects to build custom C++ classes collaboratively from the
  beginning.
- Consequence: The workstation needs the supported C++ toolchain configured
  before the project skeleton is generated and validated.

## D-009 — Keep Marketplace packages local

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: Keep Ultra Dynamic Sky, Megaplant Library and Fog Area out of Git.
  Store all project-owned assets under `Content/HouseOfAtonal` and integrate
  vendor content through child assets or adapters.
- Reason: The local vendor content currently exceeds 2.2 GB and would consume
  shared LFS storage while remaining reinstallable from its original source.
- Consequence: A replacement Unreal workstation must reinstall the documented
  dependencies. Vendor assets must not be modified directly.

## D-010 — Centralize experience state without vendor coupling

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: Use a C++ game-instance subsystem as the stable state boundary for
  location, weather and time-of-day selections. Keep level loading, UI and
  Ultra Dynamic Sky control in separate integrations.
- Reason: Menu behavior and vendor APIs will evolve independently from the
  product's core experience state.
- Consequence: Blueprints request changes through the subsystem and listen to
  its events. The subsystem itself does not directly reference vendor assets.

## D-011 — Require menu behavior specification before UI implementation

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: Ask the user for the detailed functional behavior of the VR menu
  immediately before implementing it.
- Reason: The visual references communicate style and high-level controls but
  do not define interaction, transitions, persistence or failure behavior.
- Consequence: Architectural blockout and neutral systems may proceed first;
  UI implementation waits for the requested specification.

## D-012 — Separate Menu Level from Control Menu terminology

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: `Menu Level` means the initial overhead house-and-context
  environment. `Control Menu` means the user-summoned circular spatial
  interface for destination, weather, time and house controls.
- Reason: Both concepts were previously called menu despite having different
  runtime and design responsibilities.
- Consequence: Code, assets and documentation use the two explicit terms.

## D-013 — Use a transient radial spatial Control Menu

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: Spawn the Control Menu along the right-controller pointing
  direction. Category circles expand on hover, can be pinned by their center
  icon, support ray and direct touch, and close on an outside click.
- Reason: This matches the approved circular interface concept and keeps the
  controls accessible from any experience location.
- Consequence: Menu placement and dimensions remain tunable VR parameters.
  Detailed behavior is recorded in `docs/CONTROL_MENU_SPEC.md`.

## D-014 — Start in one level and split after validation

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: Build Menu Level, exterior, interior and atrium in one Unreal level
  during early development, with fixed arrival points for destination changes.
  Split or stream content only after behavior and visuals are stable.
- Reason: One level makes early iteration and transition prototyping faster.
- Consequence: Systems must avoid hard dependencies that prevent later
  streaming or level separation.

## D-015 — Reset every experience to curated defaults

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: Do not persist location, weather, time or house state between
  application runs.
- Reason: The product is a one-time curated presentation experience.
- Consequence: Every run begins in the Menu Level with predefined state.

## D-016 — Blend environment changes and target a particle world transition

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: Blend time and weather changes. Target a destination transition in
  which the world breaks into particles and reforms, with a cheaper dissolve or
  fade fallback if required on standalone Quest 3.
- Reason: Smooth environmental change and a transformative move between places
  are central parts of the presentation concept.
- Consequence: Transition development is staged and profiled before the full
  effect becomes a production dependency.

## D-017 — Use delayed relative following for the Control Menu

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: After spawning, the Control Menu preserves its approximate relative
  offset from the user and follows translation and yaw with a small damped
  delay. It is neither fixed permanently in world space nor rigidly head-locked.
- Reason: The menu should remain accessible as the user moves while retaining
  spatial weight and the placement established when it was summoned.
- Consequence: Position lag, rotation lag, maximum separation and catch-up
  behavior are exposed as VR-tunable parameters. HMD pitch and roll do not tilt
  the menu.

## D-018 — Keep house controls out of the Control Menu

- Date: 2026-07-26
- Author: [FORGE]
- Status: accepted
- Decision: Lights, doors, curtains and similar house elements are controlled
  by direct interaction with objects in the environment, not by a House State
  category in the Control Menu.
- Reason: These actions belong spatially to the objects they affect.
- Consequence: The Control Menu contains Destination, Weather and Time of Day.

## D-019 — Lock the Control Menu V1 interaction and motion

- Date: 2026-07-28
- Author: [FORGE]
- Status: accepted
- Decision: Lock the approved three-circle radial layout, dedicated contextual
  groups, exact hover scaling, click-to-pin state, outward group placement,
  delayed cross-group transition, pronounced easing and subtle spring motion.
- Reason: The behavior has been validated in the headset and is the stable
  foundation for subsequent visual and functional work.
- Consequence: Later icon and action integration must preserve this geometry,
  interaction state and animation behavior unless a new decision supersedes it.

## D-020 — Build Menu Level as an editable tabletop diorama

- Date: 2026-07-28
- Author: [FORGE]
- Status: accepted
- Decision: Build Menu Level as a parameter-driven 7 x 7 tabletop city around
  a separately reimportable three-floor hero house. Use an oblique fixed XR
  viewpoint with head-motion parallax, no locomotion or teleport, a dark
  ground/city falloff and diagonal golden-hour hero lighting.
- Reason: The initial experience must present the house as the dominant new
  intervention within a believable existing neighborhood while remaining easy
  to art-direct and update.
- Consequence: Surrounding buildings are normally lower than or equal to the
  hero, with at most one or two taller accents. Roads, sidewalks, placeholder
  vegetation/cars and route splines remain independently tunable for later
  Megascans, traffic and pedestrian passes.

## D-021 — Keep the hero house reimportable from Blender

- Date: 2026-07-28
- Author: [FORGE]
- Status: accepted
- Decision: Use `blender/house_A.blend -> blender/house_A.fbx ->
  SM_HeroHouse` as the current exchange path, with the Unreal static mesh kept
  separate from the generated Menu Level city.
- Reason: The house is still being developed and must be replaceable without
  rebuilding or baking the surrounding diorama.
- Consequence: Future house exports overwrite `house_A.fbx` and use Unreal
  Reimport on `SM_HeroHouse`; the city generator and level composition remain
  intact.

## D-022 — Supersede the generated Menu Level with editable actors

- Date: 2026-07-28
- Author: [FORGE]
- Status: accepted; supersedes the editing model in D-020
- Decision: Use `L_MenuLevel_Editable` as the active Menu Level. Roads,
  parcels, buildings, lighting, hero house and viewpoint are ordinary
  individually movable level actors rather than runtime-generated HISM content.
- Reason: Direct viewport art direction is more important than procedural
  generation for this presentation scene.
- Consequence: Nothing regenerates automatically at runtime. Optimization and
  instancing happen only after the composition is approved.

## D-023 — Separate Menu Level and View Level control modes

- Date: 2026-07-28
- Author: [FORGE]
- Status: accepted
- Decision: Menu Level uses a three-circle selector for Atrium, Room 1 and
  Room 2. Selecting one loads `L_ViewLevel` and places the player at its
  matching editable arrival point. View Level uses the locked full Control
  Menu.
- Reason: The initial diorama is a destination selector, while the full
  environment needs destination, weather and time controls.
- Consequence: The experience state persists the selected destination across
  map loading. Each View destination is represented by a movable
  `AHouseViewArrivalPoint`.

## D-024 — Treat Quest as a PC-driven VR display target

- Date: 2026-07-28
- Author: [FORGE]
- Status: accepted
- Decision: Target a high-end PC-rendered VR experience streamed/displayed
  through Quest rather than standalone Quest rendering.
- Reason: Visual quality is the priority and compute remains on the workstation.
- Consequence: Lighting, atmosphere, characters and effects are designed for
  desktop VR performance; optimization remains important but is not constrained
  to the standalone mobile renderer.

## D-025 — Use manually placed HISM groups for repeated vegetation

- Date: 2026-07-28
- Author: [FORGE]
- Status: accepted
- Decision: Use one editable planting actor containing mesh groups and manual
  instance transforms. Render each mesh group through one Hierarchical
  Instanced Static Mesh component.
- Reason: Vegetation placement must remain directly art-directable in the
  viewport while repeated trees should not incur one actor and draw setup per
  copy.
- Consequence: The designer adds transforms under the chosen mesh, then moves,
  rotates and scales their viewport widgets. Procedural scattering is not part
  of this tool.

## D-026 — Prioritize PC-quality local atmosphere in the Menu Level

- Date: 2026-08-01
- Author: [FORGE]
- Status: accepted
- Decision: Drive local FogArea volumes with one nearly invisible global
  Exponential Height Fog and allocate increased desktop GPU quality to
  volumetric resolution and Virtual Shadow Maps.
- Reason: Quest is only the display target, the Menu Level has little runtime
  activity, and visual atmosphere is the current priority.
- Consequence: FogArea remains locally art-directable in the level, while the
  project carries a higher volumetric and shadow cost than default settings.

## D-027 — Use Desktop Deferred with Lumen for PCVR presentation

- Date: 2026-08-01
- Author: [FORGE]
- Status: accepted
- Decision: Use Desktop Deferred Rendering, TSR, Lumen GI, Lumen Reflections,
  DX12 SM6 and Virtual Shadow Maps for the PC-rendered experience.
- Reason: Forward Shading cannot provide Lumen bounce or reflections, and the
  project's priority is high-end visual depth rather than standalone Quest
  rendering performance.
- Consequence: Rendering both headset eyes is substantially more expensive and
  must be profiled, but indirect illumination and reflection features are now
  available on their supported rendering path.

## D-028 — Author cars individually, render and move them centrally

- Date: 2026-08-02
- Author: [FORGE]
- Status: accepted
- Decision: Keep individual car actors only as designer-editable authoring
  records. Convert them at BeginPlay into three HISM groups managed by one
  shared traffic manager.
- Reason: The designer needs per-car transform, speed, route and variant
  control without paying for individual runtime actors and ticks.
- Consequence: Editor transforms are the source of truth; runtime instances
  preserve authored scale and rotation offsets while following spline paths.

## D-029 — Keep Menu Level aerial Map View layers directly art-directable

- Date: 2026-08-04
- Author: [FORGE]
- Status: accepted
- Decision: Build the neighborhood, aerial map, transition clouds, local
  labels and aerial labels as independent tagged actors with movable
  viewpoints and editor preview modes. Use cloud cover to conceal the world
  representation swap during a smooth vertical transition.
- Reason: The designer must be able to switch, position and replace every
  visual layer directly in the viewport instead of editing one opaque
  generated Blueprint.
- Consequence: `BP_HouseMapViewController` coordinates visibility and player
  movement, while the actual visual content remains ordinary level actors.

## D-030 — Keep View Level time transitions narrow and weather project-owned

- Date: 2026-08-27
- Author: [FORGE]
- Status: accepted
- Decision: The three Control Menu time options map directly to the three
  authored `HouseTimeController` states and interpolate only UDS Time of Day.
  View weather is Sunny, Rain and Fog; the same project controller smoothly
  drives UDS cloud/fog values and toggles the existing fixed EasyRain actors.
- Reason: Interpolating all 599 captured UDS properties through reflection each
  frame caused unacceptable VR hitches. Fixed rain coverage around the authored
  house also avoids a visibly player-following emitter.
- Consequence: Full UDS snapshots remain useful for editor capture/preview but
  are not replayed during runtime time changes. Weather values stay editable on
  the placed controller for visual tuning.
