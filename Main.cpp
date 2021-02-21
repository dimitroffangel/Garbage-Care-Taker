#include <iostream>

#include "GarbageCollector.h"
#include "SharedHandler.h"

int main()
{
	Value val = true;
	val.type = ValueType::ValueBool;

	Value val2;
	val2.object = new Object;
	val2.object->value = &val;

	std::cout << val;

	return 0;
}