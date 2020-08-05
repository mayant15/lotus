#pragma once

// TODO: Remove glm here to make it a dev dependency only
#include "internal/glm/glm.hpp"

typedef glm::ivec2 Vector2i;
typedef glm::ivec3 Vector3i;
typedef glm::ivec4 Vector4i;
typedef glm::vec2  Vector2f;
typedef glm::vec3  Vector3f;
typedef glm::vec4  Vector4f;

typedef glm::imat2x2 Matrix2i;
typedef glm::imat3x3 Matrix3i;
typedef glm::imat4x4 Matrix4i;
typedef glm::mat2    Matrix2f;
typedef glm::mat3    Matrix3f;
typedef glm::mat4    Matrix4f;

#define FRONT    Vector3f( 0.0f,  0.0f, -1.0f)
#define BACK     Vector3f( 0.0f,  0.0f,  1.0f)
#define LEFT     Vector3f(-1.0f,  0.0f,  0.0f)
#define RIGHT    Vector3f( 1.0f,  0.0f,  0.0f)
#define UP       Vector3f( 0.0f,  1.0f,  0.0f)
#define DOWN     Vector3f( 0.0f, -1.0f,  0.0f)
#define ORIGIN   Vector3f( 0.0f,  0.0f,  0.0f)

#define X_AXIS RIGHT
#define Y_AXIS UP
#define Z_AXIS BACK

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

    const float* valuePtr(const Matrix4f& mat);

    const float* valuePtr(const Vector3f& vec);
}



//template<int L, typename T>
//class Vector
//{
//private:
//public:
//    glm::vec<L, T, glm::defaultp> value;
//
//    Vector(glm::vec<L, T, glm::defaultp> value_) : value(value_)
//    {};
//
//    Vector()
//    {
//        value = glm::vec<L, T, glm::defaultp>();
//    }
//
//    Vector(T scalar)
//    {
//        value = glm::vec<L, T, glm::defaultp>(scalar);
//    }
//
//    const float* getValuePtr() const
//    {
//        return glm::value_ptr(value);
//    }
//};
//
//template<typename T>
//class Vector<3, T>
//{
//private:
//    T coords[3];
//public:
//    Vector(glm::vec<3, T, glm::defaultp> value_)
//    {
//        coords[0] = value_.x;
//        coords[1] = value_.y;
//        coords[2] = value_.z;
//    };
//
//    Vector(T scalar)
//    {
//        Vector(glm::vec<3, T, glm::defaultp>(scalar));
//    }
//
//    Vector(T a, T b, T c)
//    {
//        Vector(glm::vec<3, T, glm::defaultp>(a, b, c));
//    }
//
//    Vector()
//    {
//        Vector(0.0f);
//    }
//
//    Vector<3, T>& operator=(const Vector<3, T>& rhs)
//    {
//        coords[0] = rhs.x();
//        coords[1] = rhs.y();
//        coords[2] = rhs.z();
//        return *this;
//    }
//
//    bool operator==(const Vector<3, T>& rhs) const
//    {
//        return (x() == rhs.x()) && (y() == rhs.y()) && (z() == rhs.z());
//    }
//
//    bool operator!=(const Vector<3, T>& rhs) const
//    {
//        return !(rhs == *this);
//    }
//
//    Vector<3, T> operator*(const float& scalar) const
//    {
//        return Vector<3, T>(x() * scalar, y() * scalar, z() * scalar);
//    }
//
//    Vector<3, T> operator+(const Vector<3, T>& rhs) const
//    {
//        return Vector<3, T>(x() + rhs.x(), y() + rhs.y(), z() + rhs.z());
//    }
//
//    Vector<3, T> operator-(const Vector<3, T>& rhs) const
//    {
//        return Vector<3, T>(x() - rhs.x(), y() - rhs.y(), z() - rhs.z());
//    }
//
//    Vector<3, T>& operator+=(const Vector<3, T>& rhs)
//    {
//        *this = *this + rhs;
//        return *this;
//    }
//
//    Vector<3, T>& operator-=(const Vector<3, T>& rhs)
//    {
//        *this = *this - rhs;
//        return *this;
//    }
//
//    const T* getValuePtr() const
//    {
//        return coords;
//    }
//
//    const glm::vec<3, T, glm::defaultp> toGLM() const
//    {
//        return glm::vec<3, T, glm::defaultp>(x(), y(), z());
//    }
//
//    T x() const
//    {
//        return coords[0];
//    }
//
//    T y() const
//    {
//        return coords[1];
//    }
//
//    T z() const
//    {
//        return coords[2];
//    }
//};
//
//template<int C, int R, typename T>
//class Matrix
//{
//public:
//    glm::mat<C, R, T, glm::defaultp> value;
//
//    Matrix()
//    {
//        Matrix(0);
//    }
//
//    Matrix(T diag)
//    {
//        value = glm::mat<C, R, T, glm::defaultp>(diag);
//    }
//
//    const float* getValuePtr() const
//    {
//        return glm::value_ptr(value);
//    }
//
//    Matrix(glm::mat<C, R, T, glm::defaultp> value_) : value(value_)
//    {}
//
//    Matrix& operator=(const Matrix& rhs)
//    {
//        value = rhs.value;
//        return *this;
//    }
//};