#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp>   // glm::vec3
#include <glm/vec4.hpp>   // glm::vec4

class Camera {
public:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	Camera();

	void rotate(float x, float y, float z);
	void setTarget(float x, float y, float z);

	virtual void update() = 0;

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec2 dimensions;
	float near = 0.1;
	float far = 10;

	void bind();
	glm::mat4 cameraMatrix;
};

class Camera2D : public Camera {
public:
	Camera2D();
	void update();
	void rotate(float z);
	void rotate(float x, float y, float z);
};