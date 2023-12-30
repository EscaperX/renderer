#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cc
{
	class GLObject
	{
	public:
		auto id() const -> uint32_t
		{
			return m_id;
		}
		uint32_t m_id = 0;
	};
}