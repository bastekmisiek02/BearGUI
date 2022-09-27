#include "BearGUI.h"

#include "Renderer/Renderer.h"

namespace Bear
{
	namespace GUI
	{
		DynamicArray<Base*> GUI::objects;

		void GUI::Init(Window* window, void* data)
		{
			if (data)
				Renderer::Init(data);
			else
				throw Exception("\"data\" is null");
		}

		void GUI::Render(void* data)
		{
			Renderer::Render(data);
		}

		void GUI::Update()
		{
			//for (auto& obj : objects)
			//{
			//	if (obj->IsPointerOnObject(renderInfo->window->GetMousePosition()))
			//	{
			//		if (!obj->isPointerOnObject)
			//		{
			//			obj->isPointerOnObject = true;
			//	
			//			obj->OnMouseEnter();
			//		}
			//
			//		if (renderInfo->window->IsKeyDown((char)Window::MouseButton::Left))
			//			obj->OnMouseClick(Window::MouseButton::Left);
			//		else if (renderInfo->window->IsKeyDown((char)Window::MouseButton::Right))
			//			obj->OnMouseClick(Window::MouseButton::Right);
			//	}
			//	else
			//	{
			//		if (obj->isPointerOnObject)
			//		{
			//			obj->isPointerOnObject = false;
			//	
			//			obj->OnMouseExit();
			//		}
			//	}
			//}
		}

		void GUI::Clean()
		{
			while (objects.Length())
				objects[0]->~Base();

			objects.Clear();
			
			Renderer::Dispose();
		}

		void GUI::Resize(const UInt& newWidth, const UInt& newHeight)
		{
			Renderer::Resize(newWidth, newHeight);
		}
	}
}