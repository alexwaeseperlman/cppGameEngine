#include "test.hpp"
#include "shader.hpp"
#include <glad/glad.h>
#include <iostream>

#include "OpenGL/glu.h"

#include <GLFW/glfw3.h>

#include "sprite.hpp"
#include "vertexObject.hpp"

#include <chrono>
#include <thread>

#include <algorithm>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// The window we'll be rendering to
static GLFWwindow *window = NULL;

Shader *whiteShader;
SpriteSheet *testSheet;
// SpriteRenderer *testRenderer;

Quad *sprite;

void onError(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}
int width = 640, height = 480;
double mouseX = 0, mouseY = 0;
static void cursorPos(GLFWwindow *window, double xpos, double ypos) {
	mouseX = xpos;
	mouseY = ypos;
}

bool init() {
	// Initialize GLFW
	if (!glfwInit()) {
		return false;
	} else {
		atexit(glfwTerminate);
		glfwSetErrorCallback(onError);

		// Request an OpenGL 4.1 context (should be core)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, "Title", NULL, NULL);
		glfwGetFramebufferSize(window, &width, &height);

		if (!window) {
			glfwTerminate();
			return false;
		} else {
			glfwMakeContextCurrent(window);
			gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
			glfwSwapInterval(1);
			initLibraries();
			std::cout << "Swap interval: " << gluErrorString(glGetError()) << std::endl;

			int w, h;
			glfwGetWindowSize(window, &w, &h);
			glViewport(0, 0, w, h);
			std::cout << "Viewport: " << gluErrorString(glGetError()) << std::endl;

			whiteShader = Shader::loadShader("assets/white");
			std::cout << "Shader: " << gluErrorString(glGetError()) << std::endl;

			whiteShader->printCompileLog();
			whiteShader->printLog();

			glfwSwapBuffers(window);
			// testSheet = new SpriteSheet("file", 0.25, 0.25);
			// std::cout << "Created Sheet: " << gluErrorString(glGetError()) << std::endl;
			// testRenderer = new SpriteRenderer(testSheet, whiteShader);
			// std::cout << "Created Renderer: " << gluErrorString(glGetError()) << std::endl;
			// sprite = testRenderer->addSprite(0, 0, 10);
			// testRenderer->addSprite(0.1, 0, 10);
			sprite = new Quad(0, 0, 0.5, 0.5);
			std::cout << "Created Sprites: " << gluErrorString(glGetError()) << std::endl << std::endl;

			glfwSetCursorPosCallback(window, cursorPos);
		}
	}
	return true;
}

bool loadAssets() {
	return true;
}

bool mainLoop() { /* Render here */
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	std::cout << "Update frame buffer size: " << gluErrorString(glGetError()) << std::endl;

	sprite->setPosition(mouseX / width * 2, -mouseY / height * 2);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	std::cout << "Clear: " << gluErrorString(glGetError()) << std::endl;

	// testRenderer->display();
	std::cout << "Draw Sprites: " << gluErrorString(glGetError()) << std::endl;

	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	return true;
}

void close() {
	// Destroy window
	glfwDestroyWindow(window);
	window = NULL;

	// Quit GLFW subsystems
	glfwTerminate();
}

int main() {

	// Start up GLFW and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!loadAssets()) {
			printf("FUCK");
		} else {
			while (!glfwWindowShouldClose(window)) {
				mainLoop();
			}
		}
	}

	// Free resources and close GLFW
	close();

	return 0;
}