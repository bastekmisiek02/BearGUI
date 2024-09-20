#include "Label.h"

namespace Bear
{
	namespace GUI
	{
		Label::Label()
			: Base()
		{
		}

		Label::Label(const Properties& properties)
			: Base(properties.parent, properties.position, properties.size, properties.defaultColor, properties.text, properties.defaultColor, properties.defaultColor, properties.anchor, properties.textAlign, properties.name)
		{
			Init();
		}
	}
}