#include "common_includes.h"


/*
Allocation_Metric s_AllocationMetric;
void* operator new(size_t size) {
	s_AllocationMetric.TotalAllocated += size;
	println(s_AllocationMetric.CurrentUsage());

	return malloc(size);
}

void operator delete(void* memory, size_t size) {
	s_AllocationMetric.TotalAllocated -= size;
	//println(s_AllocationMetric.CurrentUsage());

	free(memory);
}
*/

int GLAD_init() {
	int GLADInitResult = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (GLADInitResult == 0) return -1;
	return 1; // 1 == good, -1 == bad
}
int GLFW_init() {
	int GLFWInitResult = glfwInit();
	if (GLFWInitResult == 0) return -1;
	return 1; // 1 == good, -1 == bad
}
