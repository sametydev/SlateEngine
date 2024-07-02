#include <SlateEngine/Engine/Component/Transform.h>

Transform::Transform()
{
	mAxis[0] = vec3f(1.f, 0.f, 0.f);
	mAxis[0] = vec3f(0.f, 1.f, 0.f);
	mAxis[0] = vec3f(0.f, 0.f, 1.f);

	mScale = vec3f(1.0f);
}

void Transform::OnInternalInit()
{
}

void Transform::SetPosition(const vec3f& position)
{
	mPosition = position;
}

void Transform::SetRotation(const vec3f& rotation)
{
	mRotation = rotation;
}

void Transform::SetScale(const vec3f& scale)
{
	mScale = scale;
}

mat4x4& Transform::GetGlobal()
{
	return mGlobal;
}

mat4x4& Transform::GetLocal()
{
	return mLocal;
}

void Transform::SetParent(Transform* parent)
{
	if (parent == nullptr)
	{
		mParent = nullptr;
		hasParent = false;
		return;
	}

	mParent = parent;
	hasParent = true;
}

bool Transform::HasParent()
{
	return hasParent;
}


void Transform::OnRender(ID3D11DeviceContext* pDeviceContext)
{
}

void Transform::OnInit()
{
}

void Transform::OnShutdown()
{
}

void Transform::OnUpdate(float deltaTime)
{

	mat4x4 S = mat4x4::scaled(mScale);
	mat4x4 T = mat4x4::translated(mPosition);
	//mat4x4 R = mat4x4::RotationYawPitchRoll(mRotation.x,mRotation.y,mRotation.z);

	mat4x4 rx = mat4x4::rotateX(mRotation.x);
	mat4x4 ry = mat4x4::rotateY(mRotation.y);
	mat4x4 rz = mat4x4::rotateZ(mRotation.z);

	mat4x4 R = rx * rz * ry;

	//mLocal = S * R * T;
	mLocal = T * R * S;

	if (!hasParent)
	{
		mGlobal = mLocal;
	}
	else
	{
		mGlobal = this->mLocal * mParent->mGlobal;
	}
}