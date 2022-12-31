#pragma once
class ResearchScene
{
public:
    virtual void start() = 0;
    virtual void update() = 0;

    virtual ~ResearchScene() = default;
};

