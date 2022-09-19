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
				static struct SurfaceInfo
				{
					VkSurfaceFormatKHR format;
					VkSurfaceCapabilitiesKHR capabilities;
				}surfaceInfo;
			private:
				static VkRenderPass renderPass;
			private:
				static VkSwapchainKHR swapchain;
				static Collections::DynamicArray<VkImageView> imageViews;
				static Collections::DynamicArray<VkFramebuffer> framebuffers;
			private:
				static VkPipelineLayout pipelineLayout;
				static VkPipeline pipeline;
			private:
				static VkCommandPool commandPool;
				static Collections::DynamicArray<VkCommandBuffer> commandBuffers;
			private:
				static Collections::DynamicArray<VkSemaphore> imageAvailableSemaphores;
				static Collections::DynamicArray<VkSemaphore> presentAvailableSemaphores;
				static Collections::DynamicArray<VkFence> fences;
			private:
				struct Buffer
				{
					VkBuffer buffer;
					VkDeviceMemory memory;
				};

				static Buffer vertexBuffer;
				static Buffer indexBuffer;
			private:
				static uint32_t currentFrame;
			private:
				static struct DynamicData
				{
					VkViewport viewport;
					VkRect2D scissor;
				}dynamicData;
			private:
				static uint32_t FindMemoryIndex(const uint32_t& memoryType, const VkMemoryPropertyFlags& memoryPropertyFlags);
				static VkMemoryRequirements CreateBuffer(Buffer& buffer, const VkDeviceSize& size, const VkBufferUsageFlags& bufferUsage, const VkMemoryPropertyFlags& memoryPropertyFlags);
				static void DestroyBuffer(Buffer& buffer);
			private:
				static void ResetCommandBuffer(const VkCommandBuffer& commandBuffer, const uint32_t& index);
			private:
				static void CreateSizingObjects();
				static void DisposeSizingObjects();

				static void RecreateSizingObjects();
			public:
				static void Init(VulkanInfo* info);
				static void Dispose();
			public:
				static void Render();
			public:
				static void Resize();
			};
		}
	}
#endif