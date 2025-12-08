#include "BoxShape.h"
#include "RigidBody.h"
#include <glm/glm.hpp>

std::vector<Vector> BoxShape::getVerticesWorld(const RigidBody& body) const {
    std::vector<Vector> vertices;
    float halfX = dimensions.x / 2.0f;
    float halfY = dimensions.y / 2.0f;
    float halfZ = dimensions.z / 2.0f;

    // Local vertices
    vertices.emplace_back(halfX, halfY, halfZ);
    vertices.emplace_back(-halfX, halfY, halfZ);
    vertices.emplace_back(halfX, -halfY, halfZ);
    vertices.emplace_back(-halfX, -halfY, halfZ);
    vertices.emplace_back(halfX, halfY, -halfZ);
    vertices.emplace_back(-halfX, halfY, -halfZ);
    vertices.emplace_back(halfX, -halfY, -halfZ);
    vertices.emplace_back(-halfX, -halfY, -halfZ);

    // Transform to world space
    for (auto& vertex : vertices) {
        glm::vec3 inputVec(vertex.x, vertex.y, vertex.z);
        glm::vec3 result = body.orientation.rotateVector(inputVec);
        Vector rotatedVertex(result.x, result.y, result.z);
        vertex = rotatedVertex + body.center;
    }

    return vertices;
}
