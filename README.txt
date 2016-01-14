##C++ Lucid - 2D Game Engine + Game w/ 3D lighting

### Notes

- Built using MSVC 140
- Pre-compiled libraries added, but building 3rd party libraries yourself is advised.

### Lucid

Lucid is the combined effort of creating a game by creating a custom organized and well-structured 
game engine first and building a game on top of it. Lucid is a game about dynamically switching
physics state between specific `colored` block based on whatever colored light the player activates
throughout its lantern. The player is required to influence the correct physics collision states at 
the exact right times to build a path for moving forward. Requires memory, quick reflexes, and the 
will to keep moving forward.

Lucid is an attempt to experiment with different approaches of designing and building a larger game/game
-engine. This includes seperating render state from game logic and seperating physics state from both 
the game logic and render logic. Lucid features an entity component system for defining the game logic
and uses an event queue system for sub-system communication. The goal of the system was to decouple
irrelevant components and finding a strong balance between well-experienced design patterns and the 
system's requirements.

For more information regarding the project: see [joeydevries.com](http://joeydevries.com).