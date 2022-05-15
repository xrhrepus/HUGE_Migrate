//====================================================================================================
// Filename:	DebugUI.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef HUGE_GRAPHICS_GUI_H
#define HUGE_GRAPHICS_GUI_H

namespace H::Graphics::DebugUI {
	enum class Theme
	{
		classic,
		dark,
		light
	};


void Initialize(HWND window, bool docking, bool multiViewport, Theme theme = Theme::dark);
void Terminate();
void BeginRender();
void EndRender();


void SetTheme(Theme theme);

} // namespace H::Graphics::Gui

#endif // #ifndef INCLUDED_GRAPHICS_GUI_H