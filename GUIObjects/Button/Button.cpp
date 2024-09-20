#include "Button.h"

namespace Bear
{
	namespace GUI
	{
		Button::Button()
			: Base()
		{
		}

		Button::Button(const Properties& properties, const Vec4& clickColor, const Vec4& hoverColor)
			: Base(properties.parent, properties.position, properties.size, properties.defaultColor, properties.text, clickColor, hoverColor, properties.anchor, properties.textAlign, properties.name)
		{
			Init();
		}

		void Button::AddCallback(const OnMouseClickCallback& callback, void* data)
		{
			callbacks.Add(callback);
			this->data.Add(data);
		}

		void Button::RemoveCallback(const OnMouseClickCallback& callback)
		{
			ULInt index = -1;

			if (callbacks.Exist(callback, &index))
			{
				callbacks.RemoveOnIndex(index, index);
				this->data.RemoveOnIndex(index, index);
			}
		}

		void Button::OnMouseClick(char mouseButton)
		{
			for (ULInt i = 0; i < callbacks.Length(); i++)
				callbacks[i](this, this->data[i]);
		}
	}
}