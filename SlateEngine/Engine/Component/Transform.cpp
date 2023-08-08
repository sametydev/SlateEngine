#include <SlateEngine/Engine/Component/Transform.h>

XMFLOAT3 Transform::GetScale() const
{
    return m_scale;
}

XMFLOAT3 Transform::GetRotation() const
{
    return m_rotation;
}

XMFLOAT3 Transform::GetPosition() const
{
    return m_position;
}

XMFLOAT3 Transform::GetRight() const
{
    XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_rotation));
    XMFLOAT3 right;
    XMStoreFloat3(&right, R.r[0]);
    return right;
}

XMFLOAT3 Transform::GetUp() const
{
    XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_rotation));
    XMFLOAT3 up;
    XMStoreFloat3(&up, R.r[1]);
    return up;
}

XMFLOAT3 Transform::GetForward() const
{
    XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_rotation));
    XMFLOAT3 forward;
    XMStoreFloat3(&forward, R.r[2]);
    return forward;
}

void Transform::SetScale(const XMFLOAT3& scale)
{
    m_scale = scale;
}

void Transform::RotateEulerAnglesRadian(const XMFLOAT3& _m)
{
    XMVECTOR newRotationVec = XMVectorAdd(XMLoadFloat3(&m_rotation),
        XMLoadFloat3(&_m));
    XMStoreFloat3(&m_rotation, newRotationVec);
}

void Transform::RotateAxis(const XMFLOAT3& axis, float radian)
{
    XMVECTOR rotationVec = XMLoadFloat3(&m_rotation);
    XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(rotationVec) *
        XMMatrixRotationAxis(XMLoadFloat3(&axis), radian);
    XMFLOAT4X4 rotMatrix;
    XMStoreFloat4x4(&rotMatrix, R);
    m_rotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}

void Transform::RotateAround(const XMFLOAT3& point, const XMFLOAT3& axis, float radian)
{
    XMVECTOR rotationVec = XMLoadFloat3(&m_rotation);
    XMVECTOR positionVec = XMLoadFloat3(&m_position);
    XMVECTOR centerVec = XMLoadFloat3(&point);

    XMMATRIX RT = XMMatrixRotationRollPitchYawFromVector(rotationVec) * XMMatrixTranslationFromVector(positionVec - centerVec);
    RT *= XMMatrixRotationAxis(XMLoadFloat3(&axis), radian);
    RT *= XMMatrixTranslationFromVector(centerVec);
    XMFLOAT4X4 rotMatrix;
    XMStoreFloat4x4(&rotMatrix, RT);
}

void Transform::SetPosition(const XMFLOAT3& position)
{
    m_position = position;
}

void Transform::Translate(const XMFLOAT3& direction, float magnitude)
{
    XMVECTOR directionVec = XMVector3Normalize(XMLoadFloat3(&direction));
    XMVECTOR newPosition = XMVectorMultiplyAdd(XMVectorReplicate(magnitude), directionVec, XMLoadFloat3(&m_position));
    XMStoreFloat3(&m_position, newPosition);
}

void Transform::LookAt(const XMFLOAT3& target, const XMFLOAT3& up)
{
    XMMATRIX View = XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&target), XMLoadFloat3(&up));
    XMMATRIX InvView = XMMatrixInverse(nullptr, View);
    XMFLOAT4X4 rotMatrix;
    XMStoreFloat4x4(&rotMatrix, InvView);
    m_rotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}

void Transform::LookTo(const XMFLOAT3& direction, const XMFLOAT3& up)
{
    XMMATRIX View = XMMatrixLookToLH(XMLoadFloat3(&m_position), XMLoadFloat3(&direction), XMLoadFloat3(&up));
    XMMATRIX InvView = XMMatrixInverse(nullptr, View);
    XMFLOAT4X4 rotMatrix;
    XMStoreFloat4x4(&rotMatrix, InvView);
    m_rotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}


XMFLOAT3 Transform::GetEulerAnglesFromRotationMatrix(const XMFLOAT4X4& rotationMatrix)
{
    XMFLOAT3 rotation{};

    if (fabs(1.0f - fabs(rotationMatrix(2, 1))) < 1e-5f)
    {
        rotation.x = copysignf(XM_PIDIV2, -rotationMatrix(2, 1));
        rotation.y = -atan2f(rotationMatrix(0, 2), rotationMatrix(0, 0));
        return rotation;
    }

    float c = sqrtf(1.0f - rotationMatrix(2, 1) * rotationMatrix(2, 1));

    if (isnan(c))
        c = 0.0f;

    rotation.z = atan2f(rotationMatrix(0, 1), rotationMatrix(1, 1));
    rotation.x = atan2f(-rotationMatrix(2, 1), c);
    rotation.y = atan2f(rotationMatrix(2, 0), rotationMatrix(2, 2));
    return rotation;
}

XMFLOAT4X4 Transform::GetLocalToWorldMatrix() const
{
    XMFLOAT4X4 res;
    XMStoreFloat4x4(&res, GetLocalToWorldMatrixXM());
    return res;
}

XMMATRIX Transform::GetLocalToWorldMatrixXM() const
{
    XMVECTOR scaleVec = XMLoadFloat3(&m_scale);
    XMVECTOR rotationVec = XMLoadFloat3(&m_rotation);
    XMVECTOR positionVec = XMLoadFloat3(&m_position);
    XMMATRIX World = XMMatrixScalingFromVector(scaleVec) * XMMatrixRotationRollPitchYawFromVector(rotationVec) * XMMatrixTranslationFromVector(positionVec);
    return World;
}

XMFLOAT4X4 Transform::GetWorldToLocalMatrix() const
{
    XMFLOAT4X4 res;
    XMStoreFloat4x4(&res, GetWorldToLocalMatrixXM());
    return res;
}

XMMATRIX Transform::GetWorldToLocalMatrixXM() const
{
    XMMATRIX InvWorld = XMMatrixInverse(nullptr, GetLocalToWorldMatrixXM());
    return InvWorld;
}