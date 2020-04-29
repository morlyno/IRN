#pragma once
#include "Graphics.h"

class Overlay
{
public:
	Overlay( Graphics& gfx );
	void DrawHealBar( const float& hp ) const;
private:
	const Vec2i HPBarPos = { 10,10 };
	static constexpr int HPBarWidht = 150;
	static constexpr int HPBarHeight = 10;
	static constexpr Color HPColor = Colors::Red;
private:
	Graphics& gfx;
};