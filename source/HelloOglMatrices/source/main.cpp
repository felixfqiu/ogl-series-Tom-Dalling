
#include "platform.hpp"

// third-party libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// standard C++ libraries
#include <cassert>
#include <stdexcept>
#include <iostream>

// tdogl classes
#include "tdogl/Program.h"
#include "tdogl/Texture.h"

// constants
const glm::vec2 SCREEN_SIZE(340, 770);

// DO_NOT_SHOW_CONSOLE
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

// globals
GLFWwindow* gWindow = NULL;
tdogl::Texture* gTexture = NULL;
tdogl::Program* gProgram = NULL;
GLuint gVAO = 0;
GLuint gVBO = 0;
GLfloat gDegreesRotated = 0.0f;

static void LoadShader()
{
	std::vector<tdogl::Shader> shaders;
	shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("vertex-shader.txt"), GL_VERTEX_SHADER));
	shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("fragment-shader.txt"), GL_FRAGMENT_SHADER));
	gProgram = new tdogl::Program(shaders);

	gProgram->use();

		//set the "projection" uniform in the vertex shader, because it's not going to change
		glm::mat4 projection = glm::perspective(glm::radians(50.0f), SCREEN_SIZE.x / SCREEN_SIZE.y, 0.1f, 10.0f);
		gProgram->setUniform("projection", projection);

		//set the "camera" uniform in the vertex shader, because it's also not going to change
		//glm::vec3 camPos = glm::vec3(3, 3, 3);
		glm::vec3 camPos = glm::vec3(6, 6, 6);
		glm::vec3 camLookAt = glm::vec3(0, 0, 0);
		glm::vec3 camNormal = glm::vec3(0, 1, 0);
		glm::mat4 camera = glm::lookAt(camPos, camLookAt, camNormal);
		gProgram->setUniform("camera", camera);

	gProgram->stopUsing();
}

static void LoadObject()
{
	// VAO
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	// VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	// Put int VBO
	GLfloat vertexData[] = {
    //  X     Y     Z       U     V
    // bottom
    -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
     1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,

    // top
    -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
     1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

    // front
    -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
     1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
     1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

    // back
    -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
     1.0f, 1.0f,-1.0f,   1.0f, 1.0f,

    // left
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
    -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,

    // right
     1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
     1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
     1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
     1.0f, 1.0f, 1.0f,   0.0f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// xyz to "vert" of vertex-shader.txt
	glEnableVertexAttribArray(gProgram->attrib("vert"));
	glVertexAttribPointer(gProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);

	// uv to "vertTexCoord" of vertex-shader.txt
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
	//tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(ResourcePath("hazard.png"));
	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(ResourcePath("wooden-crate.jpg"));	
	gTexture = new tdogl::Texture(bmp);
}

static void Render()
{
	// clear everything
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind program (shaders)
	gProgram->use();

		// set the "model" uniform in the vertex shader, based on the gDegreesRotated global
		glm::mat4 model = glm::rotate(glm::mat4(), glm::radians(gDegreesRotated), glm::vec3(0,1,0));
		gProgram->setUniform("model", model);

		// bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gTexture->object());
		gProgram->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0

		// VAO bind/draw
		glBindVertexArray(gVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6*2*3);

		// unbind VAO/texture/program
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

	gProgram->stopUsing();

	// displays what was just drawn
	glfwSwapBuffers(gWindow);
}

void Update(float secondsElapsed)
{
	const GLfloat degreesPerSecond = 180.0f;
	gDegreesRotated += secondsElapsed * degreesPerSecond;
	while (gDegreesRotated > 360.0f)
	{
		gDegreesRotated -= 360.0f;
	}
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

	// GLEW throws some errors, so discard all the errors so far
	while (glGetError() != GL_NO_ERROR) {}

	if (!GLEW_VERSION_3_2)
	{
		throw std::runtime_error("OpenGL 3.2 API is not available.");
	}

	// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//
	// ### LoadShader
	//  - shaderFromFile()
	//	- vertex-shader.txt / fragment-shader.txt
	//
	LoadShader();

	//
	// load texture
	//
	LoadTexture();

	//
	// set up VBO / VAO
	//
	LoadObject();

	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(gWindow))
	{
		// pending events
		glfwPollEvents();

		// update the scene
		double thisTime = glfwGetTime();
		Update((float)(thisTime - lastTime));
		lastTime = thisTime;

		// draw frame
		Render();

		// check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "OpenGL Error " << error << std::endl;
		}
	}

	// clean up an exit
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