#include <glfw3.h>
#include <hydrogen.hpp>
#include <string>
#include "linkedList.hpp"
#include "rect2D.hpp"
#include "nanovg.h"
#include "palette.hpp"
#include <iostream>

using namespace std;
using namespace H;

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

		virtual void windowFocus(GLFWwindow* win, bool f) = 0;

		virtual void windowIconify(GLFWwindow* win, bool i) = 0;

		virtual void key(GLFWwindow* win, int k, int sc, int a, int m) = 0;

		virtual void character(GLFWwindow* win, unsigned int cp) = 0;

		virtual void mouseButton(GLFWwindow* win, int b, int a, int m) = 0;

		virtual void cursorPos(GLFWwindow* win, double x, double y) = 0;

		virtual void cursorEnter(GLFWwindow* win, bool e) = 0;

		virtual void scroll(GLFWwindow* win, double x, double y) = 0;

		virtual void drop(GLFWwindow* win, int c, const char** p) = 0;

		static AllGLFWCallbacks* get(GLFWwindow* window) {
			return static_cast<AllGLFWCallbacks*>(glfwGetWindowUserPointer(window));
		}
	};

	class GLComponent : public AllGLFWCallbacks, public Rect2D<hushort> {
	public:
		LinkedList<GLComponent*> children;

		virtual void render(NVGcontext* vg) {
			render(vg, *this);
		}

		virtual void render(NVGcontext* vg, Rect2D<hushort> rect) {
			renderThis(vg, rect);
			renderChildren(vg, rect);
		}

		virtual void renderThis(NVGcontext* vg, Rect2D<hushort> rect) {
		}

		virtual void renderChildren(NVGcontext* vg, Rect2D<hushort> rect) {
			for (auto& c : children) {
				c->render(vg, rect + *c);
			}
		}

		void windowPos(GLFWwindow* win, int x, int y) override {
			for (auto& c : children) {
				c->windowPos(win, x, y);
			}
		}

		void windowSize(GLFWwindow* win, int w, int h) override {
			for (auto& c : children) {
				c->windowSize(win, w, h);
			}
		}

		void windowClose(GLFWwindow* win) override {
			for (auto& c : children) {
				c->windowClose(win);
			}
		}

		void windowRefresh(GLFWwindow* win) override {
			for (auto& c : children) {
				c->windowRefresh(win);
			}
		}

		void windowFocus(GLFWwindow* win, bool f) {
			for (auto& c : children) {
				c->windowFocus(win, f);
			}
		}

		void windowIconify(GLFWwindow* win, bool i) {
			for (auto& c : children) {
				c->windowIconify(win, i);
			}
		}

		void key(GLFWwindow* win, int key, int scancode, int action, int mods) {
			for (auto& c : children) {
				c->key(win, key, scancode, action, mods);
			}
		}

		void character(GLFWwindow* win, unsigned int cp) {
			for (auto& c : children) {
				c->character(win, cp);
			}
		}

		void mouseButton(GLFWwindow* win, int button, int action, int mods) {
			for (auto& c : children) {
				c->mouseButton(win, button, action, mods);
			}
		}

		void cursorPos(GLFWwindow* win, double x, double y) {
			for (auto& c : children) {
				c->cursorPos(win, x, y);
			}
		}

		void cursorEnter(GLFWwindow* win, bool e) {
			for (auto& c : children) {
				c->cursorEnter(win, e);
			}
		}

		void scroll(GLFWwindow* win, double x, double y) {
			for (auto& c : children) {
				c->scroll(win, x, y);
			}
		}

		void drop(GLFWwindow* win, int num, const char** paths) {
			for (auto& c : children) {
				c->drop(win, num, paths);
			}
		}
	};

	class GLFrame : public GLComponent {
	public:
		GLFWwindow* handle;

		GLFrame(int width, int height, const char* title = "", GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr) {
			handle = glfwCreateWindow(width, height, title, monitor, share);

			if (!handle) {
				const char* desc;
				int code = glfwGetError(&desc);
				cerr << desc << endl;
				throw code;
			}

			glfwSetWindowUserPointer(handle, this);

			init(handle);
		}

		static GLFrame* get(GLFWwindow* window) {
			return static_cast<GLFrame*>(glfwGetWindowUserPointer(window));
		}
	};

	class GLButton : public GLComponent {
	public:
		bool down = false, hover = false;

		void renderThis(NVGcontext* vg, Rect2D<hushort> rect) {
			nvgBeginPath(vg);
			nvgRect(vg, rect.getX(), rect.getY(), rect.getW(), rect.getH());

			nvgFillColor(vg, hover ? gold3 : gold5);
			nvgFill(vg);

			nvgBeginPath(vg);
			nvgRect(vg, rect.getX() + 4, rect.getY() + 4, rect.getW() - 8, rect.getH() - 8);

			nvgFillColor(vg, down ? black3 : black5);
			nvgFill(vg);
		}

		void cursorPos(GLFWwindow* win, double x, double y) {
			hover = contains(x, y);
			GLComponent::cursorPos(win, x, y);
		}

		void mouseButton(GLFWwindow* win, int button, int action, int mods) {
			down = button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS && hover;
			GLComponent::mouseButton(win, button, action, mods);
		}
	};

	class GLTextButton : public GLButton {
	public:
		string text, font;

		GLTextButton(string text = "", string font = "Times New Roman") : text(text), font(font) {
		}

		void renderThis(NVGcontext* vg, Rect2D<hushort> rect) {
			GLButton::renderThis(vg, rect);

			nvgFontSize(vg, 24);
			nvgFontFace(vg, font.data());
			nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFillColor(vg, hover ? gold3 : gold5);
			nvgText(vg, rect.getX() + rect.getW() / 2, rect.getY() + rect.getH() / 2, text.data(), &text.data()[text.length()]);
		}
	};
}