#ifndef HUGE_QUADTREE_H
#define HUGE_QUADTREE_H

//#include <HUGE/Inc/HUGE.h>

namespace H
{
	class Quadtree
	{
	public:
		struct Node
		{
			std::array<std::unique_ptr<Node>, 4> children;
			std::vector<uint32_t> indices;
			H::Math::AABB aabb;
			bool isLeaf = false;
			void DrawNode()
			{
				SimpleDraw::AddBox(aabb, Colors::Cyan);
			}
		};

		using Visitor = std::function<bool(Node*)>;

		Quadtree() = default;
		
		void Initialize(const H::Graphics::Mesh* mesh, uint32_t levels);
		void Visit(Visitor& visitor);
		bool Intersect(const Math::Ray& ray, float& distance)const;

		//void Terminate() {}
		//void Update(float deltatime) {}
		//void Render() {}
		//void DebugUI() {}

	private:
		void GenerateNodeRecursive(std::unique_ptr<Node>& node, const Math::AABB& aabb, uint32_t levels);
		void VisitRecursive(Node* node, Visitor& visitor);
		bool IntersectRecursive(const Node* node, const Math::Ray& ray, float& distance)const;
		enum Quadrant {BL,BR,TL,TR};
		const H::Graphics::Mesh* mMesh = nullptr;
		std::unique_ptr<Node> mRoot = nullptr;
	};

}
 
#endif // !HUGE_QUADTREE_H

