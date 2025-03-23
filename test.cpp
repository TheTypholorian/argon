#define GLEW_STATIC
#include "glew.h"
#include <iostream>
#include "argon.hpp"
#include "linkedList.hpp"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg.h"
#include "nanovg_gl.h"
#include "io.hpp"

using namespace std;
using namespace H;
using namespace Ar;

void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	cerr << message << endl;
}

int main() {
	if (!glfwInit()) {
		cerr << "Failed to init glfw" << endl;
		return -1;
	}

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFrame frame(1280, 800, "hello :3");

	glfwMakeContextCurrent(frame.handle);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		cerr << "Failed to init glew" << endl;
		return -1;
	}

	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback((GLDEBUGPROC)DebugCallback, nullptr);

	NVGcontext* vg = nvgCreateGL3(NVG_ANTIALIAS);

	string data = loadRes(L"times.ttf", RT_RCDATA);
	nvgCreateFontMem(vg, "Times New Roman", (unsigned char*)data.data(), data.length(), true);

	GLTextButton b("Hello World", "Times New Roman");
	b.set(5, 5, 500, 100);
	frame.children.addFirst(&b);

	glfwShowWindow(frame.handle);
	glfwFocusWindow(frame.handle);

	while (!glfwWindowShouldClose(frame.handle)) {
		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		nvgBeginFrame(vg, 1280, 800, 1);

		frame.render(vg);

		nvgEndFrame(vg);

		glfwSwapBuffers(frame.handle);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}