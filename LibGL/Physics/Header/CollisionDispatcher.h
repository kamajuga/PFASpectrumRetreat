#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <functional>
#include <utility>
#include "LibMath/GeometricObject3.h" 
#include "LibMath/Collisions.h"


namespace Object = LibMath::Geometry3D;

// Type alias
using       CollisionFunc = std::function<bool(const Object::Object3D&, const Object::Object3D&)>;
using       TypeIDPair = std::pair<std::type_index, std::type_index>;

// Custom hash for std::pair<std::type_index, std::type_index>
struct      TypeIDPairHasher
{
    std::size_t operator()(const TypeIDPair& p) const noexcept
    {
        return std::hash<std::type_index>{}(p.first) ^ (std::hash<std::type_index>{}(p.second) << 1);
    }
};

// Static map for dispatch
inline      std::unordered_map<TypeIDPair, CollisionFunc, TypeIDPairHasher> collisionDispatch;

// Generic collision dispatcher
inline bool checkCollision(const Object::Object3D& a, const Object::Object3D& b)
{
    TypeIDPair key{ typeid(a), typeid(b) };
    auto it = collisionDispatch.find(key);
    if (it != collisionDispatch.end())
        return it->second(a, b);

    // Try reversed order (if symmetric)
    key = { typeid(b), typeid(a) };
    it = collisionDispatch.find(key);
    if (it != collisionDispatch.end())
        return it->second(b, a);

    return false; // Unknown combination
}

// Register supported combinations
inline void registerCollisions()
{
    using namespace LibMath::Collisions3D;

    collisionDispatch[{typeid(Object::Sphere), typeid(Object::Sphere)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::Sphere&>(a), static_cast<const Object::Sphere&>(b));
        };

    collisionDispatch[{typeid(Object::Sphere), typeid(Object::Point)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::Sphere&>(a), static_cast<const Object::Point&>(b));
        };

    collisionDispatch[{typeid(Object::Sphere), typeid(Object::Line)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::Sphere&>(a), static_cast<const Object::Line&>(b));
        };

    collisionDispatch[{typeid(Object::Sphere), typeid(Object::Plan)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::Sphere&>(a), static_cast<const Object::Plan&>(b));
        };

    collisionDispatch[{typeid(Object::Plan), typeid(Object::Plan)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::Plan&>(a), static_cast<const Object::Plan&>(b));
        };

    collisionDispatch[{typeid(Object::Plan), typeid(Object::Line)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::Plan&>(a), static_cast<const Object::Line&>(b));
        };

    collisionDispatch[{typeid(Object::Plan), typeid(Object::Point)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::Plan&>(a), static_cast<const Object::Point&>(b));
        };

    collisionDispatch[{typeid(Object::Capsule), typeid(Object::Point)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::Capsule&>(a), static_cast<const Object::Point&>(b));
        };

    collisionDispatch[{typeid(Object::Capsule), typeid(Object::Line)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::Capsule&>(a), static_cast<const Object::Line&>(b));
        };

    collisionDispatch[{typeid(Object::AABB), typeid(Object::Line)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::AABB&>(a), static_cast<const Object::Line&>(b));
        };

    collisionDispatch[{typeid(Object::AABB), typeid(Object::AABB)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::AABB&>(a), static_cast<const Object::AABB&>(b));
        };

    collisionDispatch[{typeid(Object::Capsule), typeid(Object::AABB)}] = [](const Object::Object3D& a, const Object::Object3D& b) 
        {
        return checkCollision(static_cast<const Object::Capsule&>(a), static_cast<const Object::AABB&>(b));
        };
}



