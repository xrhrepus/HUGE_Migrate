//================================
//File name: HUGE.h
//2019
//Description: H's Ultimate Game Engine

//================================


#ifndef HUGE_H
#define HUGE_H

#include "Common.h"
#include "App.h"
#include "AppState.h"

//comp
#include "ColliderComponent.h"
#include "TransformComponent.h"

//world
#include "GameObject.h"
#include "Component.h"
#include "GameObjectFactory.h"
#include "MetaRegistration.h"
#include "World.h"

//editor
#include "Editor.h"

//terrain
#include "Quadtree.h"
#include "Terrain.h"

//service
#include "CameraService.h"
#include "TerrainService.h"
#include "SkydomeService.h"

namespace H { extern App sApp; }

namespace HugeApp
{

	template<class T>
	void AddState(std::string name)
	{
		H::sApp.AddState<T>(name);
	}
	void ChangeState(std::string name);
	void Run(H::AppConfig appConfig);


}



#endif // !HUGE_H
