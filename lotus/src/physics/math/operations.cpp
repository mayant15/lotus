#include "lotus/physics.h"

namespace Lotus
{
    Vector3f LNormalize(const Vector3f& vec)
    {
        return glm::normalize(vec.toGLM());
    }

    Vector3f LCross(const Vector3f& a, const Vector3f& b)
    {
        return glm::cross(a.toGLM(), b.toGLM());
    }

    Matrix4f LLookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up)
    {
        return glm::lookAt(eye.toGLM(), center.toGLM(), up.toGLM());
    }

    Matrix4f LPerspective(float fov, float aspectRatio, float zNear, float zFar)
    {
        return glm::perspective(fov, aspectRatio, zNear, zFar);
    }

    Matrix4f LTranslate(const Matrix4f& mat, const Vector3f& translationVec)
    {
        return glm::translate(mat.value, translationVec.toGLM());
    }

    Matrix4f LScale(const Matrix4f& mat, const Vector3f& scaleVec)
    {
        return glm::scale(mat.value, scaleVec.toGLM());
    }

    Matrix4f LRotate(Matrix4f mat, float deg, Vector3f axis)
    {
        return glm::rotate(mat.value, glm::radians(deg), axis.toGLM());
    }
}