#ifndef HUGE_AI_AIWORLD_H
#define HUGE_AI_AIWORLD_H

namespace H::AI
{
	class Agent;
	struct Grid
	{
		std::vector<Agents> grids;
		int row;
		int column;
		float cellSize;
	};

	class AIWorld
	{
	public:
		using Obstacles = std::vector<H::Math::Circle>;
		using Walls = std::vector<H::Math::LineSegment2D>;//lineseg,emt
		using Agents = std::vector<Agent*>;
		//using Grid = std::vector<int>;

		AIWorld() = default;
		~AIWorld() = default;

		Agents GetNeighborhood(const H::Math::Circle& range);

		void AddObstabcles(const H::Math::Circle& obs);
		const Obstacles& GetObstabcles() const { return mObstacles; }

		void AddWall(const H::Math::LineSegment2D& wall);
		const Walls& GetWalls() const { return mWalls; }

		void ResigterAgent(  Agent* agent);
		void UnResigterAgent(  Agent* agent);

		const Agents& GetAllAgents()const;

		void Initialize(int row, int column , float cellSize);
		void Update(float deltatime);


		float runningTime = 0.0f;
	private:
		void ComputeNeighborForAgents();

		Obstacles mObstacles;
		Walls mWalls;
		Agents mAgents;
		Grid mGrid;
		//Grid mPreviousGrid;

	};



}
//	namespace H::AI


#endif // !HUGE_AI_AIWORLD_H

