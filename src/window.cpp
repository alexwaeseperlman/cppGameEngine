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

	// HOLY CRAP SO MANY EVENT HANDLERS
	glfwSetKeyCallback(this->windowptr, keyfun);
	glfwSetCharCallback(this->windowptr, charfun);
	glfwSetCharModsCallback(this->windowptr, charmodsfun);

	glfwSetCursorPosCallback(this->windowptr, cursorposfun);
	glfwSetCursorEnterCallback(this->windowptr, cursorenterfun);
	glfwSetMouseButtonCallback(this->windowptr, mousebuttonfun);
	glfwSetScrollCallback(this->windowptr, scrollfun);

	glfwSetDropCallback(this->windowptr, dropfun);
	glfwSetWindowSizeCallback(this->windowptr, windowsizefun);
	glfwSetFramebufferSizeCallback(this->windowptr, framebuffersizefun);
	glfwSetWindowIconifyCallback(this->windowptr, windowiconifyfun);
	glfwSetWindowFocusCallback(this->windowptr, windowfocusfun);
	glfwSetWindowPosCallback(this->windowptr, windowposfun);
	glfwSetWindowMaximizeCallback(this->windowptr, windowmaximizefun);

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

// Keyboard
std::vector<GLFWkeyfun> Window::keyListeners = {};
std::vector<GLFWcharfun> Window::charListeners = {};
std::vector<GLFWcharmodsfun> Window::modifierListeners = {};

// Mouse
std::vector<GLFWcursorposfun> Window::mouseMoveListeners = {};
std::vector<GLFWcursorenterfun> Window::mouseEnterListeners = {};
std::vector<GLFWmousebuttonfun> Window::mouseButtonListeners = {};
std::vector<GLFWscrollfun> Window::scrollListeners = {};

// OS
std::vector<GLFWdropfun> Window::fileDropListeners = {};
std::vector<GLFWwindowsizefun> Window::windowResizeListeners = {};
std::vector<GLFWframebuffersizefun> Window::framebufferResizeListeners = {};
std::vector<GLFWwindowiconifyfun> Window::minimizeListeners = {};
std::vector<GLFWwindowfocusfun> Window::focusedListeners = {};
std::vector<GLFWwindowposfun> Window::windowMovedListeners = {};
std::vector<GLFWwindowmaximizefun> Window::windowMaximizedListeners = {};

void Window::keyfun(GLFWwindow *window, int key, int scancode, int action, int mods) {
	for (GLFWkeyfun fun : keyListeners) {
		fun(window, key, scancode, action, mods);
	}
}
void Window::charfun(GLFWwindow *window, unsigned int codepoint) {
	for (GLFWcharfun fun : charListeners) {
		fun(window, codepoint);
	}
}
void Window::charmodsfun(GLFWwindow *window, unsigned int codepoint, int mods) {
	for (GLFWcharmodsfun fun : modifierListeners) {
		fun(window, codepoint, mods);
	}
}

void Window::cursorposfun(GLFWwindow *window, double xpos, double ypos) {
	for (GLFWcursorposfun fun : mouseMoveListeners) {
		fun(window, xpos, ypos);
	}
}
void Window::cursorenterfun(GLFWwindow *window, int entered) {
	for (GLFWcursorenterfun fun : mouseEnterListeners) {
		fun(window, entered);
	}
}
void Window::mousebuttonfun(GLFWwindow *window, int button, int action, int mods) {
	for (GLFWmousebuttonfun fun : mouseButtonListeners) {
		fun(window, button, action, mods);
	}
}
void Window::scrollfun(GLFWwindow *window, double xoffset, double yoffset) {
	for (GLFWscrollfun fun : scrollListeners) {
		fun(window, xoffset, yoffset);
	}
}

void Window::dropfun(GLFWwindow *window, int count, const char **paths) {
	for (GLFWdropfun fun : fileDropListeners) {
		fun(window, count, paths);
	}
}
void Window::windowsizefun(GLFWwindow *window, int width, int height) {
	for (GLFWwindowsizefun fun : windowResizeListeners) {
		fun(window, width, height);
	}
}
void Window::framebuffersizefun(GLFWwindow *window, int width, int height) {
	for (GLFWframebuffersizefun fun : framebufferResizeListeners) {
		fun(window, width, height);
	}
}
void Window::windowiconifyfun(GLFWwindow *window, int iconified) {
	for (GLFWwindowiconifyfun fun : minimizeListeners) {
		fun(window, iconified);
	}
}
void Window::windowfocusfun(GLFWwindow *window, int focused) {
	for (GLFWwindowfocusfun fun : focusedListeners) {
		fun(window, focused);
	}
}
void Window::windowposfun(GLFWwindow *window, int xpos, int ypos) {
	for (GLFWwindowposfun fun : windowMovedListeners) {
		fun(window, xpos, ypos);
	}
}
void Window::windowmaximizefun(GLFWwindow *window, int maximized) {
	for (GLFWwindowmaximizefun fun : windowMaximizedListeners) {
		fun(window, maximized);
	}
}
