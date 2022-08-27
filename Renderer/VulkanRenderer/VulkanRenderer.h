#pragma once

#ifdef USE_VULKAN
	#include "Defines.h"

	namespace Bear
	{
		namespace GUI
		{
			class VulkanRenderer
			{
			private:
				static VulkanInfo info;
			private:
				static VkRenderPass renderPass;
				static VkSwapchainKHR swapchain;
				static Collections::DynamicArray<VkImageView> imageViews;
			private:
				static VkPipelineLayout pipelineLayout;
				static VkPipeline pipeline;
			private:
				static VkCommandPool commandPool;
				static Collections::DynamicArray<VkCommandBuffer> commandBuffer;
			private:
				static Collections::DynamicArray<VkFramebuffer> framebuffers;
			private:
				static Collections::DynamicArray<VkSemaphore> semaphores[2];
				static Collections::DynamicArray<VkFence> fences;
			private:
				static struct
				{
					VkBuffer buffer;
					VkDeviceMemory memory;
				}vertexBuffer;
			private:
				static struct
				{
					VkBuffer buffer;
					VkDeviceMemory memory;
				}indexBuffer;
			private:
				static struct
				{
					VkViewport viewport;
					VkRect2D scissor;
				}dynamicData;
			public:
				static void Init(VulkanInfo* info);
				static void Dispose();
			public:
				static void Render();
			public:
				static void Resize(const ULInt& newWidth, const ULInt& newHeight);
			};
		}
	}
#endif