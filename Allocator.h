#pragma once
class Allocator
{
public:
	virtual void* Allocate(const size_t size) = 0;
	virtual void* Deallocate(void* pointer, const size_t size) = 0;
	virtual void* Deallocate(void* pointer) = 0;
};

