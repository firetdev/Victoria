#pragma once

#include <cmath>
#include <iostream>

class Vec2 {
public:
    float x = 0.0f;
    float y = 0.0f;

    // Constructors
    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}

    // Copy & assignment
    Vec2(const Vec2&) = default;
    Vec2& operator=(const Vec2&) = default;

    // Vector addition
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Vector subtraction
    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2& operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Scalar multiplication
    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Scalar division
    Vec2 operator/(float scalar) const {
        return Vec2(x / scalar, y / scalar);
    }

    Vec2& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Vector magnitude
    float Length() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalize vector
    Vec2 Normalized() const {
        float len = Length();
        if (len == 0) return Vec2(0, 0);
        return Vec2(x / len, y / len);
    }

    // Dot product
    float Dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    // Utility: print vector
    void Print() const {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};
