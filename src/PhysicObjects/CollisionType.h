#pragma once

enum CollisionType {
    InterpenetrationCollision, // interpénétration
    RestingContactsCollision, // contact au repos
    RodConstraint, // Rod constraint (fixed distance)
    CableConstraint, // Cable constraint (max distance)
    TBD // type de collision à déterminer
};
