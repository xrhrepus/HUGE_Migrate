#include "Precompiled.h"
#include "MetaRegistration.h"

#include "GameObject.h"
#include "Component.h"

#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "MeshRenderer.h"

#include "CameraService.h"
#include "TerrainService.h"
#include "MeshService.h"
#include "RenderMaterialService.h"


void H::StaticMetaRegistration()
{
	META_REGISTRY(H::GameObject);
	META_REGISTRY(H::Component);

	META_REGISTRY(H::ColliderComponent);
	META_REGISTRY(H::TransformComponent);
	META_REGISTRY(H::MeshRenderer);

	META_REGISTRY(H::CameraService);
	META_REGISTRY(H::TerrainService);
	META_REGISTRY(H::MeshService);
	META_REGISTRY(H::RenderMaterialService);

}