#pragma once

#include "Defines.h"

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

		class Base
		{
		private:
			friend class BearGUI;
		private:
			bool isDestroyed;
		protected:
			bool isVisible;
		protected:
			Base* parent;
			DynamicArray<Base*> childrens;
		protected:
			IVec2 position;
			Vec4 color;
		protected:
			Vec4* currentColor;
		protected:
			String text;
			String name;
		protected:
			char anchor;
			char textAlign;
		public:
			Base();
			Base(Base* parent, const IVec2& position, const Vec4& color, const String& text, const char& anchor = Anchor::Center | Anchor::Mid, const char& textAlign = Anchor::Center | Anchor::Mid, const String& name = "");
			virtual ~Base();
		protected:
			//bool isPointerOnObject;
		protected:
			virtual bool IsPointerOnObject(IVec2 mousePosition) = 0;
		protected:
			virtual void OnMouseEnter();
			virtual void OnMouseExit();

			virtual void OnMouseClick(char mouseButton);
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
			Vec4 GetColor() const;
			void SetColor(const Vec4& newColor);
		public:
			String GetText() const;
			void SetText(const String& newText);
		public:
			String GetName() const;
			void SetName(const String& newName);
		};
	}
}