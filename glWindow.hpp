#include <glfw3.h>
#include <hydrogen.hpp>
#include <string>

using namespace std;

namespace Ar {
	class AllGLFWCallbacks {
	public:
		void init(GLFWwindow* handle) {
			glfwSetWindowUserPointer(handle, this);
			glfwSetWindowPosCallback(handle, [](GLFWwindow* win, int x, int y) { get(win)->windowPos(win, x, y); });
			glfwSetWindowSizeCallback(handle, [](GLFWwindow* win, int w, int h) { get(win)->windowSize(win, w, h); });
			glfwSetWindowCloseCallback(handle, [](GLFWwindow* win) { get(win)->windowClose(win); });
			glfwSetWindowRefreshCallback(handle, [](GLFWwindow* win) { get(win)->windowRefresh(win); });
			glfwSetWindowFocusCallback(handle, [](GLFWwindow* win, int f) { get(win)->windowFocus(win, f); });
			glfwSetWindowIconifyCallback(handle, [](GLFWwindow* win, int i) { get(win)->windowIconify(win, i); });
			glfwSetKeyCallback(handle, [](GLFWwindow* win, int k, int sc, int a, int m) { get(win)->key(win, k, sc, a, m); });
			glfwSetCharCallback(handle, [](GLFWwindow* win, unsigned int cp) { get(win)->character(win, cp); });
			glfwSetMouseButtonCallback(handle, [](GLFWwindow* win, int b, int a, int m) { get(win)->mouseButton(win, b, a, m); });
			glfwSetCursorPosCallback(handle, [](GLFWwindow* win, double x, double y) { get(win)->cursorPos(win, x, y); });
			glfwSetCursorEnterCallback(handle, [](GLFWwindow* win, int e) { get(win)->cursorEnter(win, e); });
			glfwSetScrollCallback(handle, [](GLFWwindow* win, double x, double y) { get(win)->scroll(win, x, y); });
			glfwSetDropCallback(handle, [](GLFWwindow* win, int c, const char** p) { get(win)->drop(win, c, p); });
		}

		virtual void windowPos(GLFWwindow* win, int x, int y) = 0;

		virtual void windowSize(GLFWwindow* win, int w, int h) = 0;

		virtual void windowClose(GLFWwindow* win) = 0;

		virtual void windowRefresh(GLFWwindow* win) = 0;

		virtual void windowFocus(GLFWwindow* win, int f) = 0;

		virtual void windowIconify(GLFWwindow* win, int i) = 0;

		virtual void key(GLFWwindow* win, int k, int sc, int a, int m) = 0;

		virtual void character(GLFWwindow* win, unsigned int cp) = 0;

		virtual void mouseButton(GLFWwindow* win, int b, int a, int m) = 0;

		virtual void cursorPos(GLFWwindow* win, double x, double y) = 0;

		virtual void cursorEnter(GLFWwindow* win, int e) = 0;

		virtual void scroll(GLFWwindow* win, double x, double y) = 0;

		virtual void drop(GLFWwindow* win, int c, const char** p) = 0;

		static AllGLFWCallbacks* get(GLFWwindow* window) {
			return static_cast<AllGLFWCallbacks*>(glfwGetWindowUserPointer(window));
		}
	};

	class GLComponent : public AllGLFWCallbacks {
	public:
	};

	class GLFrame : public GLComponent {
	public:
		GLFWwindow* handle;

		GLFrame(int width, int height, const char* title = "", GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);

		static GLFrame* get(GLFWwindow* window) {
			return static_cast<GLFrame*>(glfwGetWindowUserPointer(window));
		}
	};
}