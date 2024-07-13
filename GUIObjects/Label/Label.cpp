#include "Label.h"

namespace Bear
{
	namespace GUI
	{
		Label::Label()
			: Base()
		{
		}

		Label::Label(Base* parent, const IVec2& position, const IVec2& size, const Vec4& color, const String& text, const char& anchor, const char& textAlign, const String& name)
			: Base(parent, position, size, color, text, color, color, anchor, textAlign, name)
		{
			Init();
		}
	}
}