#pragma once

class Object
{
public:
    Object();
    virtual ~Object();
    void SelfTerminate();
    virtual void Tick(float timeDelta);
};