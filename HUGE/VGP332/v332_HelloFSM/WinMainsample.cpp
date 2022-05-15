#include <AI/Inc/AI.h>
#include <XEngine.h>

struct Dude
{
	Dude() : stateMachine(*this) {}

	AI::StateMachine<Dude> stateMachine;
	X::Math::Vector2 position;
};

struct DanceState : public AI::State<Dude>
{
	static std::string GetName() { return "Dance"; }

	void Enter(Dude& agent) override
	{
		position = agent.position;
		time = 0.0f;
	}
	void Update(Dude& agent, float deltaTime) override
	{
		time += deltaTime * 10.0f;
		if (time > X::Math::kTwoPi)
			agent.stateMachine.ChangeState("Run");
		else
		{
			agent.position.x = position.x + sin(time * 2.0f) * 30.0f;
			agent.position.y = position.y + cos(time * 2.0f) * 10.0f;
		}
	}
	void Exit(Dude& agent) override
	{
		agent.position = position;
	}

	X::Math::Vector2 position;
	float time;
};

struct RunState : public AI::State<Dude>
{
	static std::string GetName() { return "Run"; }

	void Enter(Dude& agent) override
	{
		destination = X::RandomUnitCircle() * 250.0f + X::Math::Vector2{ 600.0f, 350.0f };
	}
	void Update(Dude& agent, float deltaTime) override
	{
		if (X::Math::DistanceSqr(agent.position, destination) < 10.0f)
			agent.stateMachine.ChangeState("Dance");
		else
		{
			agent.position += X::Math::Normalize(destination - agent.position) * 300.0f * deltaTime;
			X::DrawScreenLine(agent.position, destination, X::Colors::Red);
			X::DrawScreenCircle(destination, 10.0f, X::Colors::Green);
		}
	}
	void Exit(Dude& agent) override	{}

	X::Math::Vector2 destination;
};

Dude dude;
X::TextureId textureId = 0;

void GameInit()
{
	dude.stateMachine.AddState<DanceState>();
	dude.stateMachine.AddState<RunState>();
	textureId = X::LoadTexture("mario.png");
}

void GameCleanup()
{
}

bool GameLoop(float deltaTime)
{
	dude.stateMachine.Update(deltaTime);
	X::DrawSprite(textureId, dude.position);
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start();
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}