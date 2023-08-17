#include <SlateEngine/Engine/Component/Camera.h>

Camera::Camera(float fov, float aspectRatio, float near, float far)
{
	mFov = fov;
	zNear = near;
	zFar = far;
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
}

void Camera::Update(float dt)
{
	constexpr float movementSpeed = 4.2f;
	constexpr float mouseSpeed = 0.19999f;


	mat4x4 R = mat4x4::RotationYawPitchRoll(mRot.y, mRot.x, mRot.z);
	//right
	vec3f r = { R[0][0], R[1][0], R[2][0] };
	//up
	vec3f u = { R[0][1], R[1][1], R[2][1] };
	//foward
	vec3f f = { R[0][2], R[1][2], R[2][2] };
	r.normalize();
	u.normalize();
	f.normalize();

	mLocal = {
		r.x, u.x, f.x, mPos.x,
		r.y, u.y, f.y, mPos.y,
		r.z, u.z, f.z, mPos.z,
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
