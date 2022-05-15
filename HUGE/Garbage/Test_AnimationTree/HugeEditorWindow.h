#pragma once
#include <HUGE/Inc/HUGE.h>

class HugeEditorWindow
{
public:
	HugeEditorWindow();
	~HugeEditorWindow();

	void SetEditorWindow(const ID3D11ShaderResourceView* view);
	void ShowWindow();


private:
	ID3D11ShaderResourceView* mEditorWindow = nullptr;
};

 