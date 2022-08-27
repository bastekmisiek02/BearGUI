#pragma once

#include "Defines.h"

namespace Bear
{
	namespace GUI
	{
		class Renderer
		{
		public:
			static void Init(void* data);
			static void Dispose();
		public:
			static void Render();
		public:
			static void Resize(const ULInt& newWidth, const ULInt& newHeight);
		};
	}
}