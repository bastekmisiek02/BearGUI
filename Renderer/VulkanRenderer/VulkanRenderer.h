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
				static VkPhysicalDeviceMemoryProperties memoryProperties;
			private:
				static VkRenderPass renderPass;
				static VkSwapchainKHR swapchain;
				static Collections::DynamicArray<VkImageView> imageViews;
			private:
				static VkPipelineLayout pipelineLayout;
				static VkPipeline pipeline;
			private:
				static VkCommandPool commandPool;
				static Collections::DynamicArray<VkCommandBuffer> commandBuffers;
			private:
				static Collections::DynamicArray<VkFramebuffer> framebuffers;
			private:
				static Collections::DynamicArray<VkSemaphore> imageAvailableSemaphores;
				static Collections::DynamicArray<VkSemaphore> submitSemaphores;
				static Collections::DynamicArray<VkFence> fences;
			private:
				static struct VertexBuffer
				{
					VkBuffer buffer;
					VkDeviceMemory memory;
				}vertexBuffer;
			private:
				static struct IndexBuffer
				{
					VkBuffer buffer;
					VkDeviceMemory memory;
				}indexBuffer;
			private:
				static struct DynamicData
				{
					VkViewport viewport;
					VkRect2D scissor;
				}dynamicData;
			private:
				static uint32_t FindMemoryIndex(const uint32_t& memoryType, const VkMemoryPropertyFlags& memoryPropertyFlags);
				static VkMemoryRequirements CreateBuffer(VkBuffer& buffer, VkDeviceMemory& memory, const VkDeviceSize& size, const VkBufferUsageFlags& bufferUsage, const VkMemoryPropertyFlags& memoryPropertyFlags);
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