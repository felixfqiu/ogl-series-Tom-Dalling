
#include "platform.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cassert>
#include <stdexcept>
#include <iostream>

#include "tdogl/Program.h"
#include "tdogl/Texture.h"

const glm::vec2 SCREEN_SIZE(800, 600);

GLFWwindow* gWindow = NULL;
tdogl::Texture* gTexture = NULL;
tdogl::Program* gProgram = NULL;
GLuint gVAO = 0;
GLuint gVBO = 0;

static void LoadShaderAndProgram()
{
	//
	// shader
	//
	std::vector<tdogl::Shader> shaders;
	shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("vertex-shader.txt"), GL_VERTEX_SHADER));
	shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("fragment-shader.txt"), GL_FRAGMENT_SHADER));

	//
	// program
	//
	gProgram = new tdogl::Program(shaders);
}

static void LoadTriangle()
{
	/*/ // 10/25/2016 
	Vertex Array Objects [2.10]

	All states related to definition of data used by
	vertex processor is in a vertex array object.

	void GenVertexArrays(sizei n, uint *arrays);
	void DeleteVertexArrays(sizei n, const uint *arrays);
	void BindVertexArray(uint array);
	//*/
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	/*/ // 10/25/2016 
	Creating and Binding Buffer Objects[2.9.1]

	void GenBuffers(sizei n, uint *buffers);
	void DeleteBuffers(sizei n, const uint *buffers);

	void BindBuffer(enum target, uint buffer);
	target: PIXEL_{PACK, UNPACK}_BUFFER,
	{UNIFORM, ARRAY, TEXTURE}_BUFFER,
	COPY_{READ, WRITE}_BUFFER,
	DRAW_INDIRECT_BUFFER, ELEMENT_ARRAY_BUFFER,
	{TRANSFORM_FEEDBACK, ATOMIC_COUNTER}_BUFFER
	
	void BindBufferRange(enum target, uint index,
	uint buffer, intptr offset, sizeiptr size);
	target: ATOMIC_COUNTER_BUFFER,
	{TRANSFORM_FEEDBACK, UNIFORM}_BUFFER
	
	void BindBufferBase(enum target,
	uint index, uint buffer);
	target: see BindBufferRange
	//*/
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	/*/ // 10/26/2016 
	Creating Buffer Object Data Stores[2.9.2]

	void BufferSubData(enum target,	intptr offset, sizeiptr size, const void *data);
	target: see BindBuffer
	
	void BufferData(enum target, sizeiptr size, const void *data, enum usage);
	usage: STREAM_{DRAW, READ, COPY},
	{DYNAMIC, STATIC}_{DRAW, READ, COPY}
	target: see BindBuffer
	Mapping/Unmapping Buffer Data[2.9.3]
	
	void *MapBufferRange(enum target, intptr offset, sizeiptr length, bitfield access);
	access: The logical OR of MAP_{READ, WRITE}_BIT,
	MAP_INVALIDATE_{BUFFER, RANGE}_BIT,
	MAP_{FLUSH_EXPLICIT, UNSYNCHRONIZED}_BIT,
	target: see BindBuffer
	
	void *MapBuffer(enum target, enum access);
	access: READ_ONLY, WRITE_ONLY, READ_WRITE
	
	void FlushMappedBufferRange(enum target, intptr offset, sizeiptr length);
	target: see BindBuffer
	
	boolean UnmapBuffer(enum target);
	target: see BindBuffer
	//*/
	// Put int VBO
	/*/ // 11/3/2016 
	GLfloat vertexData[] = {
	   //  X      Y     Z 
		0.0f,  0.8f, 0.0f,
	   -0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
	};
	//*/
	GLfloat vertexData[] = {
		//  X      Y     Z      U     V
		 0.0f,  0.8f, 0.0f,  0.5f, 1.0f,
		-0.8f, -0.8f, 0.0f,  0.0f, 0.0f,
		 0.8f, -0.8f, 0.0f,  1.0f, 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	/*/ // 10/26/2016 
	Vertex Arrays [2.8]

	void EnableVertexAttribArray(uint index);

	void VertexAttribIPointer(uint index, int size, enum type, sizei stride, const void *pointer);
	type: BYTE, SHORT, UNSIGNED_{BYTE, SHORT}, INT, UINT
	index: [0, MAX_VERTEX_ATTRIBS - 1]
	//*/
	// connect xyz to "vert" attribute of vertex-shader.txt
	glEnableVertexAttribArray(gProgram->attrib("vert"));
	//glVertexAttribPointer(gProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);
	glVertexAttribPointer(gProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);

	// connect uv to "vertTexCoord" attribute of vertex-shader.txt
	glEnableVertexAttribArray(gProgram->attrib("vertTexCoord"));
	glVertexAttribPointer(gProgram->attrib("vertTexCoord"), 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (const GLvoid*)(3*sizeof(GLfloat)));

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//
// LoadTexture
//
static void LoadTexture()
{
	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(ResourcePath("hazard.png"));
	gTexture = new tdogl::Texture(bmp);
}

static void Render()
{
	// clear everything
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind program (shaders)
	glUseProgram(gProgram->object());

	//
	// bind texture
	//
	/*/ // 10/31/2016 
	void ActiveTexture(enum texture);
	texture: TEXTUREi(whereiis
	[0, max(MAX_TEXTURE_COORDS,
	MAX_COMBINED_TEXTURE_IMAGE_UNITS)-1])
	Texture Objects [3.9.1][3.10.1]
	
	void BindTexture(enum target, uint texture);
	target: TEXTURE_{1, 2}D{_ARRAY},
	TEXTURE_{3D, RECTANGLE, BUFFER},
	TEXTURE_CUBE_MAP{_ARRAY},
	TEXTURE_2D_MULTISAMPLE{_ARRAY}
	
	void DeleteTextures(sizei n, const uint *textures);
	void GenTextures(sizei n, uint *textures);
	boolean AreTexturesResident(sizei n, uint *textures, boolean *residences);
	void PrioritizeTextures(sizei n, uint *textures, const clampf *priorities);
	//*/
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexture->object());

	//
	// set "tex" uniform in fragment shader
	//
	/*/ // 11/3/2016 
	#### Uniform vs Attribute Shader Variables
	- Attribute variables can have a different value for each vertex.
	- Uniform variables keep the same value for multiple vertices.
	- Uniforms can be accessed form any shader
	- Attributes must enter the vertex shader first, not the fragment shader.
	- Vertex shader can pass the value into fragment shader if necessary.
	
	 - glUniform* set the value of a uniform
	 - set the value of an attribute
	 -     store the values in a VBO
	 -     and send them to the shader
	 -     with a VAO and glVertexAttribPointer like we saw in the previous article
	 - using glVertexAttrib* functions
	 -     if you are not storing the values in a VBO.
	//*/
	gProgram->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0

	// bind VAO
	glBindVertexArray(gVAO);

	// draw VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// unbind VAO
	glBindVertexArray(0);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// unbind program
	glUseProgram(0);
	//gProgram->stopUsing();

	// swap the display buffer (displays what was just drawn)
	glfwSwapBuffers(gWindow);
}

void AppMain()
{
	//
	// ### glInit
	// - glfwInit / Window / Context
	// - glewInit
	//
	if (!glfwInit())
	{
		throw std::runtime_error("glfwInit failed");
	}

	gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "", NULL, NULL);
	if (gWindow == NULL)
	{
		throw std::runtime_error("glfwCreateWindow failed. Hardware OpenGL 3.2 compatible ?");
	}

	glfwMakeContextCurrent(gWindow);

	glewExperimental = GL_TRUE; // stops glew crashing on OSX :-/
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("glewInit failed");
	}

	if (!GLEW_VERSION_3_2)
	{
		throw std::runtime_error("OpenGL 3.2 API is not available.");
	}

	/**/ // 11/3/2016 OpenGL settings
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//*/

	//
	// ### LoadShaderAndProgram
	//  - shaderFromFile()
	//	- vertex - shader.txt / fragment - shader.txt
	//
	LoadShaderAndProgram();

	//
	// load texture
	//
	LoadTexture();

	//
	// set up VBO / VAO
	//
	LoadTriangle();

	while (!glfwWindowShouldClose(gWindow))
	{
		// pending events
		glfwPollEvents();

		Render();
	}

	glfwTerminate();
}

int main(int argc, char *argv[])
{
	try
	{
		AppMain();
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}