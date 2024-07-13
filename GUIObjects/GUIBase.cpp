#include "GUIBase.h"

#include "Renderer/Renderer.h"

#include "BearGUI.h"

namespace Bear
{
	namespace GUI
	{
		#ifdef BEAR_LIBRARY_INCLUDED
		Base::Base()
			: parent(nullptr), position(), defaultColor({ 0.0f, 0.0f, 0.0f, 1.0f }), clickColor(defaultColor), hoverColor(defaultColor), text(), name(), isVisible(false), anchor(Anchor::Center | Anchor::Mid), textAlign(Anchor::Center | Anchor::Mid), currentColor(&defaultColor), isDestroyed(false), vertices(), indices()
		{
			id = Vertex::NextID();

			GUI::objects.Add(this);
		}

		Base::Base(Base* parent, const IVec2& position, const IVec2& size, const Vec4& defaultColor, const String& text, const Vec4& clickColor, const Vec4& hoverColor, const char& anchor, const char& textAlign, const Collections::String& name)
			: parent(parent), position(position), size(size), defaultColor(defaultColor), clickColor(clickColor), hoverColor(hoverColor), text(text), name(name), isVisible(true), anchor(anchor), textAlign(textAlign), currentColor(&this->defaultColor), isDestroyed(false), vertices(), indices()
		{
			id = Vertex::NextID();

			if (clickColor.a == -1)
				this->clickColor = defaultColor;

			if (hoverColor.a == -1)
				this->hoverColor = defaultColor;

			if (parent)
				parent->childrens.Add(this);

			GUI::objects.Add(this);

			AddRenderData();
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

			RemoveRenderData();
		}

		void Base::AddRenderData()
		{
			Renderer::AddRenderData(&vertices, &indices);
		}

		void Base::RemoveRenderData()
		{
			Renderer::RemoveRenderData(&vertices, &indices);
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

		void Base::OnPositionChange(const IVec2& newPosition)
		{
			//TODO: Dokoñczyæ z uwzglêdnieniem anchora

			vertices = Collections::MakeDynamicArray<Vertex>
			(
				Vertex
				{
					{-0.5f, 0.5f},
					*currentColor
				},

				Vertex
				{
					{0.5f, 0.5f},
					*currentColor
				},

				Vertex
				{
					{0.5f, -0.5f},
					*currentColor
				},

				Vertex
				{
					{-0.5f, -0.5f},
					*currentColor
				}
			);

			indices = Collections::MakeDynamicArray<UInt>
				(
					0U, 1U, 2U,
					2U, 3U, 0U
				);
		}

		void Base::OnSizeChange(const IVec2& newSize)
		{
		}

		void Base::OnColorChange(const Vec4& newColor)
		{
			for (auto& vertex : vertices)
				vertex.color = newColor;
		}

		void Base::OnTextChange(const String& newText)
		{
		}

		void Base::Init()
		{
			OnPositionChange(position);
			OnSizeChange(size);
			OnColorChange(defaultColor);
			OnTextChange(text);
		}

		void Base::SetColor()
		{
			this->OnColorChange(*currentColor);
		}

		void Base::SetColor(const Vec4& newColor)
		{
			this->OnColorChange(newColor);
		}
		
		const bool Base::IsVisible() const
		{
			return isVisible;
		}
		
		void Base::Hide()
		{
			isVisible = false;

			RemoveRenderData();
		}
		
		void Base::Show()
		{
			isVisible = true;

			AddRenderData();
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
			this->OnPositionChange(newPosition);

			position = newPosition;
		}

		IVec2 Base::GetSize() const
		{
			return size;
		}

		void Base::SetSize(const IVec2& newSize)
		{
			this->OnSizeChange(newSize);

			size = newSize;
		}
		
		GraphicsMath::Vec4 Base::GetDefaultColor() const
		{
			return defaultColor;
		}
		
		void Base::SetDefaultColor(const GraphicsMath::Vec4& newColor)
		{
			defaultColor = newColor;
		}

		Vec4 Base::GetClickColor() const
		{
			return clickColor;
		}

		void Base::SetClickColor(const Vec4& newColor)
		{
			clickColor = newColor;
		}

		Vec4 Base::GetHoverColor() const
		{
			return hoverColor;
		}

		void Base::SetHoverColor(const Vec4& newColor)
		{
			hoverColor = newColor;
		}
		
		Collections::String Base::GetText() const
		{
			return text;
		}
		
		void Base::SetText(const Collections::String& newText)
		{
			this->OnTextChange(newText);

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