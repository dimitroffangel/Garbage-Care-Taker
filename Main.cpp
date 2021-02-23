#include <iostream>

#include "GarbageCollector.h"
#include "SharedHandler.h"

class RaidMission;
class Player
{
public:
	std::weak_ptr<RaidMission> Mission;
	Player(const std::shared_ptr<RaidMission>& mission)
		: Mission(mission) {}
};

class RaidMission
{
public:
	std::vector<std::weak_ptr<Player>> Players;
	void AddPlayer(const std::shared_ptr<Player>& p)
	{
		Players.push_back(p);
	}
};

class Foo : public ObjectStaticArray
{
public:
	Object fooInt;

	Foo()
		:ObjectStaticArray(1, {&fooInt})
	{

	}
};

class DerivedFoo0 : public Foo
{
public:
	double fooDouble = 42.42;
};

class DerivedFoo : public Foo
{
public:
	std::string fooString = "foo";
};

int main()
{
	Foo* foo = new Foo();



	auto mission = std::make_shared<RaidMission>();

	{
		auto player = std::make_shared<Player>(mission);
		mission->AddPlayer(player);
	}

	
	{
		SharedHandler<int> sharedHandler = new int(5);

		sharedHandler.GetReferenceCounter();

		WeakHandler<int> weakHandler = sharedHandler;

		{
			SharedHandler<int> newShared = weakHandler.LockHandler();
			std::cout << newShared.GetReferenceCounter() << " VS " << sharedHandler.GetReferenceCounter() << '\n';
			*newShared = 42;
		}

		std::cout << sharedHandler.GetReferenceCounter() << '\n';
	}
	Value val = true;
	val.m_Type = ValueType::ValueBool;

	Value val2;
	val2.m_ObjectPtr = new Object;
	val2.m_ObjectPtr->value = &val;

	std::cout << val;



	return 0;
}