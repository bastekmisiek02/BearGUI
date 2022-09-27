#pragma once

#include "Defines.h"

namespace Bear
{
	namespace GUI
	{
		struct Vertex
		{
			Vec2 position;
			Vec4 color;
		};

		class Renderer
		{
		public:
			static void Init(void* data);
			static void Dispose();
		public:
			static void Render(void* data);
		public:
			static void SetViewportInfo(void* info);
		};
	}
}