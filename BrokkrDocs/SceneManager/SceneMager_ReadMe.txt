
The SceneManager class is a core class in the Brokkr game engine.
It manages different states of the game and handles the transitions between them.
This allows the engine to easily switch between different modes of gameplay, such as the main menu, game levels, and game over screen.

How to Use:

Initialization
To use the SceneManager class,Engine systems creates an instance of it.

Adding States
To add a new state to the SceneManager, you must create a new class that inherits from the Brokkr::State base class.
This class should contain the logic and data for a specific mode of gameplay. For example,
 you might want to create a MenuState class for the main menu screen or a LevelState class for the game levels.

Once you have created a state class, you can add it to the SceneManager using the AddState method:

sceneManager.AddState("level", std::make_unique<LevelState>());

The first parameter of the AddState method is a unique key that identifies the state.
This key is used to switch between states later.
The second parameter is a unique pointer to an instance of the state class.

Setting the Active State
To switch to a different state, you can use the SetActiveState method:

sceneManager.SetActiveState("menu");

This will immediately switch to the specified state.
If the new state has not yet been initialized, be sure to call its Init method before calling SetActiveState.

Updating and Drawing
Once a state is active, the SceneManager will automatically call the Update and Draw methods of that state on every frame.
You do not need to manually call these methods yourself.

Removing States
To remove a state from the SceneManager, you can use the RemoveState method:

sceneManager.RemoveState("menu");

This will destroy the specified state.
If the state is currently active, the SceneManager will automatically set the next state to active if more are remaining.
and should be noted that the states are not inforcing an order so state maybe random and is advised to only remove non active states

Process State Changes
To handle state changes, you can call the ProcessStateChange method from core class's main loop.
This will handle any pending state changes and switch to the new state if necessary.

