#pragma once

#include <vector>

struct RenderPixelData
{
	int x, y;
	int r, g, b;

	RenderPixelData(int x, int y, int r, int g, int b) :
		x(x), y(y), r(r), g(g), b(b)
	{}
};

struct RenderTLS
{
	std::vector<RenderPixelData> renderPixelData;
};
