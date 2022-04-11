#pragma once

//common includes
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>   //for like time and stuff
#include <map>      //for like dictionaries and stuff
#include <stdlib.h> //for like the rand function and stuff
#include <thread>
#include <mutex>

//OpenGL things
#include<GLAD/glad.h>
#include<GLFW/glfw3.h>


//Macros
/* I have chosen to go static linking for my engine for now, because dynamic linking is a hassle, the files are overall bigger, 
static linking does more optemization and so on. I cant find a reason to do dynamic linking over static linking at all

NOTE: if you want to switch to dynamic linking, remember to use the dynamic linking glfw library instead of the static one we are using now.
The files should be there, so you just need to switch from glfw3.lib to glfw3dll.lib. I dont if glad also has a dynamic linking library. check.

#if ENGINE 
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
*/

//some common functions
inline int get_random(int start, int end) { //start = included, end = exclude
	return std::rand() % (end - start) + start;
}

template <class myType>
inline bool go_through_list(myType list[], myType value) {
	for (int dad = 0; dad < sizeof(list) / sizeof(myType); dad++) {
		if (list[dad] == value) return true;
	}
	return false;
}


template <class myType>
inline void print(myType print_me) {
	std::cout << print_me;
}

template <class myType>
inline void println(myType print_me) {
	std::cout << print_me << std::endl;
}

/*
struct Allocation_Metric {
	uint32_t TotalAllocated = 0;
	uint32_t TotalFreed = 0;

	inline uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }
};
void* operator new(size_t size);
void operator delete(void* memory, size_t size);
*/


int GLFW_init();
int GLAD_init();