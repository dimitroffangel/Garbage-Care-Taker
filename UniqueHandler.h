#ifndef UNIQUEHANDLER_H_GUARD
#define UNIQUEHANDLER_H_GUARD

#include <assert.h>
#include <functional>
#include <vector>

template<class T>
class UniqueHandler
{
public:
	UniqueHandler(T* m_Pointer)
		: m_Pointer(m_Pointer)
	{
		m_ReferenceCounter = new size_t(1);
	}

	~UniqueHandler()
	{
		m_AfterDeletionDelegate(m_Pointer);
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
			return;
		}

		return m_Pointer;
	}

private:
	T* m_Pointer = nullptr;
	std::function<void(void* pointer, size_t sizeOfChunk)> m_AfterDeletionDelegate;
};

#endif