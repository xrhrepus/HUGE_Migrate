#include "Precompiled.h"
#include "Quadtree.h"

using namespace H;
 
void H::Quadtree::Initialize(const H::Graphics::Mesh * mesh, uint32_t levels)
{
	mMesh = mesh;

	Math::AABB bound = Graphics::MeshBuilder::ComputeBound(*mesh);
	bound.mCentre.y = 0.0f;
	bound.mExtend.y = 0.0f;

	GenerateNodeRecursive(mRoot, bound, levels);

	const auto& vertices = mesh->vertices;
	const auto& indices = mesh->indices;
	for (size_t i = 0; i < indices.size(); i+=3)
	{
		const auto& a = vertices[indices[i + 0]].position;
		const auto& b = vertices[indices[i + 1]].position;
		const auto& c = vertices[indices[i + 2]].position;

		const float minX = Math::Minf(Math::Minf(a.x, b.x), c.x);
		const float minY = Math::Minf(Math::Minf(a.y, b.y), c.y);
		const float minZ = Math::Minf(Math::Minf(a.z, b.z), c.z);

		const float maxX = Math::Maxf(Math::Maxf(a.x, b.x), c.x);
		const float maxY = Math::Maxf(Math::Maxf(a.y, b.y), c.y);
		const float maxZ = Math::Maxf(Math::Maxf(a.z, b.z), c.z);

		const Math::AABB aabb = Math::AABB::FromMinMax({ minX,minY,minZ }, { maxX,maxY ,maxZ });

		Visitor visitor = [a, b, c, aabb, i](Node* node)
		{
			if (!Math::Intersect(aabb, node->aabb))
				return false;

			if (!node->isLeaf)
				return true;

			if (Math::Intersect(a,node->aabb)||
				Math::Intersect(b, node->aabb) ||
				Math::Intersect(c, node->aabb) ||
				Math::Intersect((a + b)*0.5f, node->aabb) ||
				Math::Intersect((c + b)*0.5f, node->aabb) ||
				Math::Intersect((a + c)*0.5f, node->aabb) 
				)
			{
				node->indices.push_back(i);

			}
			return false;

		};

		Visit(visitor);
	}

}



void H::Quadtree::GenerateNodeRecursive(std::unique_ptr<Node>& node, const Math::AABB & aabb, uint32_t levels)
{
	ASSERT(levels > 0,"[H::Quadtree::GenerateNodeRecursive] levels = 0");

	node = std::make_unique<Node>();
	node->aabb = aabb;

	if (--levels == 0)
	{
		node->isLeaf = true;
	}
	else
	{
		const Math::Vector3& center = aabb.mCentre;
		const Math::Vector3& extend = aabb.mExtend;

		const Math::Vector3 half = { extend.x*0.5f,extend.y,extend.z*0.5f };
		const Math::AABB bl = { {center.x- half.x,center.y,center.z + half.z},half };
		const Math::AABB br = { {center.x+ half.x,center.y,center.z + half.z},half };
		const Math::AABB tl = { {center.x- half.x,center.y,center.z - half.z},half };
		const Math::AABB tr = { {center.x+ half.x,center.y,center.z - half.z},half };

		GenerateNodeRecursive(node->children[BL], bl, levels);
		GenerateNodeRecursive(node->children[BR], br, levels);
		GenerateNodeRecursive(node->children[TL], tl, levels);
		GenerateNodeRecursive(node->children[TR], tr, levels);

	}
}
void H::Quadtree::Visit(Visitor& visitor)
{
	VisitRecursive(mRoot.get(), visitor);
}

void H::Quadtree::VisitRecursive(Node * node, Visitor & visitor)
{
 	if (visitor(node) && !node->isLeaf)
	{
		for (auto& child : node->children)
		{
			VisitRecursive(child.get(), visitor);
		}
	}
}
bool H::Quadtree::Intersect(const Math::Ray & ray, float & distance) const
{
	if(mRoot == nullptr)
		return false;

	distance = std::numeric_limits<float>::max();
	return IntersectRecursive(mRoot.get(), ray, distance);
}
bool H::Quadtree::IntersectRecursive(const Node * node, const Math::Ray & ray, float & distance) const
{
	float distEntry, distExit;
	if (!Math::Intersect(ray, node->aabb, distEntry, distExit))
		return false;
	if (!node->isLeaf)
	{
		return 
			IntersectRecursive(node->children[0].get(), ray, distance) ||
			IntersectRecursive(node->children[1].get(), ray, distance) ||
			IntersectRecursive(node->children[2].get(), ray, distance) ||
			IntersectRecursive(node->children[3].get(), ray, distance);
	}

	bool intersect = false;

	for (size_t i = 0; i < node->indices.size(); ++i)
	{
		uint32_t faceIndex = node->indices[i];
		const auto& a = mMesh->vertices[mMesh->indices[faceIndex + 0]];
		const auto& b = mMesh->vertices[mMesh->indices[faceIndex + 1]];
		const auto& c = mMesh->vertices[mMesh->indices[faceIndex + 2]];

		float d = 0.0f;
		if (Math::Intersect(ray,a.position,b.position,c.position,d))
		{
			distance = Math::Minf(distance, d);
			intersect = true;
		}
	}

	return intersect;
	
}
