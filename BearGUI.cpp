#include "BearGUI.h"

namespace Bear
{
	namespace GUI
	{
		//Collections::DynamicArray<ObjectGUI*> GUI::objects;
		//
		//struct VertexData
		//{
		//	GraphicsMath::UIVec2 position;
		//	GraphicsMath::UIVec4 color;
		//};
		//
		//static Collections::DynamicArray<VertexData> vertexBuffer;
		//static Collections::DynamicArray<VertexData> indexBuffer;
		//
		//struct VulkanRender
		//{
		//	VkSwapchainKHR swapchain;
		//	Collections::DynamicArray<VkImageView> imageViews;
		//	Collections::DynamicArray<VkFramebuffer> framebuffers;
		//
		//	VkRenderPass renderPass;
		//	VkPipeline graphicsPipeline;
		//
		//	VkCommandPool commandPool;
		//	VkCommandBuffer commandBuffer;
		//
		//	struct
		//	{
		//		VkBuffer vertexBuffer;
		//		VkDeviceMemory vertexDeviceMemory;
		//
		//		VkBuffer indexBuffer;
		//		VkDeviceMemory indexDeviceMemory;
		//	}drawBuffer;
		//
		//	VkFence fences[2];
		//	VkSemaphore semaphores[2][2];
		//
		//	VulkanRender()
		//	{
		//
		//	}
		//};
		//
		//static struct RenderInfo
		//{
		//	GUI::API api;
		//	Window* window;
		//	
		//	void* externalData;
		//
		//	void* renderComponents;
		//
		//	~RenderInfo()
		//	{
		//		switch (api)
		//		{
		//			case GUI::API::Vulkan:
		//			{
		//				delete ((VulkanInfo*)externalData);
		//				delete ((VulkanRender*)renderComponents);
		//				break;
		//			}
		//			default:
		//			{
		//				delete externalData;
		//				delete renderComponents;
		//				break;
		//			}
		//		}
		//	}
		//
		//}*renderInfo = nullptr;

		DynamicArray<Base*> BearGUI::objects;

		void BearGUI::Init(Window* window, void* data)
		{
			//renderInfo = new RenderInfo();
			//
			//renderInfo->api = api;
			//renderInfo->window = window;
			//
			//switch (api)
			//{
			//	case API::Vulkan:
			//	{
			//		renderInfo->externalData = new VulkanInfo(*(VulkanInfo*)data);
			//		renderInfo->renderComponents = new VulkanRender();
			//		break;
			//	}
			//	default:
			//		break;
			//}
		}

		void BearGUI::Render()
		{
			
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
			//objects.Destroy();

			while (objects.Length())
				objects[0]->~Base();

			objects.Clear();
			
			//delete renderInfo;
		}
	}
}