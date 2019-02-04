#include "test.hpp"
#include "shader.hpp"
#include <glad/glad.h>
#include <iostream>

#ifdef __linux__
#include <GL/glu.h>
#else
#include <OpenGL/glu.h>
#endif

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
SpriteRenderer *testRenderer;

Sprite *sprite;

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

		// Request an OpenGL 3.3 context (should be core)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			int w, h;
			glfwGetWindowSize(window, &w, &h);
			glViewport(0, 0, w, h);
			std::cout << "Viewport: " << gluErrorString(glGetError()) << std::endl;

			whiteShader = Shader::loadShader("assets/white");
			std::cout << "Shader: " << gluErrorString(glGetError()) << std::endl;

			whiteShader->printCompileLog();
			whiteShader->printLog();

			glfwSwapBuffers(window);
			testSheet = new SpriteSheet("assets/animation.spritesheet.png");
			testSheet->loadFromJSON("assets/animation.spritesheet.json");
			std::cout << "Created Sheet: " << gluErrorString(glGetError()) << std::endl;
			testRenderer = new SpriteRenderer(testSheet, 10000);
			std::cout << "Created Renderer: " << gluErrorString(glGetError()) << std::endl;

			Sprite *test = testRenderer->addSprite(-0.25, 0, 0);
			sprite = testRenderer->addSprite(0, 0, 1);
			test->setSize(1, 1);
			test->setPosition(-0.2, 0);
			std::cout << "Created Sprite: " << gluErrorString(glGetError()) << std::endl;

			sprite->setPosition(0, 0);
			sprite->setSize(1, 1);

			std::cout << "Set Sprite Information: " << gluErrorString(glGetError()) << std::endl << std::endl;

			glfwSetCursorPosCallback(window, cursorPos);
		}
	}
	return true;
}

bool loadAssets() {
	return true;
}

bool mainLoop() { /* Render here */
	// std::cout << "\x1b[A\x1b[A\x1b[A\x1b[A\x1b[A\x1b[A";

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	std::cout << "Update frame buffer size: " << gluErrorString(glGetError()) << std::endl;

	sprite->setPosition(mouseX / width * 2 - 1, -mouseY / height * 2);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	std::cout << "Clear: " << gluErrorString(glGetError()) << std::endl;

	sprite->setTextureID((sprite->getTextureID() + 1) % 10);

	testRenderer->display();
	std::cout << "Draw Sprites: " << gluErrorString(glGetError()) << std::endl;

	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();

	std::this_thread::sleep_for(std::chrono::milliseconds(15));

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