#include "HUD.h"
#include <cmath>

void HUD::computeConstants() {
	omega_0 = std::sqrt(springConstant); // Natural frequency
	zeta = dampingFactor / (2.0f * omega_0); // Damping ratio
	if (zeta < 1.0f) {
		omega_d = omega_0 * std::sqrt(1.0f - zeta * zeta); // Damped natural frequency
	} else {
		omega_d = 0.0f; // Overdamped or critically damped
	}
}

void HUD::draw(float x, float y) {
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Particles: " + std::to_string(static_cast<int>(displayValue)), x, y + yOffset);
}

void HUD::setTargetValue(float value) {
	if (value != targetValue) {
		displayValue = value;
		float valueChange = std::fabs(value - targetValue);
		initialYDisplacement = -valueChange * 10.0f; // Initial displacement proportional to change
		initialYVelocity = 0.0f; // Start with zero initial velocity
		targetValue = value;
		timeElapsed = 0.0f;
		computeConstants();
		K1 = initialYDisplacement;
		if (omega_d > 0.0f) {
			K2 = (zeta * omega_0 * initialYDisplacement + initialYVelocity) / omega_d;
		} else {
			K2 = 0.0f; // No oscillation in overdamped or critically damped case
		}
	}
}

void HUD::update(float dt) {
	timeElapsed += dt;
	float decay = std::exp(-zeta * omega_0 * timeElapsed);
	float oscillation;
	if (omega_d > 0.0f) {
		oscillation = K1 * std::cos(omega_d * timeElapsed) + K2 * std::sin(omega_d * timeElapsed);
	} else {
		oscillation = K1 * decay; // Overdamped or critically damped
	}
	yOffset = decay * oscillation;
	if (timeElapsed > 1.0f && std::fabs(yOffset) < 0.01f) {
		yOffset = 0.0f; // Snap to zero
		timeElapsed = 0.0f; // Reset time
		K1 = 0.0f;
		K2 = 0.0f;
	}
}