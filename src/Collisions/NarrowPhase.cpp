#include "NarrowPhase.h"
#include <limits>

// Utilitaires internes pour le SAT
namespace {
    // Récupère les axes locaux (normalisés) d'un RigidBody
    void GetAxes(const RigidBody *body, Vector axes[3]) {
        // On suppose que l'orientation est stockée dans body->orientation
        // On récupère les colonnes de la matrice de rotation
        Matrix3 R = body->orientation.toRotationMatrix3();
        axes[0] = R.a; // Axe X local (colonne 0)
        axes[1] = R.b; // Axe Y local (colonne 1)
        axes[2] = R.c; // Axe Z local (colonne 2)
    }

    // Projette une boîte sur un axe et retourne la demi-longueur de l'intervalle projeté
    float TransformToAxis(const BoxShape *box, const Vector axes[3], const Vector &axis) {
        const Vector &dims = box->getDimensions();
        return (dims.x * 0.5f) * std::abs(axis.dot(axes[0])) +
               (dims.y * 0.5f) * std::abs(axis.dot(axes[1])) +
               (dims.z * 0.5f) * std::abs(axis.dot(axes[2]));
    }

    // Teste le chevauchement sur un axe donné.
    // Retourne la pénétration (négative si séparation, positive si collision).
    float PenetrationOnAxis(const BoxShape *box1, const RigidBody *body1,
                            const BoxShape *box2, const RigidBody *body2,
                            const Vector &axis, const Vector &toCenter) {
        // Projection des demi-tailles
        Vector axes1[3], axes2[3];
        GetAxes(body1, axes1);
        GetAxes(body2, axes2);

        float oneProjected = TransformToAxis(box1, axes1, axis);
        float twoProjected = TransformToAxis(box2, axes2, axis);

        // Distance projetée entre les centres
        float distance = std::abs(toCenter.dot(axis));

        // Pénétration = (somme des rayons) - distance
        return oneProjected + twoProjected - distance;
    }
}

CollisionData NarrowPhase::CheckCollision(RigidBody *body1, RigidBody *body2) {
    CollisionData result;
    result.body1 = body1;
    result.body2 = body2;

    if (body1->shape->type() == BOX && body2->shape->type() == PLANE) {
        return BoxAndPlane(body1, body2);
    }

    if (body1->shape->type() == PLANE && body2->shape->type() == BOX) {
        return BoxAndPlane(body2, body1);
    }

    if (body1->shape->type() == BOX && body2->shape->type() == BOX) {
        return BoxAndBox(body1, body2);
    }

    return result;
}

CollisionData NarrowPhase::BoxAndPlane(RigidBody *boxBody, RigidBody *planeBody) {
    CollisionData collisionData;
    collisionData.body1 = boxBody;
    collisionData.body2 = planeBody;

    auto *boxShape = dynamic_cast<BoxShape *>(boxBody->shape.get());
    auto vertices = boxShape->getVerticesWorld(*boxBody);

    auto *planeShape = dynamic_cast<PlaneShape *>(planeBody->shape.get());
    Vector planeNormal = planeShape->getNormal();
    Vector planePoint = planeShape->getPoint();

    for (const auto &Q: vertices) {
        Vector boxToPlane = Q - planePoint;
        float distance = boxToPlane.dot(planeNormal);
        if (distance < 0) {
            collisionData.addContact(Q - planeNormal * distance, planeNormal, -distance);
        }
    }
    return collisionData;
}

// -----------------------------------------------------------------------------------------------
// Implémentation SAT robuste pour Box vs Box
// -----------------------------------------------------------------------------------------------
CollisionData NarrowPhase::BoxAndBox(RigidBody *one, RigidBody *two) {
    CollisionData data;
    data.body1 = one;
    data.body2 = two;

    auto *box1 = dynamic_cast<BoxShape *>(one->shape.get());
    auto *box2 = dynamic_cast<BoxShape *>(two->shape.get());

    Vector toCenter = two->center - one->center; // Vecteur de 1 vers 2

    Vector axes1[3], axes2[3];
    GetAxes(one, axes1);
    GetAxes(two, axes2);

    float bestOverlap = std::numeric_limits<float>::max();
    Vector bestAxis;
    int bestAxisIndex = -1;

    // --- Étape 1 : Tester les 15 axes ---
    // (Cette partie reste identique, elle trouve l'axe de moindre pénétration)

    // Axes de la boîte 1
    for (int i = 0; i < 3; i++) {
        float overlap = PenetrationOnAxis(box1, one, box2, two, axes1[i], toCenter);
        if (overlap < 0) return data;
        if (overlap < bestOverlap) {
            bestOverlap = overlap;
            bestAxis = axes1[i];
            bestAxisIndex = i;
        }
    }

    // Axes de la boîte 2
    for (int i = 0; i < 3; i++) {
        float overlap = PenetrationOnAxis(box1, one, box2, two, axes2[i], toCenter);
        if (overlap < 0) return data;
        if (overlap < bestOverlap) {
            bestOverlap = overlap;
            bestAxis = axes2[i];
            bestAxisIndex = i + 3;
        }
    }

    // Axes dérivés (produits vectoriels)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Vector axis = axes1[i].cross(axes2[j]);
            if (axis.normSquared() < 0.001f) continue;
            axis.normalize();
            float overlap = PenetrationOnAxis(box1, one, box2, two, axis, toCenter);
            if (overlap < 0) return data;
            if (overlap < bestOverlap) {
                bestOverlap = overlap;
                bestAxis = axis;
                bestAxisIndex = 6 + i * 3 + j;
            }
        }
    }

    // --- Étape 2 : Résultat confirmé, générer le contact ---

    // [CORRECTION ICI]
    // On veut que la normale pointe de Body2 vers Body1.
    // toCenter va de 1 vers 2. Donc si l'axe va dans le même sens (dot > 0), on l'inverse.
    if (bestAxis.dot(toCenter) > 0) {
        bestAxis = bestAxis * -1.0f;
    }

    // Le reste du code devient correct mathématiquement grâce à cette inversion :
    // - normalForIncident pointera maintenant vers la face incidente correcte (la plus proche)
    // - resolveContact poussera les objets dans le bon sens

    Vector contactPoint;

    if (bestAxisIndex < 6) {
        // Collision Face-Face
        RigidBody *incidentBody = (bestAxisIndex < 3) ? two : one;
        Vector normalForIncident = (bestAxisIndex < 3) ? bestAxis * -1.0f : bestAxis;

        auto *incidentBox = dynamic_cast<BoxShape *>(incidentBody->shape.get());
        auto vertices = incidentBox->getVerticesWorld(*incidentBody);

        Vector bestVertex;
        float minProj = std::numeric_limits<float>::max();

        for (const auto &v: vertices) {
            float proj = v.dot(normalForIncident);
            if (proj < minProj) {
                minProj = proj;
                bestVertex = v;
            }
        }
        contactPoint = bestVertex;
    } else {
        // Collision Arête-Arête
        Vector pt1 = one->center + (bestAxis * (TransformToAxis(box1, axes1, bestAxis)));
        Vector pt2 = two->center - (bestAxis * (TransformToAxis(box2, axes2, bestAxis)));
        contactPoint = (pt1 + pt2) * 0.5f;
    }

    data.addContact(contactPoint, bestAxis, bestOverlap);
    return data;
}
