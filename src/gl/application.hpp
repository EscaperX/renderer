#pragma once

namespace cc
{
    class Application final
    {
    public:
        ~Application();
        auto update() const -> void;
        auto render() const -> void;
    };
} // namespace cc
