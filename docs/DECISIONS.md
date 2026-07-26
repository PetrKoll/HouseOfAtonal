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
