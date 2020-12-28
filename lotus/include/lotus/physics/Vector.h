/**
 * This header file defines a few wrappers over glm that should be used by the end user.
 */

#pragma once

// TODO: Remove glm here to make it a dev dependency only
#include "lotus/internal/glm/glm.hpp"
#include "lotus/internal/glm/gtc/matrix_transform.hpp"
#include "lotus/internal/glm/gtx/matrix_decompose.hpp"
#include "lotus/internal/glm/gtc/type_ptr.hpp"

#include "lotus/lcommon.h"

/**
 * Convenience typedefs
 */

typedef glm::ivec2 Vector2i;
typedef glm::ivec3 Vector3i;
typedef glm::ivec4 Vector4i;
typedef glm::vec2 Vector2f;
typedef glm::vec3 Vector3f;
typedef glm::vec4 Vector4f;

namespace glm
{
    // TODO: Maybe do some pointer memcpy magic to generalize this to vectors of all dimensions
    inline void to_json(nlohmann::json& data, const Vector3f& vec)
    {
        data = nlohmann::json::array();
        data[0] = vec.x;
        data[1] = vec.y;
        data[2] = vec.z;
    }

    inline void from_json(const nlohmann::json& data, Vector3f& vec)
    {
        vec.x = data[0];
        vec.y = data[1];
        vec.z = data[2];
    }

    inline void to_json(nlohmann::json& data, const glm::vec4& vec)
    {
        data = nlohmann::json::array();
        data[0] = vec.r;
        data[1] = vec.g;
        data[2] = vec.b;
        data[3] = vec.a;
    }

    inline void from_json(const nlohmann::json& data, glm::vec4& vec)
    {
        vec.r = data[0];
        vec.g = data[1];
        vec.b = data[2];
        vec.a = data[3];
    }
}

typedef glm::imat2x2 Matrix2i;
typedef glm::imat3x3 Matrix3i;
typedef glm::imat4x4 Matrix4i;
typedef glm::mat2 Matrix2f;
typedef glm::mat3 Matrix3f;
typedef glm::mat4 Matrix4f;

/**
 * Convenience macros
 */

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

namespace Lotus
{
    /**
     * Returns a unit vector in the direction of the passed vector.
     * @param vec Vector to be normalized
     * @return Unit vector
     */
    inline Vector3f LNormalize(const Vector3f& vec)
    {
        return glm::normalize(vec);
    }

    /**
     * Calculate the cross product of two vectors.
     * @param a First vector
     * @param b Second vector
     * @return Result of the cross product
     */
    inline Vector3f LCross(const Vector3f& a, const Vector3f& b)
    {
        return glm::cross(a, b);
    }

    /**
     * Calculate a transformation matrix to that will make an object look towards a particular point.
     * @param eye Position of the "eye" of the object
     * @param center Point to look at
     * @param up Direction representing "up" for the object
     * @return Transformation matrix
     */
    inline Matrix4f LLookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up)
    {
        return glm::lookAt(eye, center, up);
    }

    /**
     * Calculate a transformation matrix to bring the object into clip space with a perspective projection.
     * @param fov Half-angle field of view in degrees
     * @param aspectRatio Aspect ratio of the projection
     * @param zNear Frustum near cut-off
     * @param zFar Frustum far cut-off
     * @return Transformation matrix
     */
    inline Matrix4f LPerspective(float fov, float aspectRatio, float zNear, float zFar)
    {
        return glm::perspective(fov, aspectRatio, zNear, zFar);
    }

    /**
     * Add a translation to a transformation matrix.
     * @param mat Matrix to translate
     * @param translationVec Vector to translate along. Magnitude represents distance.
     * @return Updated transformation matrix
     */
    inline Matrix4f LTranslate(const Matrix4f& mat, const Vector3f& translationVec)
    {
        return glm::translate(mat, translationVec);
    }

    /**
     * Add a scale to a transformation matrix
     * @param mat Matrix to scale
     * @param scaleVec Scale along X, Y and Z
     * @return Updated transformation matrix
     */
    inline Matrix4f LScale(const Matrix4f& mat, const Vector3f& scaleVec)
    {
        return glm::scale(mat, scaleVec);
    }

    /**
     * Add a rotation to a transformation matrix
     * @param mat Matrix to rotate
     * @param deg Angle to rotate in degrees
     * @param axis Axis to rotate along
     * @return Updated transformation matrix
     */
    inline Matrix4f LRotate(Matrix4f mat, float deg, Vector3f axis)
    {
        return glm::rotate(mat, glm::radians(deg), axis);
    }

    /**
     * Get a pointer to the matrix data. Required because of GLM internals.
     * @param mat Matrix to get the pointer to
     * @return Float pointer to the matrix data
     */
    inline const float* valuePtr(const Matrix4f& mat){
        return glm::value_ptr(mat);
    }

    /**
     * Get a pointer to the vector data. Required because of GLM internals.
     * @param vec Vector to get the pointer to
     * @return Float pointer to the vector data
     */
    inline const float* valuePtr(const Vector3f& vec)
    {
        return glm::value_ptr(vec);
    }
}
