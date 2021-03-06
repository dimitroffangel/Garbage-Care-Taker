#ifndef STOPANDCOPYSWEEPMARK_H_GUARD
#define STOPANDCOPYSWEEPMARK_H_GUARD

#define DEBUG_TRACE_EXECUTION

#define DEBUG_STRESS_GC
#define DEBUG_LOG_GC

#include <vector>
#include "Object.h"
#include "GarbageCollector.h"

class Allocator;

class StopCopyGC : public GarbageCollector
{
	std::vector<Int> objectIndexes;
	std::vector<Int> addressedUsed;

	//virtual Object* Allocate(const size_t) = 0;
	//void MarkRoots();
	//void MarkValue(Value);
	//void MarkObject(Object* object);
	virtual void SweepObjects(Allocator*, Allocator*) override;
};

#endif