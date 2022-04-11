#pragma once
#include "common_includes.h"

class simple_shader {
private:
	GLuint ID;


public:
	simple_shader();
	~simple_shader();

	void init(const char* vertex_shader, const char* fragment_shader);
	void use();
};