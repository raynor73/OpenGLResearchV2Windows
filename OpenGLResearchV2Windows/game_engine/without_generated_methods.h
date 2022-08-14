#pragma once

namespace GameEngine
{
class WithoutGeneratedMethods {

public:
    WithoutGeneratedMethods() = default;
    WithoutGeneratedMethods(const WithoutGeneratedMethods&) = delete;
    WithoutGeneratedMethods(WithoutGeneratedMethods&&) = delete;

    virtual ~WithoutGeneratedMethods() = default;

    WithoutGeneratedMethods& operator=(const WithoutGeneratedMethods&) = delete;
    WithoutGeneratedMethods& operator=(WithoutGeneratedMethods&&) = delete;
};
}
