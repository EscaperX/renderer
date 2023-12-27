#pragma once
#include <vector>
#include <optional>
#include <string>
#include <typeindex>

namespace CloseGL
{
    struct VertexAttributeDescriptor final
    {
        std::string name;
    };
    class VertexArrayObject
    {
    private:
        std::vector<VertexAttributeDescriptor> attributes;
        // std::optional<std::vector<>> index_buffer;
    };
}