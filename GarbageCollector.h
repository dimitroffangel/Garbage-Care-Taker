#ifndef GARBAGECOLLECTOR_H_GUARD
#define GARBAGECOLLECTOR_H_GUARD

#define DEBUG_TRACE_EXECUTION

#define DEBUG_STRESS_GC
#define DEBUG_LOG_GC


#ifdef _WIN64
	using Int = unsigned long long;
#else
	using Int = unsigned;
#endif

#include <vector>
#include <iostream>

class Allocator;

enum ValueType
{
	ValueDouble, 
	ValueInt,
	ValueBool,
	ValueObject,
	ValueArray,
	ValueUndefined
};

class Object;

class Value
{
public:
	ValueType type = ValueUndefined;
	union
	{
		bool boolean;
		double realNumber;
		Object* object;
	};

	Value()
	{

	}

	Value(const bool value)
		:type(ValueType::ValueBool)
	{
		type = ValueType::ValueBool;
		boolean = value;
	}
	
	operator bool() const
	{
		if (type != ValueType::ValueBool)
		{
			std::cout << "Value::operator bool() casting value to bool when the value is not bool" << '\n';
			return false;
		}
		
		return boolean;
	}
};

std::ostream& operator<<(std::ostream& os, const Value& value);

class Object
{
public:
	Value value;
	bool isMarked = false;
	// TODO reference to another object ???

	Int virtual GetSize() const;
};

class ObjectStaticArray : public Object
{
public:
	size_t size;
	Value* values;

	Int virtual GetSize() const override;
};

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
