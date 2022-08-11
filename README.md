# TankClicker

Win32/C++ example application where you drive your tank around in a world full of enemy tanks. You can click on screen to set a movement target for your tank. Enemy tanks near you will start tracking you with their gun. Performance is not that great, can you find out why?

# Classes

### TankClicker

This is the main application with application initialization and shutdown, Windows message handling and main loop.

### Game

Game state (player and enemy positions) and tick function to update current state

### View

Responsible for drawing the game view to a window

### AI Worker

Worker thread that calculates the enemy gun angles asynchronously

### Settings 

Loads parameters (key/value pairs) from a simple text file. Used to store enemy count, movement speed among other things.


