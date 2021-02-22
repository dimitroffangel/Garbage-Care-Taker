#ifndef GARBAGECOLLECTOR_H_GUARD
#define GARBAGECOLLECTOR_H_GUARD

#define DEBUG_TRACE_EXECUTION

#define DEBUG_STRESS_GC
#define DEBUG_LOG_GC



#include <vector>
#include "Object.h"

class Allocator;


class GarbageCollector
{
	std::vector<Int> objectsIndecixes;
	std::vector<Int> addressedUsed;

	//virtual Object* Allocate(const size_t) = 0;
	void Reallocate(void* pointer, const size_t oldSize, const size_t newSize);
	void CollectGarbage();
	//void MarkRoots();
	//void MarkValue(Value);
	//void MarkObject(Object* object);
	void SweepObjects(Allocator*);
};

#endif
