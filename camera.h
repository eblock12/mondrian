#pragma once

class Camera
{
public:
    Camera();
    ~Camera();
    void Accelerate(float timeDelta, float x, float y) { velocityX += x * timeDelta; velocityY += y * timeDelta; }
    void SetPosition(float x, float y) { positionX = x; positionY = y; }
    void SetPositionX(float x) { positionX = x; }
    void SetPositionY(float y) { positionY = y; }
    void SetVelocityX(float x) { velocityX = x; }
    void SetVelocityY(float y) { velocityY = y; }
    void Tick(float timeDelta);
    void SetFocus(float x, float y) { focusX = x; focusY = y; }
    float GetPositionX() { return positionX; }
    float GetPositionY() { return positionY; }
private:
    float focusX;
    float focusY;
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
};