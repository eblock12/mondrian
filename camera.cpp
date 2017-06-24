#include "camera.h"
#include "application.h"

Camera::Camera()
    : positionX(0.0f), positionY(0.0f), velocityX(0.0f), velocityY(0.0f), focusX(0.0f), focusY(0.0f)
{
}

Camera::~Camera()
{
}

void Camera::Tick(float timeDelta)
{
    float width  = (float)Application::GetSingleton().GetWidth();
    float height = (float)Application::GetSingleton().GetHeight();
    
    if (focusX - positionX > width - width / 3)
        positionX = focusX - width + width / 3;
    else if (focusX - positionX < width - width * (2.0f / 3.0f))
        positionX = focusX - width + width * (2.0f / 3.0f);

    if (focusY - positionY > height - height * 0.15f)
        positionY = focusY - height + height * 0.15f;
    else if (focusY - positionY < height - height * 0.6f)
        positionY = focusY - height + height * 0.6f;
       
    positionX += velocityX * timeDelta;
    positionY += velocityY * timeDelta;

    if (positionX < 0.0f)
        positionX = 0.0f;
    if (positionY < 0.0f)
        positionY = 0.0f;
}