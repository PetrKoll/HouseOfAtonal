# Project State

Last updated: 2026-07-26
Last updated by: [FORGE]

## Project

**House of Atonal** is a flagship interactive architectural experience for
Meta Quest 3, built in Unreal Engine 5. The intended result is a premium client
presentation and a complete portfolio case study, not a game demo.

## Current phase

Phase 01 — Unreal foundation / early architectural blockout

Status: the UE 5.8 VR template runs with the headset, the project now has a
compiled C++ foundation, and architectural blockout work can begin.

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
- Initial visual references added under `res/` by the user.
- Created `HouseOfAtonal_blockout_v01.blend`, its repeatable Blender Python
  generator and a verified preview render.
- Added the user's current manual model as `blender/house_A.blend` and its plan
  reference as `res/blueprint.png`.

## In progress

- Developing the house geometry in `blender/house_A.blend`.
- Beginning the Unreal architectural and environment blockout.
- Preparing the initial fixed destinations and blockout for the Menu Level,
  exterior, interior and atrium.

## Next actions

1. Continue the manual model in `blender/house_A.blend`.
2. Resolve architectural dimensions and stair geometry from human-scale design
   requirements and the available plan.
3. Add interior partitions, stairs, openings and basement.
4. Add the existing production roadmap to the repository.
5. Write the one-page product definition and complete experience flow.
6. Restart the editor so it loads the newly compiled C++ module.
7. Create the first project-owned blockout map under `Content/HouseOfAtonal`.
8. Define the first three destinations and their arrival points.
9. Define the initial weather, time and house-state option sets.
10. Build a greybox Control Menu interaction prototype.
11. Record exact versions of Ultra Dynamic Sky and other local dependencies.
12. Connect approved weather and time behavior to Ultra Dynamic Sky.

## Blockers

None recorded.

## Risks to validate early

- Standalone Quest 3 performance budget.
- Lighting strategy and feasibility of time-of-day changes.
- Vegetation, weather and seasonal variants within the performance budget.
- Scope growth before an MVP is defined.
- Git LFS storage pressure if Marketplace libraries are committed.
- Ultra Dynamic Sky performance and feature parity on standalone Quest 3.
- Current architectural dimensions are inferred from a 1536 × 1024 concept
  board and are not suitable for construction-level accuracy.

## Current Blender source

`blender/house_A.blend` is the active user-authored house model. The older
`HouseOfAtonal_blockout_v01.blend` is retained only as an experiment and must
not be treated as the architectural source of truth.

## Handoff note

Before continuing on either computer, pull the latest changes and read this
file together with `docs/DECISIONS.md` and the latest entry in
`docs/SESSION_LOG.md`.
