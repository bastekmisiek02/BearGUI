#pragma once

#include "Renderer/Vertex/Vertex.h"

namespace Bear
{
	namespace GUI
	{
		enum Anchor : char
		{
			Left = 1,
			Center = 2,
			Right = 4,

			Top = 8,
			Mid = 16,
			Bottom = 32
		};

		struct Properties
		{
			class Base* parent;
			IVec2 position;
			IVec2 size;
			Vec4 defaultColor;
			String text;
			char anchor = Anchor::Center | Anchor::Mid;
			char textAlign = Anchor::Center | Anchor::Mid;
			String name = "";
		};

		typedef void(*OnMouseClickCallback)(Base* element, void* data);

		class Base
		{
		private:
			friend class GUI;
		private:
			UInt id;
		private:
			bool isDestroyed;
		protected:
			bool isVisible;
		protected:
			Base* parent;
			DynamicArray<Base*> childrens;
		protected:
			IVec2 position;
			IVec2 size;
		protected:
			Vec4 defaultColor;
			Vec4 clickColor;
			Vec4 hoverColor;
		protected:
			Vec4* currentColor;
		protected:
			String text;
			String name;
		protected:
			char anchor;
			char textAlign;
		protected:
			DynamicArray<Vertex> vertices;
			DynamicArray<UInt> indices;
		public:
			Base();
			Base(Base* parent, const IVec2& position, const IVec2& size, const Vec4& defaultColor, const String& text, const Vec4& clickColor = {0.0f, 0.0f, 0.0f, -1.0f}, const Vec4& hoverColor = { 0.0f, 0.0f, 0.0f, -1.0f }, const char& anchor = Anchor::Center | Anchor::Mid, const char& textAlign = Anchor::Center | Anchor::Mid, const String& name = "");
			virtual ~Base();
		protected:
			void AddRenderData();
			void RemoveRenderData();
		protected:
			virtual void OnMouseEnter();
			virtual void OnMouseExit();

			virtual void OnMouseClick(char mouseButton);

			virtual void OnPositionChange(const IVec2& newPosition);
			virtual void OnSizeChange(const IVec2& newSize);
			virtual void OnColorChange(const Vec4& newColor);
			virtual void OnTextChange(const String& newText);
		protected:
			void Init();
		protected:
			void SetColor();
			void SetColor(const Vec4& newColor);
		public:
			const bool IsVisible() const;

			void Hide();
			void Show();
		public:
			Base* GetParent() const;
		public:
			void AddChildren(Base* object);
			void RemoveChildren(Base* object);
		public:
			const DynamicArray<Base*>& GetChildrens() const;
		public:
			IVec2 GetPosition() const;
			void SetPosition(const IVec2& newPosition);
		public:
			IVec2 GetSize() const;
			void SetSize(const IVec2& newSize);
		public:
			Vec4 GetDefaultColor() const;
			void SetDefaultColor(const Vec4& newColor);

			Vec4 GetClickColor() const;
			void SetClickColor(const Vec4& newColor);

			Vec4 GetHoverColor() const;
			void SetHoverColor(const Vec4& newColor);
		public:
			String GetText() const;
			void SetText(const String& newText);
		public:
			String GetName() const;
			void SetName(const String& newName);
		};
	}
}