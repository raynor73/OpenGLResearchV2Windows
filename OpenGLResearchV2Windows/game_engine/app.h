#pragma once

namespace GameEngine
{
class App
{
public:
    virtual ~App() = default;

    virtual void requestExit() = 0;
};
}
