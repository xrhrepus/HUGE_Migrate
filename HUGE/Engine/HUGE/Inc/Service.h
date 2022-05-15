#ifndef HUGE_SERVICE_H
#define HUGE_SERVICE_H
namespace H
{
	class World;

	class Service
	{
	public:

		META_CLASS_DECLARE;

		Service() = default;
		virtual ~Service() = default;
		virtual void Initialize(){}
		virtual void Terminate() {}
		virtual void Update(float deltatime) {}
		virtual void Render() {}
		virtual void DebugUI(){}

		World& GetWorld() { return *mWorld; }
		const World& GetWorld() const { return *mWorld; }
 
	private:
		friend class World;
 		World* mWorld = nullptr;
 	};



}

#endif // !HUGE_SERVICE_H


 
