#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cc
{
	class GLSLShader
	{
	public:
		class Source
		{
		public:
			std::string m_vertex_shader;
			std::string m_geometry_shader;
			std::string m_fragment_shader;
			std::string m_compute_shader;

			Source &vertex(std::string const &shader_path);
			Source &geometry(std::string const &shader_path);
			Source &fragment(std::string const &shader_path);
			Source &compute(std::string const &shader_path);

		private:
			std::string load_shader(std::string const &shader_path);
		};
		void use() const;
		void introspect() const;
		void uniform(const std::string &name, GLboolean value) const;
		void uniform(const std::string &name, GLuint value) const;
		void uniform(const std::string &name, GLfloat value) const;
		void uniform(const std::string &name, GLdouble value) const;
		void uniform(const std::string &name, const glm::vec2 &value) const;
		void uniform(const std::string &name, const glm::vec3 &value) const;
		void uniform(const std::string &name, const glm::vec4 &value) const;
		void uniform(const std::string &name, const glm::mat3 &value) const;
		void uniform(const std::string &name, const glm::mat4 &value) const;

		GLSLShader(Source const &source);

	private:
		};

} // cc
