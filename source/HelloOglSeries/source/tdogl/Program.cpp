
#include "Program.h"
#include <assert.h>

using namespace tdogl;

Program::Program(const std::vector<Shader>& shaders) :
	_object(0)
{
	if (shaders.size() <= 0)
	{
		throw std::runtime_error("No shaders provided to create program");
	}

	/*/ // 10/26/2016 
	Program Objects[2.11.3] [2.14.3]
	
	uint CreateProgram(void);
	void AttachShader(uint program,	uint shader);
	void DetachShader(uint program,	uint shader);
	void LinkProgram(uint program);
	void UseProgram(uint program);
	uint CreateShaderProgramv(enum type, sizei count, const char **strings);
	
	void ProgramParameteri(uint program, enum pname, int value);
	pname: PROGRAM_SEPARABLE,
	PROGRAM_BINARY_{RETRIEVABLE_HINT},
	value: TRUE, FALSE
	//*/

	// create program object
	_object = glCreateProgram();
	if (_object == 0)
	{
		throw std::runtime_error("glCreateProgram failed");
	}

	// attach all shaders
	for (unsigned i=0; i<shaders.size(); i++)
	{
		glAttachShader(_object, shaders[i].object());
	}

	// link shaders together
	glLinkProgram(_object);

	// detach all shaders
	for (unsigned i = 0; i < shaders.size(); i++)
	{
		glDetachShader(_object, shaders[i].object());
	}

	/*/ // 10/26/2016 
	Program Queries [6.1.12] [6.1.18]

	void GetAttachedShaders(uint program, sizei	maxCount, sizei *count, uint *shaders);

	void GetVertexAttrib{d f i}v(uint index, enum pname, T *params);
	pname: CURRENT_VERTEX_ATTRIB or
	VERTEX_ATTRIB_ARRAY_x where xis one of
	BUFFER_BINDING, DIVISOR, ENABLED, INTEGER,
	NORMALIZED, SIZE, STRIDE, or TYPE
	
	void GetVertexAttribl{i ui}v(uint index, enum pname, T *params);
	pname: see GetVertexAttrib{d f i}v
	void GetVertexAttribLdv(uint index,
	enum pname, double *params);
	pname: see GetVertexAttrib{d f i}v
	
	void GetVertexAttribPointerv(uint index, enum pname, void **pointer);
	pname: VERTEX_ATTRIB_ARRAY_POINTER
	
	void GetUniform{f d i ui}v(uint program, int location, T *params);
	void GetUniformSubroutineuiv(enum shadertype, int location, uint *params);
	boolean IsProgram(uint program);

	void GetProgramiv(uint program,	enum pname, int *params);
	pname: DELETE_STATUS, LINK_STATUS,
	VALIDATE_STATUS, INFO_LOG_LENGTH,
	ATTACHED_SHADERS, ACTIVE_ATTRIBUTES,
	ACTIVE_ UNIFORMS{_BLOCKS},
	ACTIVE_ATTRIBUTES_MAX_LENGTH,
	ACTIVE_UNIFORM_MAX_LENGTH,
	TRANSFORM_FEEDBACK_BUFFER_MODE,
	TRANSFORM_FEEDBACK_ VARYINGS,
	TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH,
	ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH,
	GEOMETRY_VERTICES_OUT,
	GEOMETRY_{INPUT, OUTPUT}_TYPE,
	GEOMETRY_SHADER_INVOCATIONS,
	TESS_CONTROL_OUTPUT_VERTICES,
	TESS_GEN_{MODE, SPACING, VERTEX_ORDER},
	TESS_GEN_POINT_MODE, PROGRAM_SEPARABLE,
	PROGRAM_BINARY_{LENGTH, RETRIEVABLE_HINT}
	
	boolean IsProgramPipeline(uint pipeline);
	void GetProgramPipelineiv(uint pipeline, enum pname, int *params);
	void GetProgramInfoLog(uint program, sizei bufSize, sizei *length, char *infoLog);
	void GetProgramPipelineInfoLog(uint pipeline, sizei bufSize, sizei *length, char *infoLog);
	//*/

	// throw exception if linking failed
	GLint status;
	glGetProgramiv(_object, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::string msg("Program linking failure: ");

		GLint infoLogLength;
		glGetProgramiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* strInfoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(_object, infoLogLength, NULL, strInfoLog);

		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteProgram(_object);
		_object = 0;

		throw std::runtime_error(msg);
	}
}

Program::~Program()
{
	//might be 0 if ctor fails by throwing exception
	if (_object != 0)
	{
		glDeleteProgram(_object);
	}
}

GLuint Program::object() const
{
	return _object;
}

bool Program::isInUse() const
{
	GLint currentProgram = 0;
	/*/ // 11/3/2016 State and State Requests
	void GetIntegerv(enum pname, int *data);
	//*/
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

	return currentProgram == (GLint)_object;
}

GLuint Program::attrib(const GLchar* attribName) const
{
	if (attribName == NULL)
	{
		throw std::runtime_error("attribName was NULL");
	}

	/*/ // 10/26/2016 
	Vertex Attributes[2.11.6] [2.14.6]

	Vertex shaders operate on array of 4-component
	items numbered from slot 0 to
	MAX_VERTEX_ATTRIBS - 1.
	
	void GetActiveAttrib(uint program, uint index, sizei bufSize, sizei *length, int *size, enum *type, char *name);
	*type returns: FLOAT_{VECn, MATn, MATnxm},
	FLOAT, {UNSIGNED_}INT,
	{UNSIGNED_} INT_VECn

	int GetAttribLocation(uint program,	const char *name);
	void BindAttribLocation(uint program, uint index, const char *name);
	//*/
	GLuint attrib = glGetAttribLocation(_object, attribName);
	if (attrib == -1)
	{
		throw std::runtime_error(std::string("Program attribute not found: ") + attribName);
	}

	return attrib;
}


GLint Program::uniform(const GLchar* uniformName) const
{
	if (uniformName == NULL)
	{
		throw std::runtime_error("uniformName was NULL");
	}

	/*/ // 11/3/2016 
	int GetUniformLocation(uint program, const char *name);
	GetActiveUniformName
	GetUniformIndices
	GetActiveUniform
	GetActiveUniformsiv
	GetUniformBlockIndex
	GetActiveUniformBlockName
	GetActiveUniformBlockiv
	GetActiveAtomicCounterBufferiv
	//*/
	GLint uniform = glGetUniformLocation(_object, uniformName);
	if (uniform == -1)
	{
		throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);
	}

	return uniform;
}

void Program::setUniform(const GLchar* uniformName, int v0)
{
	assert(isInUse());

	/*/ // 11/3/2016 
	void Uniform{1234}{i f d}(int location, T value);
	//*/
	glUniform1d(uniform("tex"), v0);
}