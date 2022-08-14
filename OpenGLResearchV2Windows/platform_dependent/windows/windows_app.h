#pragma once

#include <game_engine/app.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine 
{
class WindowsApp : public App, public WithoutGeneratedMethods
{
    bool m_isExitRequested = false;

public:
    virtual void requestExit();

    bool isExitRequested() const { return m_isExitRequested; }
};
}
