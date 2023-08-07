#pragma once
#include <SlateEngine/Engine/DXConfig.h>

using namespace DirectX;

class Transform
{
public:
    Transform() = default;
    Transform(const XMFLOAT3& pos, const XMFLOAT3& rot, const XMFLOAT3& scale);
    ~Transform() = default;



    XMFLOAT3 GetPosition() const;
    XMFLOAT3 GetRotation() const;
    XMFLOAT3 GetScale() const;


    XMFLOAT3 GetRight() const;
    XMFLOAT3 GetUp() const;
    XMFLOAT3 GetForward() const;


    void SetScale(const XMFLOAT3& scale);
    void SetScale(float x, float y, float z);

    void SetRotationEulerAnglesRadian(const XMFLOAT3& _m);
    void SetRotation(float x, float y, float z);

    void SetPosition(const XMFLOAT3& position);
    void SetPosition(float x, float y, float z);

    void RotateEulerAnglesRadian(const XMFLOAT3& _m);

    void Translate(const XMFLOAT3& direction, float magnitude);

    void LookAt(const XMFLOAT3& target, const XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });
    void LookTo(const XMFLOAT3& direction, const XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });


    //Operators;
    Transform(const Transform&) = default;
    Transform& operator=(const Transform&) = default;

    Transform(Transform&&) = default;
    Transform& operator=(Transform&&) = default;

private:
    XMFLOAT3 m_scale = { 1.0f, 1.0f, 1.0f }; 
    XMFLOAT3 m_rotation = {};                
    XMFLOAT3 m_position = {};                
};
