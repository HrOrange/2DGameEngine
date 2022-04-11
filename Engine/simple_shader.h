#pragma once
#include "common_includes.h"

class simple_shader {
public:
	unsigned int m_ID;

	simple_shader();
	simple_shader(const char* vertex_shader, const char* fragment_shader);
	~simple_shader();

	void init(const char* vertex_shader, const char* fragment_shader);
	void use();
};