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
public:
	std::vector<Int> objectsIndecixes;
	std::vector<Int> addressedUsed;

	//virtual Object* Allocate(const size_t) = 0;
	virtual void* Reallocate(void* pointer, const size_t oldSize, const size_t newSize, Allocator* from, Allocator* to);
	virtual void CollectGarbage();
	//void MarkRoots();
	//void MarkValue(Value);
	//void MarkObject(Object* object);
	virtual void SweepObjects(Allocator*, Allocator*);
};

#endif
