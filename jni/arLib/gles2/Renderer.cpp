#include "Renderer.h"

#include "time.h"
#include "util/Logger.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "modelData/bundehaus.h"

/*
 * Constructor & Destructor
 */

static const char cameraVerticeShader[] =
		"attribute vec4 a_position;"
		"attribute vec2 a_texCoord;"
		"varying vec2 v_texCoord;"
		"void main()"
		"{"
			"gl_Position = a_position;"
			"v_texCoord = a_texCoord;"
		"}";

static const char cameraFragmentShader[] =
		"precision mediump float;"
		"uniform sampler2D s_texture;"
		"varying vec2 v_texCoord;"
		"void main()\n"
		"{"
			"gl_FragColor = texture2D( s_texture, v_texCoord );"
		"}";



static const char objectVerticeShader[] =
		"attribute vec4 a_position;"
		"attribute vec2 a_texCoord;"
		"uniform mat4 u_modelViewProjection;"
		"varying vec2 v_texCoord;"
		"varying vec4 v_color;"

		"void main()"
		"{"
			"gl_Position = u_modelViewProjection * a_position;"
			"v_color = vec4(1.0,0.0,0.0,1.0);"
			"v_texCoord = a_texCoord;"
		"}";

static const char objectFragmentShader[] =
		"precision mediump float;"
		"varying vec4 v_color;"
		"void main()\n"
		"{"
			"gl_FragColor = v_color;"
		"}";


GLfloat const gCameraQuad[] = {
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
};

GLfloat const gCameraTexCoord[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
};



Renderer::Renderer()
{
	// 10 should be enough for the moment...
	models = new vector<Model*>;
	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);

	GLuint cameraVerticeShaderRef;
	GLuint cameraFragmentShaderRef;

	GLuint objectVerticeShaderRef;
	GLuint objectFragmentShaderRef;

	// Load the vertex/fragment shaders
	cameraVerticeShaderRef = loadShader(GL_VERTEX_SHADER, cameraVerticeShader);
	cameraFragmentShaderRef = loadShader(GL_FRAGMENT_SHADER, cameraFragmentShader);
	objectVerticeShaderRef = loadShader(GL_VERTEX_SHADER, objectVerticeShader);
	objectFragmentShaderRef = loadShader(GL_FRAGMENT_SHADER, objectFragmentShader);

	// Create the program the camera image
	cameraProgramRef = glCreateProgram();
	if (cameraProgramRef == 0) {
		LOGE("setup graphics failed !!!");
		return;
	}
	// attach the camera shaders
	glAttachShader(cameraProgramRef, cameraVerticeShaderRef);
	glAttachShader(cameraProgramRef, cameraFragmentShaderRef);
	this->linkProgram(cameraProgramRef);

	glUseProgram(cameraProgramRef);

	// Use tightly packed data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Create the program the objects
	objectProgramRef = glCreateProgram();
	if (objectProgramRef == 0) {
		LOGE("setup graphics failed !!!");
		return;
	}
	// attach the camera shaders
	glAttachShader(objectProgramRef, objectVerticeShaderRef);
	glAttachShader(objectProgramRef, objectFragmentShaderRef);
	this->linkProgram(objectProgramRef);


	vsPositionHandle = glGetAttribLocation(cameraProgramRef, "a_position");
	vsTexCoordHandle = glGetAttribLocation(cameraProgramRef, "a_texCoord");
    fsCameraTextureRef = glGetUniformLocation(cameraProgramRef, "s_texture");
    vsModelViewProjectionRef = glGetUniformLocation(objectProgramRef, "u_modelViewProjection");

}

Renderer::~Renderer()
{
	delete models;
}

// interface methods
void Renderer::setViewport(float offsetX, float offsetY, float width, float height)
{
	glViewport(offsetX, offsetY, width, height);
    checkGlError("glViewport");

	LOGI("viewport width: %f", width);
	LOGI("viewport height: %f", height);
}

GLuint Renderer::generateTexture() {
	// TODO: has to be improved when objects contains textures
	GLuint textureRef;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureRef);
    checkGlError("glGenTextures");
	return textureRef;
}

void Renderer::loadTexture(GLuint glRef, GLubyte* pixels, float width, float height) {
    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    checkGlError("glActiveTexture");

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glRef);
    checkGlError("glBindTexture");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
             GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    checkGlError("glGetAttribLocation");
}

void Renderer::renderFrame(EnvironmentData *envData) {

	static float angle = 0.0f;
	angle += 0.5f;
	if (angle >= 360.0f)
		angle = 0.0f;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(cameraProgramRef);
    checkGlError("glUseProgram");

    glVertexAttribPointer(vsPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, gCameraQuad);
    checkGlError("glVertexAttribPointer");
    glVertexAttribPointer(vsTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, gCameraTexCoord);
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(vsPositionHandle);
    checkGlError("glEnableVertexAttribArray");

    glEnableVertexAttribArray(vsTexCoordHandle);
    checkGlError("glEnableVertexAttribArray");

    // Set the sampler texture unit to 0
    glUniform1i(fsCameraTextureRef, 0);
    checkGlError("fsCameraTextureRef");
    glBindTexture(GL_TEXTURE_2D, envData->cameraTextrueRef);
    checkGlError("glBindTexture");
    glDrawArrays(GL_TRIANGLES, 0, 6);
    checkGlError("glDrawArrays");

    glUseProgram(objectProgramRef);

    Model* m = models->at(0);
    glm::mat4 MVP = envData->getProjectionMatrix() * envData->getViewMatrix() * m->getModelMatrix();
    glUniformMatrix4fv(vsModelViewProjectionRef, 1, GL_FALSE, glm::value_ptr(MVP));

    glVertexAttribPointer(vsPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, m->vertices);
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(vsPositionHandle);
    checkGlError("glEnableVertexAttribArray");

    glDrawArrays(GL_TRIANGLES, 0, m->verticesSize/3);
    checkGlError("glDrawArrays");
    /*
    m = models->at(1);
    //View = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
    //View = glm::translate(View, glm::vec3(0.0, 0.0, -13.0));
    View = (*m->modelView);
    View = Projection * View;

    glUniformMatrix4fv(vsModelViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(View));

    glVertexAttribPointer(vsPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, m->vertices);
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(vsPositionHandle);
    checkGlError("glEnableVertexAttribArray");

    glDrawArrays(GL_TRIANGLES, 0, m->verticesSize/3);
    checkGlError("glDrawArrays");
	*/
	/*
	//calculate FPS
	 struct timespec res;
	 clock_gettime(CLOCK_MONOTONIC, &res);

	 double currentTime = 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
	 double timeDelta = currentTime-lastTime;
	 lastTime = currentTime;
	 //LOGI("FPS: %f",timeDelta);
	  */
}




GLuint Renderer::loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {

    	int shaderStringLength = strlen(pSource);
        glShaderSource(shader, 1, &pSource, &shaderStringLength);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                    exit(-1);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

void Renderer::linkProgram(GLuint programHandler) {
	GLint linked;
	// Link the program
	glLinkProgram(programHandler);
	// Check the link status
	glGetProgramiv(programHandler, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint infoLen = 0;
		glGetProgramiv(programHandler, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			LOGE("Error linking program",LOG_TAG);
		}
		glDeleteProgram(programHandler);
		return;
	}

}

// adding models

void Renderer::addModel(Model* m) {
	models->push_back(m);
}

void Renderer::printGLString(const char *name, GLenum s) {
	const char *v = (const char *) glGetString(s);
	LOGI("GL %s = %s\n", name, v);

}

void Renderer::checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		LOGI("after %s() glError (0x%x)\n", op, error);
	}
}
