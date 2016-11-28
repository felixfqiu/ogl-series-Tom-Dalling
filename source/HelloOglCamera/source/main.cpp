
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
#include <vector>

// tdogl classes
#include "tdogl/Program.h"
#include "tdogl/Texture.h"
#include "tdogl/Camera.h"

// constants
const glm::vec2 SCREEN_SIZE(340, 770);

// DO_NOT_SHOW_CONSOLE
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

struct ModelAsset
{
	tdogl::Texture* texture;
	tdogl::Program* shaders;
	GLuint vbo;
	GLuint vao;
	GLenum drawType;
	GLint drawStart;
	GLint drawCount;

	ModelAsset() :
		texture(NULL),
		shaders(NULL),
		vbo(0),
		vao(0),
		drawType(GL_TRIANGLES),
		drawStart(0),
		drawCount(0)
	{}
};

struct ModelInstance
{
	ModelAsset* asset;
	glm::mat4 transform;

	ModelInstance(ModelAsset* other_asset, glm::mat4 other_transform) :
		asset(other_asset),
		transform(other_transform)
	{}
};

ModelAsset gWoodenCrate;
std::vector<ModelInstance> gInstance;

// globals
GLFWwindow* gWindow = NULL;
double gScrollY = 0.0;
tdogl::Camera gCamera;
GLfloat gDegreesRotated = 0.0f;

static tdogl::Program* LoadShaders(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	std::vector<tdogl::Shader> shaders;
	shaders.push_back(tdogl::Shader::shaderFromFile(vertexShaderFile, GL_VERTEX_SHADER));
	shaders.push_back(tdogl::Shader::shaderFromFile(fragmentShaderFile, GL_FRAGMENT_SHADER));
	return new tdogl::Program(shaders);
}

static tdogl::Texture* LoadTexture(const char* textureFile)
{
	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(textureFile);
	return new tdogl::Texture(bmp);
}

static void LoadAssert()
{
	// LoadTexture
	gWoodenCrate.texture = LoadTexture(ResourcePath("wooden-crate.jpg").c_str());

	// LoadShaders
	gWoodenCrate.shaders = LoadShaders(ResourcePath("vertex-shader.txt").c_str(), ResourcePath("fragment-shader.txt").c_str());

	// gen/bind VAO
	glGenVertexArrays(1, &gWoodenCrate.vao);
	glBindVertexArray(gWoodenCrate.vao);

	// gen/bind VBO
	glGenBuffers(1, &gWoodenCrate.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gWoodenCrate.vbo);

	// Put int VBO
	gWoodenCrate.drawType = GL_TRIANGLES;
	gWoodenCrate.drawStart = 0;
	gWoodenCrate.drawCount = 36; // 6*2*3

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
	glEnableVertexAttribArray(gWoodenCrate.shaders->attrib("vert"));
	glVertexAttribPointer(gWoodenCrate.shaders->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);

	// uv to "vertTexCoord" of vertex-shader.txt
	glEnableVertexAttribArray(gWoodenCrate.shaders->attrib("vertTexCoord"));
	glVertexAttribPointer(gWoodenCrate.shaders->attrib("vertTexCoord"), 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (const GLvoid*)(3*sizeof(GLfloat)));

	// unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glm::mat4 translate(GLfloat x, GLfloat y, GLfloat z)
{
	return glm::translate(glm::mat4(), glm::vec3(x, y, z));
}

glm::mat4 scale(GLfloat x, GLfloat y, GLfloat z)
{
	return glm::scale(glm::mat4(), glm::vec3(x, y, z));
}

static void CreateInstance()
{
	gInstance.push_back( ModelInstance(&gWoodenCrate, translate(-8, 0, 0) * scale(1, 6, 1))    ); // HLeft
	gInstance.push_back( ModelInstance(&gWoodenCrate, translate(-4, 0, 0) * scale(1, 6, 1))    ); // HRight
	gInstance.push_back( ModelInstance(&gWoodenCrate, translate(-6, 0, 0) * scale(2, 1, 0.8f)) ); // HMid
	gInstance.push_back( ModelInstance(&gWoodenCrate, glm::mat4())                             ); // iTop
	gInstance.push_back( ModelInstance(&gWoodenCrate, translate(0, -4, 0) * scale(1, 2, 1))    ); // iBottom
}

static void RenderInstance(const ModelInstance& inst)
{
	ModelAsset* assert = inst.asset;
	tdogl::Program* shaders = assert->shaders;

	// bind program (shaders)
	shaders->use();

	shaders->setUniform("camera", gCamera.matrix());
	shaders->setUniform("model", inst.transform);
	shaders->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0

	// bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, assert->texture->object());

	// VAO bind
	glBindVertexArray(assert->vao);

	// draw
	glDrawArrays(assert->drawType, assert->drawStart, assert->drawCount);

	// unbind VAO/texture/program
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	shaders->stopUsing();
}

static void Render()
{
	// clear everything
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (size_t i=0; i<gInstance.size(); i++)
	{
		RenderInstance( gInstance[i] );
	}

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

	//
	gInstance[3].transform = glm::rotate(glm::mat4(), glm::radians(gDegreesRotated), glm::vec3(0,1,0));


	//
	// camera move pos, check 'W','A','S','D'
	//
	const float moveSpeed = 2.0;
	if (glfwGetKey(gWindow, 'W'))
	{
		gCamera.move(secondsElapsed * moveSpeed * gCamera.directionFacing());
	}
	else if (glfwGetKey(gWindow, 'S'))
	{
		gCamera.move(secondsElapsed * moveSpeed * -gCamera.directionFacing());
	}

	if (glfwGetKey(gWindow, 'D'))
	{
		gCamera.move(secondsElapsed * moveSpeed * gCamera.directionRight());
	}
	else if (glfwGetKey(gWindow, 'A'))
	{
		gCamera.move(secondsElapsed * moveSpeed * -gCamera.directionRight());
	}	

	//
	// camera up and down, check 'E','F'
	//
	if (glfwGetKey(gWindow, 'F'))
	{
		gCamera.move(secondsElapsed * moveSpeed * glm::vec3(0, 1, 0));
	}
	else if (glfwGetKey(gWindow, 'E'))
	{
		gCamera.move(secondsElapsed * moveSpeed * -glm::vec3(0, 1, 0));
	}

	//
	// on mouse wheel, update field of view
	//
	const float zoomSensitivity = -0.2f;
	float fov = gCamera.fieldOfView() + zoomSensitivity*(float)gScrollY;
	if (fov < 5.0f) fov = 5.0f;
	if (fov > 130.0f) fov = 130.0f;
	gCamera.setFieldOfView(fov);
	gScrollY = 0; // reset

	//
	// on mouse move, rotate camera
	//
	const float mouseSensitivity = -0.1f;
	double mouseX, mouseY;
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);
	gCamera.offsetOrientation(mouseSensitivity*(float)mouseY, mouseSensitivity*(float)mouseX);
	glfwSetCursorPos(gWindow, 0, 0); // reset
}

void OnScroll(GLFWwindow* window, double dX, double dY)
{
	gScrollY += dY;
}

void AppMain()
{
	if (!glfwInit())
	{
		throw std::runtime_error("glfwInit failed");
	}

	//
	// Create Window
	//
	gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "", NULL, NULL);
	if (gWindow == NULL)
	{
		throw std::runtime_error("glfwCreateWindow failed. Hardware OpenGL 3.2 compatible ?");
	}
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(gWindow, 0, 0);

	glfwSetScrollCallback(gWindow, OnScroll);

	glfwMakeContextCurrent(gWindow);

	glewExperimental = GL_TRUE; // stops glew crashing on OSX :-/
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("glewInit failed");
	}

	// GLEW throws some errors, so discard all the errors so far
	while (glGetError() != GL_NO_ERROR) {}

	// GLEW version check
	if (!GLEW_VERSION_3_2)
	{
		throw std::runtime_error("OpenGL 3.2 API is not available.");
	}

	//
	// OpenGL settings
	//
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// initialise assert
	LoadAssert();

	// create instance based on assert
	CreateInstance();

	//
	// setup gCamera
	//
	//gCamera.setPos(glm::vec3(0, 0, 4));
	gCamera.setPos(glm::vec3(-4, 0, 17));
	gCamera.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);

	//
	// loop
	//
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(gWindow))
	{
		// pending events
		glfwPollEvents();

		// update
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

		// exit program if escape key
		if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(gWindow, GL_TRUE);
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