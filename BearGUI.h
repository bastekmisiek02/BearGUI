#pragma once

#include "Defines.h"

#include "GUIObjects/GUIBase.h"

namespace Bear
{
	namespace GUI
	{
		class BearGUI
		{
		private:
			friend class Base;
		private:
			static DynamicArray<Base*> objects;
		public:
			static void Init(Window* window, void* data);
			static void Update();
			static void Render();
			static void Clean();
		};
	}
}