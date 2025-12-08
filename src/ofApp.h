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
    // Core systems
    World world{200.0f};  // Initialize directly with size
    ofEasyCam cam;

    // Timing
    float dt{1.0f / 60.0f};

    // Projectile spawning
    void spawnBox(const Vector& position, const Vector& dimensions, float mass);
    void throwProjectile();

    // UI
    void drawHUD() const;
};

