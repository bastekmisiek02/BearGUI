#include "Label.h"

namespace Bear
{
	namespace GUI
	{
		Label::Label()
			: Base()
		{
		}

		Label::Label(const Defaults& settings)
			: Base(settings.parent, settings.position, settings.size, settings.defaultColor, settings.text, settings.defaultColor, settings.defaultColor, settings.anchor, settings.textAlign, settings.name)
		{
			Init();
		}
	}
}