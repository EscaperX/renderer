#include <cstdint>

namespace cc
{
    enum class StateObjectType
    {
        Buffer,
        VertexArray,
        FrameBuffer,
        RenderBuffer,
        Shader,
        Texture
    };

    class StateObject
    {
    public:
        StateObject() : m_id(0) {}
        ~StateObject() {}
        auto id() const -> uint32_t { return m_id; }
        auto mutable_id() -> uint32_t & { return m_id; }
        uint32_t m_id;
    };
}