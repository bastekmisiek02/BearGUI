#include "GUIBase.h"

#include "Renderer/Renderer.h"

#include "BearGUI.h"

namespace Bear
{
	namespace GUI
	{
		#ifdef BEAR_LIBRARY_INCLUDED
		Base::Base()
			: parent(nullptr), position(), color({ 0.0f, 0.0f, 0.0f, 1.0f }), text(), name(), isVisible(false), anchor(Anchor::Center | Anchor::Mid), textAlign(Anchor::Center | Anchor::Mid), currentColor(&color), isDestroyed(false)
		{
			GUI::objects.Add(this);
		}

		Base::Base(Base* parent, const GraphicsMath::IVec2& position, const GraphicsMath::Vec4& color, const Collections::String& text, const char& anchor, const char& textAlign, const Collections::String& name)
			: parent(parent), position(position), color(color), text(text), name(name), isVisible(true), anchor(anchor), textAlign(textAlign), currentColor(&this->color), isDestroyed(false)
		{
			if (parent)
				parent->childrens.Add(this);

			GUI::objects.Add(this);
		}
		
		Base::~Base()
		{
			if (isDestroyed)
				return;

			if (parent)
			{
				parent->childrens.Remove(this);
				parent = nullptr;
			}
		
			for (auto& children : childrens)
			{
				children->parent = nullptr;
				children->~Base();
			}
		
			childrens.Clear();
		
			GUI::objects.Remove(this);

			isDestroyed = true;
		}
		
		void Base::OnMouseEnter()
		{
		}
		
		void Base::OnMouseExit()
		{
		}
		
		void Base::OnMouseClick(char mouseButton)
		{
		}
		
		const bool Base::IsVisible() const
		{
			return isVisible;
		}
		
		void Base::Hide()
		{
			isVisible = false;
		}
		
		void Base::Show()
		{
			isVisible = true;
		}
		
		Base* Base::GetParent() const
		{
			return parent;
		}
		
		void Base::AddChildren(Base* object)
		{
			if (object->parent)
				object->parent->childrens.Remove(object);

			childrens.Add(object);

			object->parent = this;
		}
		
		void Base::RemoveChildren(Base* object)
		{
			childrens.Remove(object);
			
			object->parent = nullptr;
		}
		
		const DynamicArray<Base*>& Base::GetChildrens() const
		{
			return childrens;
		}
		
		GraphicsMath::IVec2 Base::GetPosition() const
		{
			return position;
		}
		
		void Base::SetPosition(const GraphicsMath::IVec2& newPosition)
		{
			position = newPosition;
		}
		
		GraphicsMath::Vec4 Base::GetColor() const
		{
			return color;
		}
		
		void Base::SetColor(const GraphicsMath::Vec4& newColor)
		{
			color = newColor;
		}
		
		Collections::String Base::GetText() const
		{
			return text;
		}
		
		void Base::SetText(const Collections::String& newText)
		{
			text = newText;
		}
		
		Collections::String Base::GetName() const
		{
			return name;
		}
		
		void Base::SetName(const Collections::String& newName)
		{
			name = newName;
		}
		#endif
	}
}