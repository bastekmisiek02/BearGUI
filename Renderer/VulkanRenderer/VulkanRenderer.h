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
				struct ImageCreateInfo
				{
					VkImageType imageType;
					VkFormat format;
					VkExtent2D extent;
					VkImageTiling tiling;
					VkImageUsageFlags usage;
					VkImageAspectFlags aspectFlag;
					VkMemoryPropertyFlags memoryFlag;
				};

				struct Image
				{
					VkImageView imageView;
					VkImage image;
					VkDeviceMemory memory;

					VkImageLayout layout;
					VkAccessFlags accessFlags;
				};
			private:
				static DynamicArray<VkImageView> imageViews;
				static DynamicArray<VkFramebuffer> framebuffers;
			private:
				static VkPipelineLayout pipelineLayout;
				static VkPipeline pipeline;
			private:
				static struct OneTimeSubmitData
				{
					void Begin();
					void End();

					VkCommandPool commandPool;
					VkCommandBuffer commandBuffer;
					VkFence fence;
				}oneTimeSubmitData;
			private:
				struct Buffer
				{
					VkBuffer buffer;
					VkDeviceMemory memory;
				};

				static DynamicArray<Buffer> vertexBuffers;
				static DynamicArray<Buffer> indexBuffers;
			private:
				static DynamicArray<Image> IDImages;
				static Buffer IDsBuffer;
			private:
				static struct DynamicData
				{
					VkViewport viewport;
					VkRect2D scissor;
				}dynamicData;
			private:
				static UInt FindMemoryIndex(const UInt& memoryType, const VkMemoryPropertyFlags& memoryPropertyFlags);
			private:
				static VkMemoryRequirements CreateBuffer(Buffer& buffer, const ULInt& size, const VkBufferUsageFlags& bufferUsage, const VkMemoryPropertyFlags& memoryPropertyFlags);
				static void DestroyBuffer(Buffer& buffer);
			private:
				static VkMemoryRequirements CreateImage(Image& image, const ImageCreateInfo& imageCreateInfo);
				static void DestroyImage(Image& image);

				static void TransitionImageLayout(Image& image, const VkImageLayout& newLayout, const VkAccessFlags& newAccess);
			private:
				static void CreateSizingObjects();
				static void DisposeSizingObjects();

				static void RecreateSizingObjects();
			public:
				static void Init(void* info);
				static void Dispose();
			public:
				static void Render(void* frameInfo);
			public:
				static void SetViewportInfo(void* viewportInfo);
			public:
				static UInt GetIDFromPos(const UInt& x, const UInt& y, void* frameIndex);
			};
		}
	}
#endif