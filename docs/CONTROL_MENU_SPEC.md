# Control Menu Functional Specification

Author: [FORGE]
Status: first approved behavior draft
Last updated: 2026-07-28

## Locked Control Menu V1

The first interaction and motion prototype is approved and locked:

- three main filled circles arranged as a triangle
- main labels `01`, `02` and `03`
- controller hover enlarges exactly the targeted circle
- clicking a main circle pins it open
- each main circle owns a separate group of three contextual circles
- contextual labels are `11–13`, `21–23` and `31–33`
- contextual circles use 75 percent of the main-circle diameter
- each contextual circle enlarges independently on hover
- the top group expands upward; lower groups expand to their outer sides
- contextual groups travel from and return behind their parent circle
- changing category retracts the old group while the new group begins after a
  short delay; visibility is never switched between shared circles
- option motion uses pronounced easing and a subtle damped spring
- the menu follows head movement horizontally and vertically with tunable lag
- the world-space menu is currently scaled by a factor of two

Do not alter this behavior while replacing number labels with final icons or
connecting selections to experience state.

## Implementation status

The first C++ greybox foundation is compiled:

- world-space actor and widget component
- delayed relative position and yaw following
- XR-pawn spawn/toggle component
- shared hover, expand, pin and outside-dismiss state

Still pending in the Unreal Editor:

- Blueprint widget visuals
- connection to `BP_XRPawn`
- right-trigger input binding
- ray and direct-touch event wiring
- headset tuning

## Terminology

### Menu Level

The Menu Level is the experience's initial environment. It presents the house
and surrounding urban context from above, following the visual direction in
the overhead concept images under `res/`.

The user begins every experience in this level and uses the Control Menu to
choose the first destination.

### Control Menu

The Control Menu is the spatial circular interface summoned by the user. It
controls:

- destination
- weather
- time of day

It does not contain a miniature house. Its visual language is composed of
circles, category icons and contextual option icons.

## Session behavior

- Every application run is a one-time curated experience.
- Every run starts from a predefined location, weather, time and house state.
- Selections from a previous run are not persisted.
- The first location is the Menu Level.

## Opening and placement

- The user summons the Control Menu with the index-finger button on the right
  controller. Implementation assumes the right trigger until verified in the
  headset.
- The menu appears along the direction in which the right controller points.
- Distance from the user is a tunable parameter and must be tested in VR.
- The menu faces the user when spawned.
- At spawn time, the menu records its relative horizontal offset from the user.
- As the user moves or turns, the menu follows the corresponding target offset
  with a small damped delay rather than moving instantly.
- The delayed movement should preserve the menu's approximate original
  relative placement while still feeling like a world-space object with weight.
- Position and facing direction interpolate smoothly. HMD pitch and roll do not
  tilt the menu.
- Follow lag, comfortable distance, maximum allowed separation and collision
  behavior are tunable parameters that must be tested in VR.

## Interaction

- Both controller ray/pointer interaction and direct hand/controller touch are
  supported.
- Each main category is represented by a circular icon.
- Pointing at a collapsed category circle enlarges it and reveals its available
  option icons.
- Moving away collapses a category unless it has been pinned.
- Selecting the category's center icon pins that category open.
- A pinned category remains open until the user selects another category or
  clicks outside the menu.
- Selecting another category transfers focus and closes the previously pinned
  category.
- Clicking outside the Control Menu closes the whole menu.
- Selection and hover behavior must be equivalent for ray and direct touch.

## Categories

### Destination

- Initially exposes three fixed destinations.
- The exact destination names and transforms are still to be defined.
- Selecting a destination moves the user to a predefined arrival point.
- Within each destination area, the user can move or teleport between local
  navigation points.

### Weather

- Uses presets rather than an unrestricted continuous control in the first
  version.
- Transitions blend over time rather than switching instantly.
- Weather affects the full environment, including:
  - sky and clouds
  - rain or snow emitters
  - wind and vegetation response
  - wetness or snow material state
  - suitable ambient audio
- Ultra Dynamic Sky is the intended sky and weather foundation.

### Time of day

- The first version exposes predefined time presets.
- The underlying system must retain the ability to accept an adjustable time
  value later.
- Time transitions blend rather than switching instantly.

### Object interaction

House elements such as lights, doors and curtains are not controlled through
the Control Menu. The user interacts with those objects directly in the
environment.

## World organization

- Menu Level, exterior, interior and atrium begin inside one Unreal level while
  the experience and visual transitions are developed.
- The user moves between fixed arrival points in that level.
- After behavior and visuals are stable, the world may be split using level
  streaming, data layers or separate levels so invisible content does not
  consume unnecessary Quest resources.
- The split must preserve the Control Menu's public behavior.

## Destination transition

The target visual is a transformation in which the current world breaks into
particles and reforms into the destination state.

Implementation should be staged:

1. establish reliable destination state changes and player relocation
2. prototype a masked dissolve/fade transition
3. add particle breakup and reformation
4. profile on standalone Quest 3
5. retain a cheaper fallback if the full effect exceeds the performance budget

The transition must hide relocation and streaming details from the user.

## State architecture

`UHouseExperienceSubsystem` owns the selected destination, weather and time
state. UI, world transition, Ultra Dynamic Sky and house-state systems react
through separate adapters.

The subsystem is reset to the curated defaults at the beginning of every run.

## Parameters requiring headset testing

- spawn distance
- positional and rotational follow lag
- maximum separation before accelerated catch-up
- menu angular and physical size
- hover expansion amount
- hover dwell and collapse delay
- icon spacing and selection target size
- direct-touch activation depth
- blend duration for time and weather
- transition duration and comfort

## Still required before UI implementation

- the three initial destination names and arrival points
- the initial weather presets
- the initial time presets
- final icon set and labels
- confirmation of right-trigger input
- collision and occlusion behavior while the menu follows the user
