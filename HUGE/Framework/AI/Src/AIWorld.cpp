#include "Precompiled.h"
#include "Agent.h"
#include "AIWorld.h"
#include "Graphics\Inc\SimpleDraw.h"

using namespace H;
using namespace H::AI;

 void H::AI::AIWorld::Initialize(int row, int column, float cellSize)
{
	mGrid.row = row;
	mGrid.column = column;
	mGrid.cellSize = cellSize;
	mGrid.grids.resize(row * column);

}
 void H::AI::AIWorld::Update(float deltatime)
 {
	 //for (int i = 0; i < mGrid.row; i++)
	 //{
		// H::Graphics::SimpleDraw::AddScreenLine({ 0.0f,i * mGrid.cellSize }, { 1280.0f,i * mGrid.cellSize }, H::Graphics::Colors::Cyan);
	 //}
	 //for (int i = 0; i < mGrid.column; i++)
	 //{
		// H::Graphics::SimpleDraw::AddScreenLine({ i * mGrid.cellSize,0.0f }, { i * mGrid.cellSize , 1280.0f }, H::Graphics::Colors::Cyan);
	 //}
	 ComputeNeighborForAgents();
	 runningTime += deltatime;
}

AIWorld::Agents H::AI::AIWorld::GetNeighborhood(const H::Math::Circle & range)
{
	Agents ret;

	//find agents in 9 cells
	int a_row, a_col = 0;
	//where circle at ?
	a_row = static_cast<int>(range.centre.y / mGrid.cellSize);
	a_col = static_cast<int>(range.centre.x / mGrid.cellSize);
	int cIndex = a_row * mGrid.column + a_col;
	//compute how many cells should cross
	int crossCell = static_cast<int>(range.radius * 2.0f / mGrid.cellSize);
 	//compute all cell index
	std::vector<int> indecies;
	indecies.reserve(4 * (crossCell + crossCell * crossCell) + 1);//max amount;
	
	const int minX = (a_col - crossCell) < 0 ? 0 : (a_col - crossCell);
	const int maxX = (a_col + crossCell) >= mGrid.column ? mGrid.column - 1 : (a_col + crossCell);
	const int minY = (a_row - crossCell) < 0 ? 0 : (a_row - crossCell);
	const int maxY = (a_row + crossCell) >= mGrid.row ? mGrid.row - 1 : (a_row + crossCell);
	int tIndex = 0;
	//add all agent within (range.radius)^2 square area 
	for (int i = minX; i < minX +( maxX - minX) + 1; i++)
	{
		for (int j = minY; j < minY + (maxY - minY) + 1; j++)
		{
			tIndex = j * mGrid.column + i;
			for (auto a : mGrid.grids[tIndex])
			{
				ret.push_back(a);
			}
		}
	}

 
	return std::move(ret);
}
void AIWorld::ComputeNeighborForAgents()
{
	//mPreviousGrid = std::move(mGrid);
	mGrid.grids.clear();
	mGrid.grids.resize(mGrid.row * mGrid.column);

	for (auto a : mAgents)
	{
		int a_row, a_col = 0;
		a_row = static_cast<int>(a->position.y / mGrid.cellSize);
		a_col = static_cast<int>(a->position.x / mGrid.cellSize);
		if (a_row < 0 || a_row >= mGrid.row || a_col < 0 || a_col >= mGrid.column)
		{
			continue;
		}
		mGrid.grids[static_cast<int>(a_row * mGrid.column + a_col)].push_back(a);
	}

}

void AIWorld::AddObstabcles(const H::Math::Circle & obs)
{
	mObstacles.push_back(obs);
}

void AIWorld::AddWall(const H::Math::LineSegment2D & wall)
{
	mWalls.push_back(wall);
}

void H::AI::AIWorld::ResigterAgent(Agent * agent)
{
	mAgents.push_back(agent);
}

void H::AI::AIWorld::UnResigterAgent(Agent * agent)
{
	auto iter = std::find(mAgents.begin(), mAgents.end(), agent);
	if (iter != mAgents.end())
	{
		//std::swap(*iter, mAgents.back());
		std::iter_swap(iter, mAgents.end() - 1);
		mAgents.pop_back();
	}

}

const Agents & H::AI::AIWorld::GetAllAgents() const
{
	return mAgents;
}
