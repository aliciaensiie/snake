## Snake Game
# Description
This game is the first that i have done in C++. 
The goal of the game is to have the tallest snake. Your snake die if it eats himself.
You can have the installers for both Linux and Windows in the directory release_packages.

# Installation
First, you need to install SFML version 2.5 (or higher on windows) as well as cmake.
After doing this, you will need to change to the SFML PATH  according to the place you install SFML. You have to change this in CMakeLists.txt (line 6 for linux and line 8 for windows).
To compile and obtain the executable, create a build directory in the game, go in the directory and use this command: 
    cmake ..
    cmake --build .
You can find the executable in the build directory.

# Author
Alicia Plath



