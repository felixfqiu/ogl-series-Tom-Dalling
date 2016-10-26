
#include "Program.h"

using namespace tdogl;

Program::Program(const std::vector<Shader>& shaders) :
	_object(0)
{
}

Program::~Program()
{
	if (_object != 0)
	{
		glDeleteProgram(_object);
	}
}

GLuint Program::object() const
{
	return _object;
}