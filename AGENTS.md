# House of Atonal — Agent Instructions

This repository is used from multiple computers and multiple Codex chats.
Repository files are the shared project memory. Do not rely on chat history as
the only source of context.

## Start of every work session

1. Run `git status` and do not overwrite uncommitted user work.
2. Pull the latest committed changes before starting when the worktree is clean.
3. Read, in this order:
   - `docs/PROJECT_STATE.md`
   - `docs/DECISIONS.md`
   - `docs/SESSION_LOG.md` (latest entry)
4. Inspect the files relevant to the current task.
5. Confirm that the requested work does not conflict with an existing decision.

## During work

- Keep changes focused and preserve unrelated work.
- Record durable product or technical decisions in `docs/DECISIONS.md`.
- Keep `docs/PROJECT_STATE.md` concise and current. It is a handoff, not a diary.
- Never store passwords, access tokens, API keys, private client data, or other
  secrets in project-memory files.

## End of every meaningful work session

1. Update `docs/PROJECT_STATE.md`:
   - current phase and status
   - what changed
   - next concrete actions
   - blockers or risks
2. Append a short dated entry to `docs/SESSION_LOG.md`.
3. Update `docs/DECISIONS.md` if a durable decision was made.
4. Verify the changed files and report any uncommitted work.
5. Update the handoff documentation, commit all intended session changes and
   push them at the end of every meaningful session. The user granted standing
   approval for this cross-computer synchronization workflow on 2026-07-26.

## Source-of-truth rules

- Current status and handoff: `docs/PROJECT_STATE.md`
- Durable decisions and their reasoning: `docs/DECISIONS.md`
- Chronological work history: `docs/SESSION_LOG.md`
- Git history: exact implementation changes
- If chat history conflicts with committed repository documentation, point out
  the conflict and ask before changing a durable decision.

