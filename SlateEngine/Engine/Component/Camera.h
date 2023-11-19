#pragma once
#include <SlateEngine/Engine/Math/LineerMath.h>

class Camera
{
	enum Axis {
		Right = 0, Up, Forward
	};
public:
	Camera(float fov, float aspectRatio, float _near, float _far);

	void SetFOV(float value);
	void SetAspectRatio(float value);
	void Update(float dt);

	void SetPosition(const vec3f& pos);
	vec3f& GetPos() { return mPos; }

	mat4x4& GetViewMatrix();
	mat4x4& GetProjectionMatrix();

	float mFov = 45.f;
	float zNear = 0.01f;
	float zFar = 100.f;
	float mAspect = 0;

	vec3f mPos;
	vec3f mRot;

	mat4x4 mView;
	mat4x4 mProj;

	mat4x4 mLocal;
	mat4x4 mLocalInv;

	vec3f mRight;
	vec3f mUp;
	vec3f mForward;

	float mYaw = 0.0f;
	float mPitch = 0.0f;

};

