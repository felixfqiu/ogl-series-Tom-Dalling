
#pragma once

#include "Shader.h"
#include <vector>
#include <glm/glm.hpp>

namespace tdogl
{
	class Program
	{
	public:
		Program(const std::vector<Shader>& shaders);
		~Program();

		GLuint object() const;

		void use() const;
		void stopUsing() const;
		bool isInUse() const;

		GLuint attrib(const GLchar* name) const;

		GLint uniform(const GLchar* name) const;
		void setUniform(const GLchar* name, int v0);
		void setUniform(const GLchar* name, const glm::mat4& m, GLboolean transpose = GL_FALSE);

	private:
		GLuint _object;

		Program(const Program&);
		const Program& operator = (const Program&);
	};
}