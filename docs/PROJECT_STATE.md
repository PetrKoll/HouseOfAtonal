# Project State

Last updated: 2026-07-25

## Project

**House of Atonal** is a flagship interactive architectural experience for
Meta Quest 3, built in Unreal Engine 5. The intended result is a premium client
presentation and a complete portfolio case study, not a game demo.

## Current phase

Phase 01 — Preproduction / early architectural blockout

Status: the user is building the architectural model manually in Blender.

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
- Initial visual references added under `res/` by the user.
- Created `HouseOfAtonal_blockout_v01.blend`, its repeatable Blender Python
  generator and a verified preview render.
- Added the user's current manual model as `blender/house_A.blend` and its plan
  reference as `res/blueprint.png`.

## In progress

- Developing the house geometry in `blender/house_A.blend`.
- Defining the product, experience flow and art direction.

## Next actions

1. Continue the manual model in `blender/house_A.blend`.
2. Resolve architectural dimensions and stair geometry from human-scale design
   requirements and the available plan.
3. Add interior partitions, stairs, openings and basement.
4. Add the existing production roadmap to the repository.
5. Write the one-page product definition and complete experience flow.
6. Prepare the Unreal Engine `.gitignore` and Git LFS policy before adding the
   Unreal project.

## Blockers

None recorded.

## Risks to validate early

- Standalone Quest 3 performance budget.
- Lighting strategy and feasibility of time-of-day changes.
- Vegetation, weather and seasonal variants within the performance budget.
- Scope growth before an MVP is defined.
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
