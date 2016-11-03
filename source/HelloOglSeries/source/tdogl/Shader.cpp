
#include "Shader.h"
#include <fstream>
#include <stdexcept>
#include <cassert>
#include <sstream>

using namespace tdogl;

Shader Shader::shaderFromFile(const std::string& filePath, GLenum shaderType)
{
	std::ifstream f;
	f.open(filePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		throw std::runtime_error(std::string("Fail to open file: ") + filePath);
	}

	std::stringstream buffer;
	buffer << f.rdbuf();

	Shader shader(buffer.str(), shaderType);
	return shader;
}

Shader::Shader(const std::string& shaderCode, GLenum shaderType) :
	_object(0),
	_refCount(NULL)
{
	/*/ // 10/26/2016 
	Shader Objects [2.11.1-2] [2.14.1-2]
	
	uint CreateShader(enum type);
	type: {VERTEX, FRAGMENT, GEOMETRY}_SHADER,
	TESS_{EVALUATION, CONTROL}_SHADER
	
	void ShaderSource(uint shader, sizei count,	const char **string, const int *length);
	void CompileShader(uint shader);
	void ReleaseShaderCompiler(void);
	void DeleteShader(uint shader);
	void ShaderBinary(sizei count, const uint *shaders, enum binaryformat, const void *binary, sizeilength);
	//*/
	_object = glCreateShader(shaderType);
	if (_object == 0)
	{
		throw std::runtime_error("glCreateShader failed");
	}

	const char* code = shaderCode.c_str();
	glShaderSource(_object, 1, (const GLchar**)&code, NULL);

	/*/ // 10/26/2016 
	Shader Queries[6.1.12] [6.1.18]

	boolean IsShader(uint shader);

	void GetShaderiv(uint shader, enum pname, int *params);
	pname: SHADER_TYPE, {GEOMETRY, VERTEX}_SHADER,
	TESS_{CONTROL, EVALUATION}_SHADER,
	FRAGMENT_SHADER, {DELETE, COMPILE}_STATUS,
	INFO_LOG_LENGTH, SHADER_SOURCE_LENGTH
	
	void GetShaderInfoLog(uint shader, sizei bufSize, sizei *length, char *infoLog);
	void GetShaderSource(uint shader, sizei bufSize, sizei *length, char *source);

	void GetShaderPrecisionFormat(enum shadertype, enum precisiontype, int *range, int *precision);
	shadertype: {VERTEX, FRAGMENT}_SHADER
	precisiontype: LOW_{FLOAT, INT},
	MEDIUM_{FLOAT, INT}, HIGH_{FLOAT, INT}

	void GetProgramStageiv(uint program, enum shadertype, enum pname, int *values);
	pname: ACTIVE_SUBROUTINES,
	ACTIVE_SUBROUTINE_{UNIFORMS, MAX_LENGTH},
	ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS,
	ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH
	//*/
	GLint status;
	glCompileShader(_object);
	glGetShaderiv(_object, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::string msg("Compile failure in shader:\n");

		GLint infoLogLength;
		glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* strInfoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(_object, infoLogLength, NULL, strInfoLog);

		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteShader(_object);
		_object = 0;

		throw std::runtime_error(msg);
	}

	_refCount = new unsigned;
	*_refCount = 1;
}

GLuint Shader::object() const
{
	return _object;
}

Shader::~Shader()
{
	// _refCount will be NULL if constructor failed and threw an exception
	if (_refCount)
	{
		_release();
	}
}

Shader::Shader(const Shader& other) :
	_object(other._object),
	_refCount(other._refCount)
{
	_retain();
}

Shader& Shader::operator = (const Shader& other)
{
	_release();
	_object = other._object;
	_refCount = other._refCount;
	_retain();
	return *this;
}

void Shader::_retain()
{
	assert(_refCount);
	*_refCount += 1;
}

void Shader::_release()
{
	assert(_refCount && *_refCount > 0);
	*_refCount -= 1;

	if (*_refCount == 0)
	{
		glDeleteShader(_object);
		_object = 0;
		
		delete _refCount;
		_refCount = NULL;
	}
}