#ifndef OBJECT_H_GUARD
#define OBJECT_H_GUARD

#include <iostream>

#ifdef _WIN64
	using Int = unsigned long long;
#else
	using Int = unsigned;
#endif

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

#endif