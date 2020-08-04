#include "lotus/physics.h"
#include "lotus/internal/glm/gtc/type_ptr.hpp"

namespace Lotus
{
    Vector3f LNormalize(const Vector3f& vec)
    {
        return glm::normalize(vec);
    }

    Vector3f LCross(const Vector3f& a, const Vector3f& b)
    {
        return glm::cross(a, b);
    }

    Matrix4f LLookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up)
    {
        return glm::lookAt(eye, center, up);
    }

    Matrix4f LPerspective(float fov, float aspectRatio, float zNear, float zFar)
    {
        return glm::perspective(fov, aspectRatio, zNear, zFar);
    }

    Matrix4f LTranslate(const Matrix4f& mat, const Vector3f& translationVec)
    {
        return glm::translate(mat, translationVec);
    }

    Matrix4f LScale(const Matrix4f& mat, const Vector3f& scaleVec)
    {
        return glm::scale(mat, scaleVec);
    }

    Matrix4f LRotate(Matrix4f mat, float deg, Vector3f axis)
    {
        return glm::rotate(mat, glm::radians(deg), axis);
    }

    const float* valuePtr(const Matrix4f& mat){
        return glm::value_ptr(mat);
    }

    const float* valuePtr(const Vector3f& vec)
    {
        return glm::value_ptr(vec);
    }
}