#include "test.hpp"
#include "shader.hpp"
#include <glad/glad.h>
#include <iostream>

#define FRAMEWORK_PLATFORM_GLFW

#ifdef __linux__
#include <GL/glu.h>
#else
#include <OpenGL/glu.h>
#endif

#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "sprite.hpp"
#include "vertexObject.hpp"
#include "window.hpp"

#include <chrono>
#include <thread>

#include <algorithm>

#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <Ultralight/Ultralight.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Window *window;

Shader *whiteShader;
SpriteSheet *testSheet;
SpriteRenderer *testRenderer;

Sprite *sprite;

Camera2D *camera;

using namespace ultralight;
static bool finished = false;

class LoadListenerImpl : public LoadListener {
public:
	virtual ~LoadListenerImpl() {}

	virtual void OnFinishLoading(ultralight::View *caller) { finished = true; }
};

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
	Window::init();

	window = new Window(width, height, "WOW");
	window->update();
	initLibraries();

	camera = new Camera2D();
	camera->dimensions.x = window->getWidth();
	camera->dimensions.y = window->getHeight();
	camera->update();

	whiteShader = Shader::loadShader("assets/white");
	std::cout << "Shader: " << gluErrorString(glGetError()) << std::endl;

	whiteShader->printCompileLog();
	whiteShader->printLog();

	testSheet = new SpriteSheet("assets/animation.spritesheet.png");
	testSheet->loadFromJSON("assets/animation.spritesheet.json");
	std::cout << "Created Sheet: " << gluErrorString(glGetError()) << std::endl;
	testRenderer = new SpriteRenderer(testSheet, 10000);
	std::cout << "Created Renderer: " << gluErrorString(glGetError()) << std::endl;

	Sprite *test = testRenderer->addSprite(-0.25, 0, 0);
	sprite = testRenderer->addSprite(0, 0, 1);
	test->setSize(71, 95);
	test->setPosition(-0.2, 0);
	test->setTextureID(1);

	sprite->setDrawMode(Sprite::drawMode::CENTER);

	std::cout << "Created Sprite: " << gluErrorString(glGetError()) << std::endl;

	sprite->setPosition(0, 0);
	sprite->setSize(71, 95);

	std::cout << "Set Sprite Information: " << gluErrorString(glGetError()) << std::endl << std::endl;

	window->mouseMoveListeners.push_back(cursorPos);

	auto htmlRenderer = ultralight::Renderer::Create();

	std::cout << "Creating View..." << std::endl;

	auto view = htmlRenderer->CreateView(200, 200, false);

	LoadListenerImpl *load_listener = new LoadListenerImpl();
	view->set_load_listener(load_listener);

	view->LoadHTML(R"(
	        <html>
	          <head>
	            <style type="text/css">
	            h1 { background: yellow; }
	            h1, p { padding: 8px; text-align: center; }
	            </style>
	            <title>Hello World!</title>
	          </head>
	          <body>
	            <h1>Hello World!</h1>
	            <p>Welcome to Ultralight!</p>
	          </body>
	        </html>
	        )");

	std::cout << "Starting update loop..." << std::endl;

	while (!finished) htmlRenderer->Update();

	std::cout << "Writing bitmap to output.png" << std::endl;

	htmlRenderer->Render();

	view->bitmap()->WritePNG("output.png");

	std::cout << "Done." << std::endl;

	std::cout << "\n\n";

	view->set_load_listener(nullptr);
	delete load_listener;

	return true;
}

bool loadAssets() {
	return true;
}

float rotation = 0;

bool mainLoop() { // Render here /
	// std::cout << "\x1b[A\x1b[A\x1b[A\x1b[A\x1b[A\x1b[A";

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	std::cout << "Clear: " << gluErrorString(glGetError()) << std::endl;

	sprite->setPosition(mouseX - width / 2, -mouseY + height / 2);

	sprite->setTextureID((sprite->getTextureID() + 1) % 10 + 1);

	camera->dimensions.x = width;
	camera->dimensions.y = height;
	rotation += 0.05;
	std::cout << "Rotation should be: " << rotation << std::endl;
	camera->rotate(0, rotation, 0);
	camera->update();

	SpriteRenderer::spriteShader->bind();

	sprite->setRotation((int) floor(rotation));
	glUniformMatrix4fv(SpriteRenderer::viewProjectionUniform, 1, GL_FALSE,
	                   (float *) glm::value_ptr(camera->cameraMatrix));
	std::cout << "Camera values: " << glm::to_string(camera->cameraMatrix) << "\nWidth: " << width
	          << ", height: " << height << std::endl;
	testRenderer->display();
	std::cout << "Draw Sprites: " << gluErrorString(glGetError()) << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(15));
	window->update();

	return true;
}

void close() {
	// Destroy window
	delete window;

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
			while (!window->shouldClose()) {
				mainLoop();
			}
		}
	}

	// Free resources and close GLFW
	close();

	return 0;
}