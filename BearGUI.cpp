#include "BearGUI.h"

#include "Renderer/Renderer.h"

namespace Bear
{
	namespace GUI
	{
		DynamicArray<Base*> GUI::objects;
		Base* GUI::lastActive = nullptr;
		Window* GUI::window = nullptr;

		static char GetMouseClickedButtons(Window* window)
		{
			char mouseButtonClicked = 0;

			if (window->IsKeyDown((char)Window::MouseButton::Left))
				mouseButtonClicked |= (char)Window::MouseButton::Left;

			if (window->IsKeyDown((char)Window::MouseButton::Right))
				mouseButtonClicked |= (char)Window::MouseButton::Right;

			if (window->IsKeyDown((char)Window::MouseButton::Middle))
				mouseButtonClicked |= (char)Window::MouseButton::Middle;

			if (window->IsKeyDown((char)Window::MouseButton::XButton1))
				mouseButtonClicked |= (char)Window::MouseButton::XButton1;

			if (window->IsKeyDown((char)Window::MouseButton::XButton2))
				mouseButtonClicked |= (char)Window::MouseButton::XButton2;

			return mouseButtonClicked;
		}

		void GUI::Init(Window* window, void* data)
		{
			GUI::window = window;

			if (data)
				Renderer::Init(data);
			else
				throw Exception("\"data\" is null");
		}

		void GUI::Render(void* data)
		{
			Renderer::Render(data);
		}

		void GUI::ResetLastActive()
		{
			if (lastActive)
			{
				lastActive->currentColor = &lastActive->defaultColor;
				lastActive->SetColor();

				lastActive->OnMouseExit();
			}
		}

		void GUI::Update(void* data)
		{
			const auto& pos = window->GetMousePosition();

			const char mouseButtonClicked = GetMouseClickedButtons(window);

			void* additionalData = nullptr;

			#ifdef USE_VULKAN
				additionalData = data;
			#endif

			UInt id = 0;

			if (pos.x != -1 && pos.y != -1)
				id = Renderer::GetObjectIDFromPos(pos.x, pos.y, additionalData);

			if (id == 0)
			{
				ResetLastActive();

				lastActive = nullptr;

				return;
			}
			
			for (auto& obj : objects)
			{
				//TODO (Map): Gdy bede mial mape to po prostu dodac ze klucz to jest id a obj do tego jest value i wtedy nie trzeba po liœcie siê poruszaæ
				if (obj->id == id)
				{
					if (lastActive != obj)
					{
						ResetLastActive();

						obj->currentColor = &obj->hoverColor;
						obj->SetColor();

						lastActive = obj;

						obj->OnMouseEnter();
					}

					obj->currentColor = &obj->hoverColor;
					obj->SetColor();

					if (mouseButtonClicked)
					{
						obj->currentColor = &obj->clickColor;
						obj->SetColor();

						obj->OnMouseClick(mouseButtonClicked);
					}

					return;
				}

				//if (obj->IsPointerOnObject(renderInfo->window->GetMousePosition()))
				//{
				//	if (!obj->isPointerOnObject)
				//	{
				//		obj->isPointerOnObject = true;
				//
				//		obj->OnMouseEnter();
				//	}
				//
				//	if (renderInfo->window->IsKeyDown((char)Window::MouseButton::Left))
				//		obj->OnMouseClick(Window::MouseButton::Left);
				//	else if (renderInfo->window->IsKeyDown((char)Window::MouseButton::Right))
				//		obj->OnMouseClick(Window::MouseButton::Right);
				//}
				//else
				//{
				//	if (obj->isPointerOnObject)
				//	{
				//		obj->isPointerOnObject = false;
				//
				//		obj->OnMouseExit();
				//	}
				//}
			}
		}

		void GUI::Clean()
		{
			while (objects.Length())
				objects[0]->~Base();

			objects.Clear();
			
			Renderer::Dispose();
		}

		void GUI::Resize(void* info)
		{
			Renderer::SetViewportInfo(info);
		}
	}
}