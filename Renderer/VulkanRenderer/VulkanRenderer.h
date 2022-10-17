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
				}surfaceInfo;
			private:
				static VkRenderPass renderPass;
			private:
				static DynamicArray<VkImageView> imageViews;
				static DynamicArray<VkFramebuffer> framebuffers;
			private:
				static VkPipelineLayout pipelineLayout;
				static VkPipeline pipeline;
			private:
				struct Buffer
				{
					VkBuffer buffer;
					VkDeviceMemory memory;
				};

				static DynamicArray<Buffer> vertexBuffers;
				static DynamicArray<Buffer> indexBuffers;
			private:
				static struct DynamicData
				{
					VkViewport viewport;
					VkRect2D scissor;
				}dynamicData;
			private:
				static UInt FindMemoryIndex(const UInt& memoryType, const VkMemoryPropertyFlags& memoryPropertyFlags);
				static VkMemoryRequirements CreateBuffer(Buffer& buffer, const ULInt& size, const VkBufferUsageFlags& bufferUsage, const VkMemoryPropertyFlags& memoryPropertyFlags);
				static void DestroyBuffer(Buffer& buffer);
			private:
				static void CreateSizingObjects();
				static void DisposeSizingObjects();

				static void RecreateSizingObjects();
			public:
				static void Init(VulkanInfo* info);
				static void Dispose();
			public:
				static void Render(VulkanFrameInfo* frameInfo);
			public:
				static void SetViewportInfo(VulkanViewportInfo* viewportInfo);
			};
		}
	}
#endif