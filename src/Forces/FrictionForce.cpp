#include "FrictionForce.h"
#include "Gravity.h"
#include "Maths/vector.h"

FrictionForce::FrictionForce(const float coef_sta, const float coef_cin, const Vector n)
    : coef_friction_sta(coef_sta)
      , coef_friction_cin(coef_cin)
      , vec_norm(n) {
}

void FrictionForce::updateforce(Particle *p, float /*dt*/) {
    if (!p) return;
    // Projection de la gravit� sur le plan (composante tangentielle)
    Vector g_normal = vec_norm * GRAVITY.dot(vec_norm);
    // --- Projection sur le plan tangent ---
    Vector g_tangent = GRAVITY - g_normal;
    float norm_fric = coef_friction_sta * g_tangent.norm();
    if (g_tangent.norm() < norm_fric) {
        p->addforce(norm_fric * g_tangent.normalized());
    } else {
        float norm_fric = coef_friction_cin * g_tangent.norm();
        p->addforce(norm_fric * g_tangent.normalized());
    }
}
