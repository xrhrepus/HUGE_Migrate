//======================================
//
//
//======================================
#ifndef HUGE_HUGEEDITOR_H
#define HUGE_HUGEEDITOR_H

namespace H
{
	class World;
	class GameObject;
	class Service;

	class Editor
	{
	public:
		Editor(World& world);
	public:

		void ShowMenuBar();
		void ShowWindowWithDockSpace();
	public:

		void ShowSceneView(const RenderTarget& rt);
		void ShowWorldView();
		void ShowInspectorView();

	private:
		
		World& mWorld;

		Service* mSelectedService = nullptr;
		GameObject* mSelectedGameObject = nullptr;
 
	};
}
 

#endif // !HUGE_HUGEEDITOR_H

