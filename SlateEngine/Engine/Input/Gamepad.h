#pragma once

#define NOMINMAX
#include <Windows.h>
#include <Xinput.h>
#include <SlateEngine/Engine/Math/vec2f.h>

class ENGINE_API Gamepad
{
public:
    Gamepad(unsigned int id);
    Gamepad(unsigned int id, float deadzoneX, float deadzoneY);

    inline unsigned int getControllerID() const { return controllerId; };

    XINPUT_GAMEPAD* GetGamepad();

    bool IsConnected();
    bool Update();
    void Vibrate(unsigned short leftSpeed, unsigned short rightSpeed);
    void Vibrate(unsigned short speed);
    void ResetVibration();
    bool IsButtonPressed(unsigned int button) const;

    union {
        float leftStickRaw[2];
        struct { float x; float y; } leftStick;
    };

    union {
        float rightStickRaw[2];
        struct { float x; float y; } rightStick;
    };

    float leftTrigger;
    float rightTrigger;

private:
    unsigned int controllerId;
    XINPUT_STATE state{};
    XINPUT_VIBRATION vibration{};


    float deadzoneX;
    float deadzoneY;

    float ApplyDeadzone(float value, float maxValue, float deadzone);
};