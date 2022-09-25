#ifndef VERTICES_H
#define VERTICES_H

#include <span>
#include <type_traits>

#include "glfw_wrapper.h"

template<typename T>
concept CoordinateType = 
	std::is_arithmetic_v<T>;

struct Color
{
	double R;
	double G;
	double B;
};

template<CoordinateType T, typename Additional> 
struct Vertice;

template<CoordinateType T> 
struct Vertice<T, void>
{
	T x;
	T y;
	T z;

	static inline constexpr 
		std::size_t ADDITION_OFFSET = sizeof(T) * 3;
};

template<CoordinateType T, typename Additional> 
struct Vertice
{
	T x;
	T y;
	T z;

	Additional add;

	static inline constexpr 
		std::size_t ADDITION_OFFSET = sizeof(T) * 3;
};

using SimpleVertice = Vertice<double, void>;
using ColorVertice = Vertice<double, Color>;


// class Vertices
// {
// public:
// 	Vertices();
// 	~Vertices();
//
// 	template<typename VerticeT>
// 	bool AddVertices(const std::span<Vertice<VerticeT>> verticesArr)
// 	{
// 		static_assert(std::is_trivial<Vertice<VerticeT>>(), "Vertice struct should be POD!");
// 		glBufferData(GL_ARRAY_BUFFER, 
// 			sizeof(Vertice<VerticeT>) * verticesArr.size(),
// 			verticesArr.data(), GL_STATIC_DRAW);
// 	}
// private:
// 	GLuint m_vboID;
// };

#endif // VERTICES_H
