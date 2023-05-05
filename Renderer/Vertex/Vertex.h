#pragma once

#include "Defines.h"

namespace Bear
{
	namespace GUI
	{
		struct Vertex
		{
			UInt ID;
			Vec2 position;
			Vec4 color;

			Vertex();
			Vertex(const Vec2& position, const Vec4& color);

			static void NextID();
		};
	}
}