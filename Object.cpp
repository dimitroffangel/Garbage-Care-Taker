#include "Object.h"

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
