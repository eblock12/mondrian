#include "object.h"
#include "engine.h"

Object::Object()
{
}

Object::~Object()
{
}

void Object::SelfTerminate()
{
    Engine::GetSingleton().AddExpiredObject(this);
}

void Object::Tick(float timeDelta)
{
}