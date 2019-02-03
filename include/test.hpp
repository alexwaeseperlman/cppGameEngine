#pragma once

// Starts up SDL and creates window
bool init();

// Load assets at the beginning
bool loadAssets();

// Do event polling
bool pollEvents();

bool mainLoop();

// Frees media and shuts down SDL
void close();
