#pragma once

// TODO: This entire file needs a revamp to make it generic. Implementing core functionality
// for just Vector3f for now.

// TODO: Remove glm here to make it a dev dependency only
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"

template<int L, typename T>
class Vector
{
private:
public:
    glm::vec<L, T, glm::defaultp> value;

    Vector(glm::vec<L, T, glm::defaultp> value_) : value(value_)
    {};

    Vector()
    {
        value = glm::vec<L, T, glm::defaultp>();
    }

    Vector(T scalar)
    {
        value = glm::vec<L, T, glm::defaultp>(scalar);
    }

    const float* getValuePtr() const
    {
        return glm::value_ptr(value);
    }
};

template<typename T>
class Vector<3, T>
{
private:
    T coords[3];
public:
    Vector(glm::vec<3, T, glm::defaultp> value_)
    {
        coords[0] = value_.x;
        coords[1] = value_.y;
        coords[2] = value_.z;
    };

    Vector(T scalar)
    {
        Vector(glm::vec<3, T, glm::defaultp>(scalar));
    }

    Vector(T a, T b, T c)
    {
        Vector(glm::vec<3, T, glm::defaultp>(a, b, c));
    }

    Vector()
    {
        Vector(0.0f);
    }

    Vector<3, T>& operator=(const Vector<3, T>& rhs)
    {
        coords[0] = rhs.x();
        coords[1] = rhs.y();
        coords[2] = rhs.z();
        return *this;
    }

    bool operator==(const Vector<3, T>& rhs) const
    {
        return (x() == rhs.x()) && (y() == rhs.y()) && (z() == rhs.z());
    }

    bool operator!=(const Vector<3, T>& rhs) const
    {
        return !(rhs == *this);
    }

    Vector<3, T> operator*(const float& scalar) const
    {
        return Vector<3, T>(x() * scalar, y() * scalar, z() * scalar);
    }

    Vector<3, T> operator+(const Vector<3, T>& rhs) const
    {
        return Vector<3, T>(x() + rhs.x(), y() + rhs.y(), z() + rhs.z());
    }

    Vector<3, T> operator-(const Vector<3, T>& rhs) const
    {
        return Vector<3, T>(x() - rhs.x(), y() - rhs.y(), z() - rhs.z());
    }

    Vector<3, T>& operator+=(const Vector<3, T>& rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    Vector<3, T>& operator-=(const Vector<3, T>& rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    const T* getValuePtr() const
    {
        return coords;
    }

    const glm::vec<3, T, glm::defaultp> toGLM() const
    {
        return glm::vec<3, T, glm::defaultp>(x(), y(), z());
    }

    T x() const
    {
        return coords[0];
    }

    T y() const
    {
        return coords[1];
    }

    T z() const
    {
        return coords[2];
    }
};

template<int C, int R, typename T>
class Matrix
{ ;
public:
    glm::mat<C, R, T, glm::defaultp> value;

    Matrix();

    Matrix(T diag);

    const float* getValuePtr() const
    {
        return glm::value_ptr(value);
    }

    Matrix(glm::mat<C, R, T, glm::defaultp> value_) : value(value_)
    {}

    Matrix& operator=(const Matrix& rhs)
    {
        value = rhs.value;
        return *this;
    }
};

typedef Vector<2, int> Vector2i;
typedef Vector<3, int> Vector3i;
typedef Vector<4, int> Vector4i;
typedef Vector<2, float> Vector2f;
typedef Vector<3, float> Vector3f;
typedef Vector<4, float> Vector4f;

typedef Matrix<2, 2, int> Matrix2i;
typedef Matrix<3, 3, int> Matrix3i;
typedef Matrix<4, 4, int> Matrix4i;
typedef Matrix<2, 2, float> Matrix2f;
typedef Matrix<3, 3, float> Matrix3f;
typedef Matrix<4, 4, float> Matrix4f;

#define FORWARD  Vector3f( 0.0f,  0.0f, -1.0f)
#define BACKWARD Vector3f( 0.0f,  0.0f,  1.0f)
#define LEFT     Vector3f(-1.0f,  0.0f,  0.0f)
#define RIGHT    Vector3f( 1.0f,  0.0f,  0.0f)
#define UP       Vector3f( 0.0f,  1.0f,  0.0f)
#define DOWN     Vector3f( 0.0f, -1.0f,  0.0f)
#define ORIGIN   Vector3f( 0.0f,  0.0f,  0.0f)

/**
 * OPERATIONS
 */

namespace Lotus
{
    Vector3f LNormalize(const Vector3f& vec);

    Vector3f LCross(const Vector3f& a, const Vector3f& b);

    Matrix4f LLookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up);

    Matrix4f LPerspective(float fov, float aspectRatio, float zNear, float zFar);

    Matrix4f LTranslate(const Matrix4f& mat, const Vector3f& translationVec);

    Matrix4f LScale(const Matrix4f& mat, const Vector3f& scaleVec);

    Matrix4f LRotate(Matrix4f mat, float deg, Vector3f axis);
}
