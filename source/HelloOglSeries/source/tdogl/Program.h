
#pragma once

#include "Shader.h"
#include <vector>

namespace tdogl
{
	class Program
	{
	public:
		Program(const std::vector<Shader>& shaders);
		~Program();

		GLuint object() const;

		bool isInUse() const;

		GLuint attrib(const GLchar* attribName) const;

		GLint uniform(const GLchar* uniformName) const;
		void setUniform(const GLchar* uniformName, int v0);

	private:
		GLuint _object;

		Program(const Program&);
		const Program& operator = (const Program&);
	};
}