#pragma once

#include "GUIObjects/GUIBase.h"

namespace Bear
{
	namespace GUI
	{
		class Button : public Base
		{
		private:
			//TODO (Map): Gdy bede mial mape to po prostu dodac ze klucz to jest OnMouseClickCallback a data do tego jest value
			Collections::DynamicArray<OnMouseClickCallback> callbacks;
			Collections::DynamicArray<void*> data;
		public:
			Button();
			Button(const Properties& properties, const Vec4& clickColor = { 0.0f, 0.0f, 0.0f, -1.0f }, const Vec4& hoverColor = { 0.0f, 0.0f, 0.0f, -1.0f });
		public:
			void AddCallback(const OnMouseClickCallback& callback, void* data = nullptr);
			void RemoveCallback(const OnMouseClickCallback& callback);
		private:
			virtual void OnMouseClick(char mouseButton) override final;
		};
	}
}