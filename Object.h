#ifndef OBJECT_H_GUARD
#define OBJECT_H_GUARD

#include <iostream>
#include <vector>
#include <assert.h>

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
	ValueClass,
	ValueUndefined
};

class Object;

class Value
{
public:
	ValueType m_Type = ValueUndefined;
	union
	{
		bool m_Boolean;
		double m_RealNumber;
		Object* m_ObjectPtr;
	};

	Value()
	{

	}

	Value(const bool value)
		: m_Type(ValueType::ValueBool),
		  m_Boolean(value)
	{

	}

	Value(const int value)
		: m_Type(ValueType::ValueInt),
		  m_RealNumber(value)
	{
	}

	Value(const double value)
		: m_Type(ValueType::ValueDouble),
		m_RealNumber(value)
	{
	}

	Value(Object* objectPtr, const ValueType& valueType)
		:m_ObjectPtr(objectPtr)
	{
		assert(valueType == ValueType::ValueArray || valueType == ValueType::ValueObject || valueType == ValueType::ValueArray);

		m_Type = valueType;
	}

	operator bool() const
	{
		if (m_Type == ValueType::ValueBool)
		{
			return m_Boolean;
		}

		if (m_Type == ValueType::ValueArray)
		{
			return m_ObjectPtr != nullptr;
		}

		if (m_Type == ValueType::ValueDouble || m_Type == ValueType::ValueInt)
		{
			return m_Type;
		}

		if (m_Type == ValueType::ValueUndefined)
		{
			return false;
		}

		std::cout << "Value::operator bool() -> Undetermined ValueType" << '\n';

		return false;
	}
};

std::ostream& operator<<(std::ostream& os, const Value& value);

class Object
{
public:
	Value value;
	bool isMarked = false;
	// TODO reference to another object ???

	virtual ~Object()
	{

	}

	Int virtual GetSize() const;
};

class ObjectStaticArray : public Object
{
public:
	const size_t m_Size;
	const std::vector<Object*> m_Values;

	ObjectStaticArray(const size_t size, std::initializer_list<Object*> initList)
		:	m_Size(size),
			m_Values(initList)
	{

	}

	Int virtual GetSize() const override;
};

#endif