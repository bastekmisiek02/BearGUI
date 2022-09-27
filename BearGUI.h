#pragma once

#include "Defines.h"

#include "GUIObjects/GUIBase.h"

namespace Bear
{
	namespace GUI
	{
		class GUI
		{
		private:
			friend class Base;
		private:
			static DynamicArray<Base*> objects;
		public:
			static void Init(Window* window, void* data);
			static void Render(void* data);
			static void Update();
			static void Clean();
		public:
			static void Resize(const ULInt& newWidth, const ULInt& newHeight);
		};
	}
}