#pragma once

#include "GUIObjects/GUIBase.h"

namespace Bear
{
	namespace GUI
	{
		class Label : public Base
		{
		public:
			Label();
			Label(Base* parent, const IVec2& position, const Vec4& color, const String& text, const char& anchor = Anchor::Center | Anchor::Mid, const char& textAlign = Anchor::Center | Anchor::Mid, const String& name = "");
		};
	}
}