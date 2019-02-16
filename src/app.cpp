#include "app.hpp"
#include <chrono>
#include <thread>

#define currentMicroSeconds                                                                                            \
	(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count())

// TODO: The window class needs to be done before this can be added

void App::setup() {
	this->frame = new frameInfo();
	this->app = new appInfo();

	this->init();

	std::thread updater([this]() {
		uint64_t time = currentMicroSeconds;
		uint64_t frameTime = 0;

		while (!frame->shouldClose) {
			frame->time.timePassedUpdate = (double) (currentMicroSeconds - time) / 1000000;
			std::cout << currentMicroSeconds << std::endl;
			this->update();
			frameTime = (1000000 / app->logic.maxUPS) - (currentMicroSeconds - time);
			time = currentMicroSeconds;
			if (frameTime > 0) std::this_thread::sleep_for(std::chrono::microseconds(frameTime));
		}
	});
	uint64_t time = currentMicroSeconds;
	uint64_t frameTime = 0;

	while (updater.joinable()) {
		if (app->window) {
			frame->shouldClose = glfwWindowShouldClose(app->window);
			glfwMakeContextCurrent(app->window);
		}
		frame->time.timePassedRender = (double) (currentMicroSeconds - time) / 1000000;

		this->render();

		frameTime = (1000000 / app->render.maxFPS) - (currentMicroSeconds - time);
		if (frameTime > 0) std::this_thread::sleep_for(std::chrono::microseconds(frameTime));
	}

	updater.join();
	// render.join();

	this->cleanup();
	/*glfwSetCursorPosCallback(app->window, onMouseMoved);
	glfwSetMouseButtonCallback(app->window, onMouseButton);*/

	/*glfwSetWindowPosCallback(app->window, onWindowMoved);
	glfwSetWindowSizeCallback(app->window, onWindowResized);
	glfwSetWindowFocusCallback(app->window, onWindowFocused);
	glfwSetFramebufferSizeCallback(app->window, onFrameBufferResized);*/
}
/*
void App::onWindowMoved(GLFWwindow *window, int x, int y) {
  frame->window.x = x;
  frame->window.y = y;
}


void App::onWindowResized(GLFWwindow *window, int width, int height) {
  frame->window.width = width;
  frame->window.height = height;
}
void App::onWindowFocused(GLFWwindow *window, int focused) {
  frame->window.focused = (bool) focused;
}
void App::onFrameBufferResized(GLFWwindow *window, int width, int height) {
  frame->width = width;
  frame->height = height;
}
*/
/*
void App::onMouseMoved(GLFWwindow *window, double x, double y) {
  frame->mouse.x = x;
  frame->mouse.y = y;
}
void App::onMouseButton(GLFWwindow *window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    frame->mouse.leftButtonDown = action == GLFW_PRESS;
  } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    frame->mouse.rightButtonDown = action == GLFW_PRESS;
  }
}*/