# 201-group6-escape-room
simple escape room game made in c++

## Audio playback
- Use the in-game command `audio <path/to/file.mp3>` to trigger dialogue or sound effects from any MP3 file.
- On Windows, link with `-lwinmm` when building so MP3 playback works (the project uses the WinMM API).
- On macOS/Linux, the engine will try `afplay` or `ffplay`/`mpg123` respectively; install one of those CLI players if you want sound.

## Dialogue + events
- `Dialogue` objects hold text and an optional MP3 path; they can be attached to Rooms (intros), Areas (descriptions/objects), Doors (locked text), Items (description/tip), or fired via the event system.
- Areas render their description via `Dialogue`, interactable objects use `Dialogue`, and room intros deliver via `Dialogue` so no extra events are needed for narration.
- The `EventSystem` supports simple "when command X is entered in area Y, fire this Dialogue" hooks; see the example registration in `Game.cpp` for an inner monologue on observing Mount Sockmore.
