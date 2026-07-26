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
