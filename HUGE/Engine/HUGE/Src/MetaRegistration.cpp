#include "Precompiled.h"
#include "MetaRegistration.h"

#include "GameObject.h"
#include "Component.h"

#include "ColliderComponent.h"
#include "TransformComponent.h"

#include "CameraService.h"
#include "TerrainService.h"


void H::StaticMetaRegistration()
{
	META_REGISTRY(H::GameObject);
	META_REGISTRY(H::Component);

	META_REGISTRY(H::ColliderComponent);
	META_REGISTRY(H::TransformComponent);

	META_REGISTRY(H::CameraService);
	META_REGISTRY(H::TerrainService);

}