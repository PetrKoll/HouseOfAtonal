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

