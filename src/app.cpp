#include "app.hpp"
#include <chrono>
#include <thread>

#define currentNanoSeconds                                                                                             \
	(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count())

void App::callUpdate() {
	uint64_t time = currentNanoSeconds;
	uint64_t frameTime = 0;

	while (!this->frame.shouldClose) {
		this->frame.shouldClose = glfwWindowShouldClose(this->app.window);
		glfwPollEvents();
		this->frame.time.timePassedUpdate = (double) (currentNanoSeconds - time) / 1000000;
		this->update();
		frameTime = (1000000 / this->app.logic.maxUPS) - (currentNanoSeconds - time);
		time = currentNanoSeconds;
		if (frameTime > 0) std::this_thread::sleep_for(std::chrono::nanoseconds(1000000 - frameTime));
	}
}

void App::callRender() {
	uint64_t time = currentNanoSeconds;
	uint64_t frameTime = 0;

	while (!this->frame.shouldClose) {
		glfwMakeContextCurrent(this->app.window);
		this->frame.time.timePassedRender = (double) (currentNanoSeconds - time) / 1000000;
		this->render();
		frameTime = (1000000 / this->app.render.maxFPS) - (currentNanoSeconds - time);
		time = currentNanoSeconds;
		if (frameTime > 0) std::this_thread::sleep_for(std::chrono::nanoseconds(1000000 - frameTime));
	}
}

void App::setup(GLFWwindow *win) {
	this->init();
	std::thread updater(this->callUpdate);
	std::thread render(this->callRender);
	glfwSetCursorPosCallback(this->app.window, this->onMouseMoved);
	glfwSetMouseButtonCallback(this->app.window, this->onMouseButton);

	glfwSetWindowPosCallback(this->app.window, this->onWindowMoved);
	glfwSetWindowSizeCallback(this->app.window, this->onWindowResized);
	glfwSetWindowFocusCallback(this->app.window, this->onWindowFocused);
	glfwSetFramebufferSizeCallback(this->app.window, this->onFrameBufferResized);
}

void App::onWindowMoved(GLFWwindow *window, int x, int y) {
	this->frame.window.x = x;
	this->frame.window.y = y;
}
void App::onWindowResized(GLFWwindow *window, int width, int height) {
	this->frame.window.width = width;
	this->frame.window.height = height;
}
void App::onWindowFocused(GLFWwindow *window, int focused) {
	this->frame.window.focused = (bool) focused;
}
void App::onFrameBufferResized(GLFWwindow *window, int width, int height) {
	this->frame.width = width;
	this->frame.height = height;
}

void App::onMouseMoved(GLFWwindow *window, double x, double y) {
	this->frame.mouse.x = x;
	this->frame.mouse.y = y;
}
void App::onMouseButton(GLFWwindow *window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		this->frame.mouse.leftButtonDown = action == GLFW_PRESS;
	} else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		this->frame.mouse.rightButtonDown = action == GLFW_PRESS;
	}
}