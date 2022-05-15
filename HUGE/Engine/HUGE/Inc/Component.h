#ifndef HUGE_COMPONENT_H
#define HUGE_COMPONENT_H

namespace H
{
	class GameObject;

	class Component
	{

	public:
 		META_CLASS_DECLARE
		virtual ~Component() = default;
		virtual void Initialize() {}
		virtual void Terminate() {}
		virtual void Update(float deltatime) {}
		virtual void Render() {}
		virtual void DebugUI() {}


		GameObject& GetOwner() { return *mOwner; }
		const GameObject& GetOwner() const { return *mOwner; }

	private:
		friend class GameObject;
		GameObject* mOwner = nullptr;
	};

}



#endif // !HUGE_COMPONENT_H

