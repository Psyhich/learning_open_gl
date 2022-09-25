#ifndef STATE_H
#define STATE_H

#include <type_traits>

#include "glfw_wrapper.h"

class State
{
public:
	State()
	{
		glGenVertexArrays(1, &m_vaoID);
	}

	State(const State &copyValue) = delete;
	State(State &&moveValue)
	{
		m_vaoID = moveValue.m_vaoID;
		moveValue.m_vaoID = -1;
	}

	State &operator=(const State &copyValue) = delete;
	State &operator=(State &&moveValue)
	{
		if(this == &moveValue)
		{
			return *this;
		}

		m_vaoID = moveValue.m_vaoID;
		moveValue.m_vaoID = -1;
		return *this;
	}

	~State()
	{
		glDeleteVertexArrays(1, &m_vaoID);
	}

	inline void UseState() const
	{
		glBindVertexArray(m_vaoID);
	}

	GLuint GetStateID() const
	{
		return m_vaoID;
	}

	template<typename Fn, typename ...Args>
	void RunInState(Fn fn, Args ...args)
	{
		UseState();
		fn(std::forward(args)...);
		ClearState();
	}

	inline static void ClearState()
	{
		glBindVertexArray(0);
	}

private:
	GLuint m_vaoID;
	// std::vector<>
};

#endif // STATE_H
