#include "BearGUI.h"

#include "Renderer/Renderer.h"

namespace Bear
{
	namespace GUI
	{
		DynamicArray<Base*> BearGUI::objects;

		void BearGUI::Init(Window* window, void* data)
		{
			if (data)
			{
				Renderer::Init(data);
			}
			else
				throw Exception::DataNotPass;
		}

		void BearGUI::Render()
		{
			Renderer::Render();
		}

		void BearGUI::Update()
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

		void BearGUI::Clean()
		{
			while (objects.Length())
				objects[0]->~Base();

			objects.Clear();
			
			Renderer::Dispose();
		}

		void BearGUI::Resize(const ULInt& newWidth, const ULInt& newHeight)
		{
			Renderer::Resize(newWidth, newHeight);
		}
	}
}