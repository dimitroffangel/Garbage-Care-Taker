#include "StopAndCopySweepMark.h"

#include "Allocator.h"

#include <unordered_map>
#include <assert.h>

void StopCopyGC::SweepObjects(Allocator* fromAlloc, Allocator* toAlloc)
{
	if (fromAlloc == nullptr)
	{
		std::cout << "StopCopyGC::SweepObjects(Allocator* fromAlloc, Allocator* toAlloc): fromAlloc == nullptr" << '\n';
		return;
	}

	if (toAlloc == nullptr)
	{
		std::cout << "StopCopyGC::SweepObjects(Allocator* fromAlloc, Allocator* toAlloc): toAlloc == nullptr" << '\n';
		return;
	}

	std::unordered_map<Int, Int> addressToNewAddress;
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
			const Int newAddress = (Int)toAlloc->Allocate(currentObject->GetSize());
			Object* result = new ((void*)newAddress) Object();

			addressToNewAddress[(Int)currentObject] = newAddress;
		}

		// remove the memory
		const ValueType objectValueType = currentObject->value.m_Type;

		if (objectValueType == ValueType::ValueBool || objectValueType == ValueType::ValueDouble ||
			objectValueType == ValueType::ValueInt || objectValueType == ValueType::ValueObject)
		{
			// remove the memory
			currentObject->~Object();
			fromAlloc->Deallocate(currentObject, currentObject->GetSize());
		}

		else if (objectValueType == ValueType::ValueArray)
		{
			auto objectArray = static_cast<ObjectStaticArray*>(currentObject);


			for (const auto& element : objectArray->m_Values)
			{
				if (element == nullptr)
				{
					continue;
				}

				element->~Object();
			}

			fromAlloc->Deallocate(currentObject, currentObject->GetSize());
		}

		else
		{
			std::cout << "GarbageCollector::SweepObjects(Allocator* allocator)::Array not implemented yet..." << '\n';
			fromAlloc->Deallocate(currentObject, currentObject->GetSize());
		}
	}

	// update references from the allocated space
	addressedUsed.clear();
	rootIndexes.clear();
	size_t i = 0;

	for (const auto pair : addressToNewAddress)
	{
		addressedUsed.push_back(pair.second);
		rootIndexes.push_back(i++);

		Object* const currentObject = (Object*)pair.first;

		// cannot be nullptr from ahead
		assert(currentObject != nullptr);

		if (currentObject->value == ValueType::ValueObject)
		{
			const Int pointeeNewAddress = addressToNewAddress[(Int)currentObject->value.m_ObjectPtr];

			currentObject->value.m_ObjectPtr = (Object*)pointeeNewAddress;
		}

		// problem if there is a pointer outside the allocated space who points to such location
	}

	for (size_t i = 0; i < addressedUsed.size(); ++i)
	{
		Object* currentObject = (Object*)addressedUsed[i];

		if (currentObject == nullptr)
		{
			std::cout << "GarbageCollector::SweepObjects() currentObject == nullptr" << '\n';
			continue;
		}

		currentObject->isMarked = false;
	}
}
