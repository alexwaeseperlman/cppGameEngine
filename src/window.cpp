#include "window.hpp"

void Window::onError(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}

void Window::init() {
	atexit(glfwTerminate);
	glfwSetErrorCallback(Window::onError);

	glfwInit();

	// Request an OpenGL 3.3 context (should be core)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Window::Window(float width, float height, std::string title) {
	this->windowptr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	glfwGetFramebufferSize(this->windowptr, &this->fbWidth, &this->fbHeight);

	if (!this->windowptr) {
		onError(1, "Unable to create window!");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(this->windowptr);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval(1);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, this->w, this->h);

	// Swap buffers when window is created in order to make sure it opens
	glfwSwapBuffers(this->windowptr);
}

void Window::setTitle(std::string title) {
	this->title = title;
	glfwSetWindowTitle(this->windowptr, title.c_str());
}

void Window::setSize(int w, int h) {
	this->w = w;
	this->h = h;
	glfwSetWindowSize(this->windowptr, w, h);
}

void Window::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	glfwSetWindowPos(this->windowptr, x, y);
}

void Window::update() {
	glfwGetWindowSize(this->windowptr, &this->w, &this->h);
	glfwGetFramebufferSize(this->windowptr, &this->fbWidth, &this->fbHeight);
	glfwGetWindowPos(this->windowptr, &this->x, &this->y);
	glfwSwapBuffers(this->windowptr);
	glViewport(0, 0, this->w, this->h);
	glfwPollEvents();
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(this->windowptr);
}

Window::~Window() {
	glfwDestroyWindow(this->windowptr);
}

float Window::aspectRatio() {
	return (float) this->w / (float) this->h;
}

glm::vec2 Window::scale() {
	glm::vec2 scale(0, 0);
	glfwGetWindowContentScale(this->windowptr, &scale.x, &scale.y);
	return scale;
}