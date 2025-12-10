#pragma once

#include "ofMain.h"
#include "Actors/World.h"

class ofApp final : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

    void keyPressed(int key) override;
    void keyReleased(int key) override;

    void mouseMoved(int x, int y) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;

    void windowResized(int w, int h) override;
    void gotMessage(ofMessage msg) override;

    void dragEvent(ofDragInfo dragInfo) override;

private:
    // Systèmes principaux
    World world{200.0f}; // taille du monde
    ofEasyCam cam;

    // Timing
    float dt{1.0f / 60.0f};

    // Génération de projectiles
    void throwProjectile();
	void throwForwardProjectile();

    // UI
    void drawHUD() const;

	float lastThrowTime = 0.0f;
	const float throwCooldown = 0.3f; // 300 ms
};
