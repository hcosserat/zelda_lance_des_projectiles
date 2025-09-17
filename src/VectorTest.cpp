#include "VectorTest.h"
#include "Vector.h"
#include <cassert>
#include <iostream>

void test_constructeur() {
	Vector v1(1.0f, 2.0f, 3.0f);
	assert(v1.vec3().x == 1.0f);
	assert(v1.vec3().y == 2.0f);
	assert(v1.vec3().z == 3.0f);

	Vector v2(0.0f, 0.0f, 0.0f);
	assert(v2.vec3().x == 0.0f);
	assert(v2.vec3().y == 0.0f);
	assert(v2.vec3().z == 0.0f);
}

void test_methods() {
	Vector v(1.0f, 2.0f, 3.0f);

	// Test de `vec2()`
	glm::vec2 vec2 = v.vec2();
	assert(vec2.x == 1.0f);
	assert(vec2.y == 2.0f);

	// Test de `vec3()`
	glm::vec3 vec3 = v.vec3();
	assert(vec3.x == 1.0f);
	assert(vec3.y == 2.0f);
	assert(vec3.z == 3.0f);

	// Test de `color()`
	ofColor color = v.color();
	assert(color.r == 1.0f);
	assert(color.g == 2.0f);
	assert(color.b == 3.0f);
}

void test_op_add_sub() {
	Vector v1(1.0f, 2.0f, 3.0f);
	Vector v2(4.0f, 5.0f, 6.0f);

	// Test de l'addition
	Vector result = v1 + v2;
	assert(result.x == 5.0f);
	assert(result.y == 7.0f);
	assert(result.z == 9.0f);

	// Test de la soustraction
	result = v1 - v2;
	assert(result.x == -3.0f);
	assert(result.y == -3.0f);
	assert(result.z == -3.0f);
}

void test_op_multiply() {
	Vector v(1.0f, 2.0f, 3.0f);

	// Test de multiplication par un scalaire
	Vector result = v * 2.0f;
	assert(result.x == 2.0f);
	assert(result.y == 4.0f);
	assert(result.z == 6.0f);

	// Test de l'opérateur *=
	v *= 2.0f;
	assert(v.x == 2.0f);
	assert(v.y == 4.0f);
	assert(v.z == 6.0f);

	// Test de l'opérateur %=
	v %= 2.0f; // doit donner (0, 0, 0)
	assert(v.x == 0.0f);
	assert(v.y == 0.0f);
	assert(v.z == 0.0f);
}

void test_normalize() {
	Vector v(3.0f, 4.0f, 0.0f);

	v.normalize();
	assert(v.x == 0.6f); // 3 / 5
	assert(v.y == 0.8f); // 4 / 5
	assert(v.z == 0.0f);
}

void test_dot_cross() {
	Vector v1(1.0f, 2.0f, 3.0f);
	Vector v2(4.0f, 5.0f, 6.0f);

	// Test du produit scalaire
	float dotProduct = v1.dot(v2);
	assert(dotProduct == 32.0f); // 1*4 + 2*5 + 3*6 = 32

	// Test du produit vectoriel
	Vector crossProduct = v1.cross(v2);
	assert(crossProduct.x == -3.0f);
	assert(crossProduct.y == 6.0f);
	assert(crossProduct.z == -3.0f);
}

void test_distance() {
	Vector v1(1.0f, 2.0f, 3.0f);
	Vector v2(4.0f, 5.0f, 6.0f);

	// Test de la distance
	float dist = v1.distance(v2);
	assert(dist == sqrt(27.0f));

	// Test de la distance au carré
	float distSquared = v1.distanceSquared(v2);
	assert(distSquared == 27.0f);
}

void test_op_eq() {
	Vector v1(1.0f, 2.0f, 3.0f);
	Vector v2(1.0f, 2.0f, 3.0f);
	Vector v3(3.0f, 2.0f, 1.0f);

	// Test de l'égalité
	assert(v1 == v2);

	// Test de l'inégalité
	assert(!(v1 == v3));
}

int test_vector() {
	test_constructeur();
	test_methods();
	test_op_add_sub();
	test_op_multiply();
	test_normalize();
	test_dot_cross();
	test_distance();
	test_op_eq();

	std::cout << "Tous les tests sont passés avec succès !" << std::endl;
	return 0;
}
