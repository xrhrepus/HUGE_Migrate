#ifndef HUGE_GAMEOBJECTFACTORY_H
#define HUGE_GAMEOBJECTFACTORY_H
#include "GameObject.h"

namespace H
{
	class GameObjectFactory
	{
	public:
		GameObjectFactory(GameObjectAllocator& allocator);

		GameObject* Create(const char* templateFileName);
		void Destroy(GameObject* gameobject);

	private:
		GameObjectAllocator& mGameObjectAllocator;
	};
}

#endif // !HUGE_GAMEOBJECTFACTORY_H

