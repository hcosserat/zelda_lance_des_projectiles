#pragma once
#include "ofMain.h"

class HUD {
private:
	float targetValue = 0.0f;
	float displayValue = 0.0f;
	float yOffset = 0.0f;
	float targetYOffset = 0.0f;
	float initialYDisplacement = 0.0f;
	float initialYVelocity = 0.0f;
	float timeElapsed = 0.0f;
	float K1, K2;
	float omega_0, omega_d, zeta;
	const float springConstant = 50.0f; // Spring constant for the spring-damper model
	const float dampingFactor = 0.5f; // Damping factor for the spring-damper model

	void computeConstants();

public:
	void update(float dt);
	void draw(float x, float y);
	void setTargetValue(float value);
};