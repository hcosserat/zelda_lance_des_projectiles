#include "Projectile.h"

Projectile::Projectile(Particle p, projectileType t) : particle(p),
                                                       type(t),
                                                       velStart(Vector(0, 0, 0)) {
    if (velStart.isNAN() || velStart == Vector(0, 0, 0)) {
        velStart = p.vel;
    }
}
