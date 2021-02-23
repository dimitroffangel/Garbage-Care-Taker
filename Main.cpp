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

int main()
{
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
	val.type = ValueType::ValueBool;

	Value val2;
	val2.object = new Object;
	val2.object->value = &val;

	std::cout << val;



	return 0;
}