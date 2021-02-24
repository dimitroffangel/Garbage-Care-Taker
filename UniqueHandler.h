#ifndef UNIQUEHANDLER_H_GUARD
#define UNIQUEHANDLER_H_GUARD

#include <assert.h>
#include <functional>
#include <vector>

#include "Allocator.h"

template<class T>
class UniqueHandler
{
public:
	UniqueHandler(T* m_Pointer)
		: m_Pointer(m_Pointer)
	{

	}

	~UniqueHandler()
	{
		if (m_Pointer == nullptr)
		{
			return;
		}

		m_Pointer->~T();

		if (m_Allocator != nullptr)
		{
			m_Allocator->Deallocate(m_Pointer, sizeof(T));
			return;
		}

		delete m_Pointer;
	}

	UniqueHandler* operator=(const UniqueHandler& rhs) = delete;

	operator bool() const
	{
		return m_Pointer != nullptr;
	}

	T* operator->() const
	{
		if (m_Pointer == nullptr)
		{
			std::cout << "UniqueHandler::operator-> m_Pointer is nullptr" << '\n';
			return nullptr;
		}

		return m_Pointer;
	}

	T& operator*() const
	{
		if (m_Pointer == nullptr)
		{
			std::cout << "UniqueHandler::operator-> m_Pointer is nullptr" << '\n';
		}

		return *m_Pointer;
	}

private:
	T* m_Pointer = nullptr;
	Allocator* m_Allocator = nullptr;
};

#endif