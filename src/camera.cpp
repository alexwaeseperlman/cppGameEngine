#include "camera.hpp"
#include <iostream>

Camera::Camera() {
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rotation = glm::quat(0, 0, 0, 0);
	dimensions = glm::vec2(1, 1);
}

void Camera::rotate(float x, float y, float z) {
	// this->rotation += glm::quat(glm::vec3(x, y, z));
	this->rotation = glm::rotate(this->rotation, y, glm::vec3(x, y, z));
	std::cout << "Rotation: " << glm::pitch(this->rotation) << ", " << glm::yaw(this->rotation) << ", "
	          << glm::roll(this->rotation) << std::endl;
}

void Camera2D::update() {
	this->projectionMatrix = glm::ortho(-this->dimensions.x / 2, this->dimensions.x / 2, -this->dimensions.y / 2,
	                                    this->dimensions.y / 2, this->near, this->far);
	this->viewMatrix = glm::translate(glm::mat4(1.0), this->position) * glm::toMat4(this->rotation) /* *
	     /*glm::scale(glm::mat4(1.0), this->scale)*/
	    ;

	this->cameraMatrix = this->viewMatrix * this->projectionMatrix;
}

void Camera2D::rotate(float z) {
	Camera::rotate(0, 0, z);
}

void Camera2D::rotate(float x, float y, float z) {
	Camera::rotate(x, y, z);
}

Camera2D::Camera2D() : Camera::Camera() {
}