
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

	private:
		GLuint _object;

		Program(const Program&);
		const Program& operator = (const Program&);
	};
}