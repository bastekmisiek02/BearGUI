#include "Button.h"

namespace Bear
{
	namespace GUI
	{
		Button::Button()
			: Base()
		{
		}

		Button::Button(const Defaults& settings, const Vec4& clickColor, const Vec4& hoverColor)
			: Base(settings.parent, settings.position, settings.size, settings.defaultColor, settings.text, clickColor, hoverColor, settings.anchor, settings.textAlign, settings.name)
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