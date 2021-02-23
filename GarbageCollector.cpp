#include "GarbageCollector.h"

#include "Allocator.h"

#include <iostream>
#include <assert.h>
#include <stack>

void GarbageCollector::Reallocate(void* pointer, const size_t oldSize, const size_t newSize)
{
	if (newSize > oldSize)
	{
#ifdef DEBUG_STESS_GC
		CollectGarbage();
#endif
	}

	// TODO
}

void GarbageCollector::CollectGarbage()
{
#ifdef DEBUG_LOG_GC
	std::cout << "GC begins ..." << '\n';
#endif

	//MarkRoots();

	std::stack<Int> indexesToIterate;

	for (size_t i = 0; i < objectsIndecixes.size(); ++i)
	{
		indexesToIterate.push(objectsIndecixes[i]);
	}

	while (!indexesToIterate.empty())
	{
		Object* currentObject = (Object*)(indexesToIterate.top());
		indexesToIterate.empty();

		assert(currentObject != nullptr);

		currentObject->isMarked = true;

		if (currentObject->value.m_Type == ValueType::ValueObject && 
			currentObject->value.m_ObjectPtr != nullptr &&
			!currentObject->value.m_ObjectPtr->isMarked)
		{
			indexesToIterate.push((Int)(currentObject->value.m_ObjectPtr));
		}
	}

#ifdef DEBUG_LOG_GC
	std::cout << "GC ends ..." << '\n';
#endif

}

//void GarbageCollector::MarkRoots()
//{
//	for (size_t i = 0; i < objectsIndecixes.size(); ++i)
//	{
//		Object* currentObject = (Object*)(static_cast<char*>(memory) + objectsIndecixes[i]);
//
//		assert(currentObject != nullptr);
//
//		//MarkValue(*currentObject);
//
//		currentObject->isMarked = true;
//	}
//}
//
//void GarbageCollector::MarkObject(Object* object)
//{
//	if (object == nullptr)
//	{
//		std::cout << "GarbageCollector::MarkObject(Object* object) object == nullptr" << '\n';
//		return;
//	}
//
//#ifdef DEBUG_LOG_GC
//	std::cout << " marked" << (void*)object << '\n';
//
//	std::cout << "Value: " << object->value << '\n';
//#endif
//
//	object->isMarked = true;
//}

void GarbageCollector::SweepObjects(Allocator* fromAlloc, Allocator* toAlloc = nullptr)
{
	if (fromAlloc == nullptr)
	{
		std::cout << "StopCopyGC::SweepObjects(Allocator* fromAlloc, Allocator* toAlloc): fromAlloc == nullptr" << '\n';
		return;
	}

	for (Int i = 0; i < addressedUsed.size(); ++i)
	{
		Object* currentObject = (Object*)addressedUsed[i];

		if (currentObject == nullptr)
		{
			std::cout << "GarbageCollector::SweepObjects() currentObject == nullptr" << '\n';
			continue;
		}

		if (currentObject->isMarked == true)
		{
			continue;
		}

		// remove the address from the addressedUsedBlock
		addressedUsed.erase(addressedUsed.begin() + i);
		--i;
		// remove the memory
		const ValueType objectValueType = currentObject->value.m_Type;

		if (objectValueType == ValueType::ValueBool || objectValueType == ValueType::ValueDouble ||
			objectValueType == ValueType::ValueInt || objectValueType == ValueType::ValueObject)
		{
			// remove the memory
			fromAlloc->Deallocate(currentObject, currentObject->GetSize());
		}

		else
		{
			std::cout << "GarbageCollector::SweepObjects(Allocator* allocator)::Array not implemented yet..." << '\n';
			fromAlloc->Deallocate(currentObject, currentObject->GetSize());
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Value& value)
{
	if (value.m_Type == ValueBool)
	{
		os << value.m_Boolean;
	}

	else if (value.m_Type == ValueType::ValueDouble)
	{
		os << value.m_RealNumber;
	}

	else if (value.m_Type == ValueType::ValueInt)
	{
		os << value.m_RealNumber;
	}

	else if (value.m_Type == ValueType::ValueObject)
	{
		os << value.m_ObjectPtr;
	}

	return os;
}

Int Object::GetSize() const
{
	return sizeof(value) + sizeof(isMarked);
}

Int ObjectStaticArray::GetSize() const
{
	return sizeof(Value) * m_Size + Object::GetSize();
}
