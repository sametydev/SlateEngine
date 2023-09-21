#include "Gamepad.h"
#include <algorithm>
#include <climits>

float normalize(float input, float min, float max);

Gamepad::Gamepad(unsigned int id) : controllerId(id),
deadzoneX(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE),
deadzoneY(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
{

}

Gamepad::Gamepad(unsigned int id, float deadzoneX, float deadzoneY)
    : deadzoneX(deadzoneX), deadzoneY(deadzoneY)
{
}

XINPUT_GAMEPAD* Gamepad::GetGamepad()
{
    return &state.Gamepad;
}

bool Gamepad::IsConnected()
{
    return XInputGetState(controllerId - 1, &state) == ERROR_SUCCESS;
}

float normalize(float input, float min, float max)
{
    float average = (min + max) / 2;
    float range   = (max - min) / 2;
    return (input - average) / range;
}

float Gamepad::ApplyDeadzone(float value,float maxValue, float deadzone)
{
    if (value < -deadzone)
    {
        value += deadzone;
    }
    else if (value > deadzone)
    {
        value -= deadzone;
    }
    else
    {
        return 0;
    }

    float normValue = (float)value / (float)(maxValue - deadzone);

    return std::max(-1.0f, std::min(normValue, maxValue));
}

bool Gamepad::Update()
{
    if (!IsConnected())
        return false;

    float normLX = normalize(static_cast<float>(state.Gamepad.sThumbLX), -32767, 32767);
    float normLY = normalize(static_cast<float>(state.Gamepad.sThumbLY), -32767, 32767);

    float normRX = normalize(static_cast<float>(state.Gamepad.sThumbRX), -32767, 32767);
    float normRY = normalize(static_cast<float>(state.Gamepad.sThumbRY), -32767, 32767);

    if (deadzoneX <= 1.0f || deadzoneY <= 1.0f)
    {
        leftStick.x  = ApplyDeadzone(normLX, 1.0f, deadzoneX);
        leftStick.y  = ApplyDeadzone(normLY, 1.0f, deadzoneY);
        rightStick.x = ApplyDeadzone(normRX, 1.0f, deadzoneX);
        rightStick.y = ApplyDeadzone(normRY, 1.0f, deadzoneY);
    }
    else
    {
        leftStick.x  = ApplyDeadzone(normLX, 1.0f, normalize(deadzoneX, SHRT_MIN, SHRT_MAX));
        leftStick.y  = ApplyDeadzone(normLY, 1.0f, normalize(deadzoneY, SHRT_MIN, SHRT_MAX));
        rightStick.x = ApplyDeadzone(normRX, 1.0f, normalize(deadzoneX, SHRT_MIN, SHRT_MAX));
        rightStick.x = ApplyDeadzone(normRY, 1.0f, normalize(deadzoneY, SHRT_MIN, SHRT_MAX));
    }

    leftTrigger  = static_cast<float>(state.Gamepad.bLeftTrigger) / 255.0f;
    rightTrigger = static_cast<float>(state.Gamepad.bRightTrigger) / 255.0f;

    return true;
}

void Gamepad::Vibrate(unsigned short leftSpeed, unsigned short rightSpeed)
{
    vibration.wLeftMotorSpeed  = leftSpeed;
    vibration.wRightMotorSpeed = rightSpeed;
    XInputSetState(controllerId - 1, &vibration);
}

void Gamepad::Vibrate(unsigned short speed)
{
    vibration.wLeftMotorSpeed  = speed;
    vibration.wRightMotorSpeed = speed;
    XInputSetState(controllerId - 1, &vibration);
}

void Gamepad::ResetVibration()
{
    vibration.wLeftMotorSpeed  = 0;
    vibration.wRightMotorSpeed = 0;
    XInputSetState(controllerId - 1, &vibration);
}

bool Gamepad::IsButtonPressed(unsigned int button) const
{
    return (state.Gamepad.wButtons & button) != 0;
}