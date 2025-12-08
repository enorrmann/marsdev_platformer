# Engine Overview — MarsDev Platformer

This document explains the main parts of the 2D platformer engine in this repository and how they interact. It's written for contributors who want to understand or extend the engine.

**Project layout (key files)**
- `src/main.c`: program entrypoint and main game loop.
- `src/player.c` / `inc/player.h`: player entity and `struct pBody` definition used for all dynamic bodies.
- `src/mob.c`, `inc/mob.h`: mob behavior, update and collision logic.
- `src/mobmanager.c`, `inc/mobmanager.h`: spawn point discovery, mob allocation and management.
- `src/map.c`, `inc/map.h`: tilemap and `collisionMap` that encodes level tiles.
- `src/levelgenerator.c`: utilities to generate or load collision data.
- `src/camera.c`, `inc/camera.h`: camera position and viewport logic.
- `src/interactions.c`, `inc/interactions.h`: collision checks like AABB intersections and hit resolution.
- `src/bullet.c`, `inc/bullet.h`: projectile logic and management.
- `res/`: asset definitions and `resources.res`.

High-level flow
- Initialization (in `main`): hardware and libraries (`JOY_init`, `SPR_init`), load level (`loadLevel()`), `playerInit()`, `initSpawnPoints()`, `mobsInit()`, and `setupCamera(...)`.
- Per-frame loop (in `main`): input & player update, spawn mobs if the player is near spawn points, update mobs, update camera, bullets, interactions, then call `SPR_update()` and `SYS_doVBlankProcess()` to present sprites.

Rendering & sprites
- The engine uses the SGDK sprite system. Sprites are created with `SPR_addSprite(...)` (see `mobInit` and `playerInit`), and positioned with `SPR_setPosition(...)`.
- Animations are handled by `SPR_setAnim(...)` and horizontal flipping with `SPR_setHFlip(...)`.
- `SPR_update()` is called each frame at the end of the main loop to submit sprite changes to VDP.
- Common pitfalls: sprites at (0,0) or off-screen will not be visible; ensure `globalPosition` -> `position` conversion accounts for `cameraPosition`.

Input & controls
- Joystick management uses `JOY_init()` and `JOY_setEventHandler(...)`.
- The `inGameJoyEvent` function fills a global `input` struct and forwards changes to `playerInputChanged()`.
- Keep input handling lightweight in the event callback; heavy logic runs in the update functions.

Physics & collisions
- Dynamic objects use `struct pBody` (in `inc/player.h`) for velocities, AABBs, position and state flags.
- Vertical/horizontal motion uses fixed-point (`fix16`) for smooth physics; functions like `fix16ToInt()` and `fix16ToRoundedInt()` convert where necessary.
- Collision detection against the tilemap uses utilities like `posToTile()`, `getTileValue()`, and `getTileBounds()` to form level limits and correct positions (see `mob.c` collision logic).
- AABB intersection checks between entities are in `interactions.c` (e.g., `checkAABBIntersection`). Use these for overlaps and hit detection.

Entities: player, mobs, bullets
- `struct pBody` is reused by the player, mobs and sometimes bullets — keep fields consistent across initializers.
- `playerInit()` configures the player's sprite, AABB, and initial state.
- `mobInit()` creates a sprite for each mob and sets default state: important default: `active = FALSE` and `dead = TRUE` (mobs are inactive until spawned).
- `mobsInit()` allocates `struct pBody` instances for the mob pool and calls `mobInit()` on each.
- Spawning: `initSpawnPoints()` scans `collisionMap` for a special tile value used as spawn markers (in this project `8`). For each marker it records a `SpawnPoint` with a world `position`.
- `spawnMobs()` checks distance from the `playerBody.globalPosition` to each `SpawnPoint` and calls `spawnUnusedMobAt()` to reuse a dead mob from the pool: it sets `dead = FALSE`, `active = TRUE`, and positions the mob's `globalPosition`.
- Bullets use a separate pool and `BULLET_update()` to move, collide and recycle projectiles.

Level & map
- The tile collision map is provided by `src/map.c` (an array `collisionMap[MAP_HEIGHT][MAP_WIDTH]`) or generated at runtime by `levelgenerator`.
- Tile values indicate ground, one-way platforms, spawn markers, etc. The engine checks tile values (e.g., `GROUND_TILE`, `ONE_WAY_PLATFORM_TILE`, or numeric marker `8`) to decide collision behavior and spawn points.

Camera & viewport
- Camera position is tracked in `cameraPosition` and updated via `updateCamera()`.
- Entities convert `globalPosition` to screen `position` by subtracting the camera offset before calling `SPR_setPosition(...)`.

Resources & assets
- Graphics and sound are declared in `res/` and referenced by `resources.h`.
- Palettes and tiles must be loaded/declared correctly so sprites use the right tiles and palette indices.

Build & run
- There is a `Makefile` and `build.sh` in the project root. Use `make` or run `./build.sh` (depending on your setup) to build the ROM.

Debugging tips
- If sprites never show, check that the sprite `position` is computed from `globalPosition - cameraPosition` and that `active` is set true.
- Verify mobs are spawned: `mobInit()` sets `dead = TRUE` by default — ensure `spawnUnusedMobAt()` sets `dead = FALSE` and `active = TRUE` and assigns `globalPosition`.
- Watch for uninitialized pointers when using spawn arrays or object pools. Null-check spawn point pointers before dereferencing.
- Use `VDP_setTextPlane(WINDOW)` and small `debug()` helpers (if present) to draw help text on screen.

Extending the engine
- To add new enemy types, extend `mobInit()` or provide a factory that initializes different properties and animations per type.
- To support streamed levels, replace the static `collisionMap` with a loader that fills the map and re-runs `initSpawnPoints()`.

References (key files)
- `src/main.c`, `src/player.c`, `src/mob.c`, `src/mobmanager.c`, `src/map.c`, `src/interactions.c`, `src/camera.c`, `res/`.

If you want, I can also:
- Add an architecture diagram (PNG/SVG) in `docs/` showing data flows.
- Create a short checklist for debugging spawn/visibility issues.

---
Document created by the development helper.
