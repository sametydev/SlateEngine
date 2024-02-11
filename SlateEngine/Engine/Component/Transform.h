#pragma once
#include <SlateEngine/Engine/Math/LineerMath.h>
#include <SlateEngine/Engine/Component/Component.h>
class Transform : public Component
{
public:
	Transform();
	~Transform() {};


	void OnInternalInit() override;

	void SetPosition(const vec3f& position);
	void SetRotation(const vec3f& rotation);

	void SetScale(const vec3f& scale);

	mat4x4& GetGlobal();
	mat4x4& GetLocal();

	Transform* mParent = nullptr;

	void SetParent(Transform* parent);
	bool HasParent();

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnInit() override;
	void OnShutdown() override;

	vec3f mPosition;
	vec3f mRotation;
	vec3f mScale;
private:
	bool hasParent = false;
	bool mIsAutoUpdate = false;

	mat4x4 mGlobal;
	mat4x4 mLocal;

	union {
		struct { vec3f mAxis[3]; };
		struct { vec3f mRight, mUp, mForward; };
	};

};