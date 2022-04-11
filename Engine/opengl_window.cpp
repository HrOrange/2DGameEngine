#include "opengl_window.h"

opengl_window::opengl_window() {
}
opengl_window::opengl_window(int width, int height)
	: m_width(width),
	  m_height(height) {

}
opengl_window::opengl_window(int width, int height, bool fullscreen_setting)
	: m_width(width),
	  m_height(height),
	  m_fullscreen_setting(fullscreen_setting) {

}
opengl_window::opengl_window(bool fullscreen_setting) 
	: m_fullscreen_setting(fullscreen_setting) {

}
void opengl_window::toggle_fullscreen() {
	if (m_fullscreen_setting) {
		GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
		int monitor_width, monitor_height, monitor_xpos, monitor_ypos;
		glfwGetMonitorWorkarea(primary_monitor, &monitor_xpos, &monitor_ypos, &monitor_width, &monitor_height);

		glfwSetWindowMonitor(m_window, NULL, (monitor_width - m_width) / 2, (monitor_height - m_height) / 2, m_width, m_height, GLFW_DONT_CARE);
	}
	else {
		GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
		int monitor_width, monitor_height, monitor_xpos, monitor_ypos;
		glfwGetMonitorWorkarea(primary_monitor, &monitor_xpos, &monitor_ypos, &monitor_width, &monitor_height);

		/*
		println(monitor_width);
		println(monitor_height);
		println(monitor_xpos);
		println(monitor_ypos);
		*/

		glfwSetWindowMonitor(m_window, primary_monitor, monitor_xpos, monitor_ypos, monitor_width, monitor_height, GLFW_DONT_CARE);
	}
	m_fullscreen_setting = !m_fullscreen_setting;
}
opengl_window::opengl_window(int width, int height, const std::string& title, bool fullscreen_setting, float background_color[4], bool visible, GLFWwindow* share_resources_with)
	: m_width(width), 
	  m_height(height), 
	  m_title(title),
      m_fullscreen_setting(fullscreen_setting) {
	
	
	for (int i = 0; i < 4; i++) m_background_color[i] = background_color[i] / 255.0f;
	
	//HINTS
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE); //this set the buffer to 1 instead of 2
	if(visible) glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	else glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	

	/* Create a windowed mode window and its OpenGL context */
	if (fullscreen_setting) {
		
		//retrieve monitor information
		GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
		int monitor_width, monitor_height, monitor_xpos, monitor_ypos;
		glfwGetMonitorWorkarea(primary_monitor, &monitor_xpos , &monitor_ypos, &monitor_width, &monitor_height);

		//create window
		m_window = glfwCreateWindow(monitor_width, monitor_height, title.c_str(), primary_monitor, share_resources_with);
	}
	else m_window = glfwCreateWindow(m_width, m_height, title.c_str(), NULL, share_resources_with);

	if (!m_window) glfwTerminate();
	else {
		// Introduce the window into the current context
		glfwMakeContextCurrent(m_window);
		

		//Turn off V-sync
		glfwSwapInterval(0);

		//Load GLAD so it configures OpenGL
		gladLoadGL();

		// Specify the viewport of OpenGL in the Window
		// In this case the viewport goes from x = 0, y = 0, to x = width, y = height
		glViewport(0, 0, m_width, m_height);

		/* Set background color setting */
		glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
	}
}
void opengl_window::init(int width, int height, std::string title, uint8_t fullscreen_setting, float background_color[4], bool visible, GLFWwindow* share_resources_with) {
	for (int i = 0; i < 4; i++) m_background_color[i] = background_color[i];
	m_title = title;
	m_fullscreen_setting = fullscreen_setting;

	//HINTS
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE); //this set the buffer to 1 instead of 2
	if (visible) glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	else glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	
	//window = 0, fullscreen = 1, bordless full screen window = 2
	if (m_fullscreen_setting == 1) {
		glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);

		GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primary_monitor);

		println(mode->width);
		println(mode->height);
		m_width = mode->width;
		m_height = mode->height;

		//create window
		m_window = glfwCreateWindow(mode->width, mode->height, "My Title", primary_monitor, NULL);
	}
	else if (m_fullscreen_setting == 2) {
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

		//retrieve monitor information
		GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primary_monitor);

		//glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		//glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		//glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		//glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE); //max refresh rate

		println(mode->width);
		println(mode->height);
		m_width = mode->width;
		m_height = mode->height;

		//create window
		m_window = glfwCreateWindow(mode->width, mode->height, "My Title", NULL, NULL);
	}
	else {
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


		m_width = width;
		m_height = height;
		m_window = glfwCreateWindow(m_width, m_height, title.c_str(), NULL, share_resources_with);
	}




	if (!m_window) {
		println("Window could not be created.");
		glfwTerminate();
	}
	else {
		// Introduce the window into the current context
		glfwMakeContextCurrent(m_window);

		//Turn off V-sync
		glfwSwapInterval(0);

		//Load GLAD so it configures OpenGL
		gladLoadGL();

		// Specify the viewport of OpenGL in the Window
		// In this case the viewport goes from x = 0, y = 0, to x = width, y = height
		glViewport(0, 0, m_width, m_height);

		/* Set background color setting */
		glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
	}
}

void opengl_window::close() {
	//free used memory
}
opengl_window::~opengl_window() {
	close();

	//stop glfw
	glfwTerminate();
}
int opengl_window::add_triangle() {
	m_triangles++;
	return m_triangles;
}
int opengl_window::stay_open() {
	return 1 - glfwWindowShouldClose(m_window);
}
void opengl_window::update() {

	/* Swap front and back buffers */
	glfwSwapBuffers(m_window);
	//glFlush();

	/* Poll for and process events */
	glfwPollEvents();

	
}