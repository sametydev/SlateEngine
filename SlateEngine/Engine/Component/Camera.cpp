#include <SlateEngine/Engine/Component/Camera.h>

Camera::Camera(float fov, float aspectRatio, float _near, float _far)
{
	mFov = fov;
	zNear = _near;
	zFar = _far;
	mAspect = aspectRatio;

	mProj = mat4x4::perspectiveLH(mFov, mAspect, zNear, zFar);
}

void Camera::SetFOV(float value)
{
	mFov = value;
}

void Camera::SetAspectRatio(float value)
{
	mAspect = value;
	mProj = mat4x4::perspectiveLH(mFov, mAspect, zNear, zFar);
}

void Camera::Update(float dt)
{
	mat4x4 R = mat4x4::RotationYawPitchRoll(mRot.y, mRot.x, mRot.z);

	//right
	mRight    = { R[0][0], R[1][0], R[2][0] };
	//up
	mUp       = { R[0][1], R[1][1], R[2][1] };
	//foward
	mForward  = { R[0][2], R[1][2], R[2][2] };

	mRight.normalize();
	mUp.normalize();
	mForward.normalize();

	mLocal = {
		mRight.x, mUp.x, mForward.x, mPos.x,
		mRight.y, mUp.y, mForward.y, mPos.y,
		mRight.z, mUp.z, mForward.z, mPos.z,
		0.f, 0.f, 0.f, 1.0f
	};
}

void Camera::SetPosition(const vec3f& pos)
{
	mPos = pos;
}



mat4x4 Camera::GetViewMatrix()
{
	return mLocal.inverted();
}

mat4x4& Camera::GetProjectionMatrix()
{
	return mProj;
}
