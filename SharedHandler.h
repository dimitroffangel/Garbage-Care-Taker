#ifndef SHAREDHANDLER_H_GUARD
#define SHAREDHANDLER_H_GUARD

#include <assert.h>
#include <functional>
#include <vector>

template<class T>
class SharedHandler
{
public:
	SharedHandler(T* m_Pointer)
		: m_Pointer(m_Pointer)
	{
		m_ReferenceCounter = new size_t(1);
	}

	SharedHandler(T* pointer, size_t* counterPointer)
	{
		m_Pointer = pointer;

		assert(counterPointer != nullptr && *counterPointer >= 0);
		
		m_ReferenceCounter = counterPointer;
		++(*m_ReferenceCounter);
	}

	SharedHandler(const SharedHandler& other):
		m_ReferenceCounter(other.m_ReferenceCounter),
		m_Pointer(other.m_Pointer)
	{
		++(*m_ReferenceCounter);
	}


	~SharedHandler()
	{
		if (*m_ReferenceCounter == 1)
		{
			--(*m_ReferenceCounter);
			m_AfterDeletionDelegate(m_Pointer);
			delete m_Pointer;
			delete m_ReferenceCounter;
			return;
		}

		--(*m_ReferenceCounter);
	}

	SharedHandler* operator=(const SharedHandler& rhs)
	{
		if (this != &rhs)
		{
			// if it is the last object remembering that object, annihilate it
			if (m_ReferenceCounter == 1)
			{
				--(*m_ReferenceCounter);
				m_AfterDeletionDelegate(m_Pointer);
				delete m_Pointer;
				delete m_ReferenceCounter;
			}

			assert(m_ReferenceCounter != 0);

			// m_ReferenceCounter >= 1
			--(*m_ReferenceCounter);

			m_ReferenceCounter = rhs.m_ReferenceCounter;
			m_Pointer = rhs.m_Pointer;
		}

		return *this;
	}

	operator bool() const
	{
		return m_Pointer != nullptr;
	}

	T* operator->() const
	{
		if (m_Pointer == nullptr)
		{
			std::cout << "SharedHandler::operator-> m_Pointer is nullptr" << '\n';
			return;
		}

		return m_Pointer;
	}

	int GetReferenceCounter() const
	{
		return m_ReferenceCounter;
	}

private:
	T* m_Pointer = nullptr;
	size_t* m_ReferenceCounter = nullptr;
	std::function<void(void* pointer, size_t sizeOfChunk)> m_AfterDeletionDelegate;
};

template<class T>
class WeakHandler
{
public:
	WeakHandler(const SharedHandler<T>& sharedHandler)
		: m_Pointer(m_Pointer)
	{
		m_WeakReferenceCounter = new size_t(0);
		m_SharedReferenceCounter = sharedHandler.m_ReferenceCounter;

		if (*m_SharedReferenceCounter > 0)
		{
			++(*m_WeakReferenceCounter);
		}
	}

	WeakHandler(const WeakHandler& other) :
		m_SharedReferenceCounter(other.m_SharedReferenceCounter),
		m_WeakReferenceCounter(other.m_WeakReferenceCounter),
		m_Pointer(other.m_Pointer)
	{
		++(*m_WeakReferenceCounter);
	}


	~WeakHandler()
	{
		if (*m_WeakReferenceCounter== 1)
		{
			delete m_WeakReferenceCounter;
			return;
		}

		--(*m_WeakReferenceCounter);
	}

	WeakHandler* operator=(const WeakHandler& rhs)
	{
		if (this != &rhs)
		{
			// if it is the last object remembering that object, annihilate it
			if (m_WeakReferenceCounter == 1)
			{
				delete m_WeakReferenceCounter;
			}

			assert(m_WeakReferenceCounter != 0);

			// m_WeakReferenceCounter >= 1
			--(*m_WeakReferenceCounter);

			m_SharedReferenceCounter = rhs.m_SharedReferenceCounter;
			m_WeakReferenceCounter = rhs.m_WeakReferenceCounter;
			m_Pointer = rhs.m_Pointer;
		}

		return *this;
	}

	bool HasExpired() const
	{
		return m_SharedReferenceCounter != nullptr && *m_SharedReferenceCounter > 0;
	}

	SharedHandler LockHandler() 
	{
		return SharedHandler(m_Pointer, m_SharedReferenceCounter);
	}

private:
	T* m_Pointer;
	size_t* m_SharedReferenceCounter;
	size_t* m_WeakReferenceCounter;
};

#endif