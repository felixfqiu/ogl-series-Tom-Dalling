
#include "Program.h"

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