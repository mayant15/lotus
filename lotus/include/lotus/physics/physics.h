/**
 * This header file defines a few wrappers over glm that should be used by the end user.
 */

#pragma once

// TODO: Remove glm here to make it a dev dependency only
#include "lotus/internal/glm/glm.hpp"
#include "lotus/internal/glm/gtc/matrix_transform.hpp"
#include "lotus/internal/glm/gtx/matrix_decompose.hpp"
#include "lotus/internal/glm/gtc/type_ptr.hpp"

/**
 * Convenience typedefs
 */

typedef glm::ivec2 Vector2i;
typedef glm::ivec3 Vector3i;
typedef glm::ivec4 Vector4i;
typedef glm::vec2 Vector2f;
typedef glm::vec3 Vector3f;
typedef glm::vec4 Vector4f;

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
    Vector3f LNormalize(const Vector3f& vec);

    /**
     * Calculate the cross product of two vectors.
     * @param a First vector
     * @param b Second vector
     * @return Result of the cross product
     */
    Vector3f LCross(const Vector3f& a, const Vector3f& b);

    /**
     * Calculate a transformation matrix to that will make an object look towards a particular point.
     * @param eye Position of the "eye" of the object
     * @param center Point to look at
     * @param up Direction representing "up" for the object
     * @return Transformation matrix
     */
    Matrix4f LLookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up);

    /**
     * Calculate a transformation matrix to bring the object into clip space with a perspective projection.
     * @param fov Half-angle field of view in degrees
     * @param aspectRatio Aspect ratio of the projection
     * @param zNear Frustum near cut-off
     * @param zFar Frustum far cut-off
     * @return Transformation matrix
     */
    Matrix4f LPerspective(float fov, float aspectRatio, float zNear, float zFar);

    /**
     * Add a translation to a transformation matrix.
     * @param mat Matrix to translate
     * @param translationVec Vector to translate along. Magnitude represents distance.
     * @return Updated transformation matrix
     */
    Matrix4f LTranslate(const Matrix4f& mat, const Vector3f& translationVec);

    /**
     * Add a scale to a transformation matrix
     * @param mat Matrix to scale
     * @param scaleVec Scale along X, Y and Z
     * @return Updated transformation matrix
     */
    Matrix4f LScale(const Matrix4f& mat, const Vector3f& scaleVec);

    /**
     * Add a rotation to a transformation matrix
     * @param mat Matrix to rotate
     * @param deg Angle to rotate in degrees
     * @param axis Axis to rotate along
     * @return Updated transformation matrix
     */
    Matrix4f LRotate(Matrix4f mat, float deg, Vector3f axis);

    /**
     * Get a pointer to the matrix data. Required because of GLM internals.
     * @param mat Matrix to get the pointer to
     * @return Float pointer to the matrix data
     */
    const float* valuePtr(const Matrix4f& mat);

    /**
     * Get a pointer to the vector data. Required because of GLM internals.
     * @param vec Vector to get the pointer to
     * @return Float pointer to the vector data
     */
    const float* valuePtr(const Vector3f& vec);

    enum class EPhysicsShape
    {
        SPHERE,
        CAPSULE,
        BOX
    };

    // TODO: Implement other shapes
    struct CCollider
    {
        EPhysicsShape Shape;
        float Radius = 1.0f;
        Vector3f Position = Vector3f {1.0f};
    };

    /**
     * @attention You must add a collider and a transform before adding a rigidbody component
     */
    struct CRigidBody
    {
        float Gravity = 1.0f;
        bool IsKinematic = false;
    };
}
