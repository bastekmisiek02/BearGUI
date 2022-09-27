#pragma once

#ifdef BEAR_LIBRARY_INCLUDED
	#include <BearLibrary.h>
#else
	#error BearGUI doesn't support others libraries yet
#endif

#ifdef USE_VULKAN
	#include <vulkan/vulkan.h>
#endif

namespace Bear
{
	namespace GUI
	{		
		#ifdef BEAR_LIBRARY_INCLUDED
		template<typename T>
		using DynamicArray = Bear::Collections::DynamicArray<T>;

		using IVec2 = Bear::GraphicsMath::IVec2;
		using Vec2 = Bear::GraphicsMath::Vec2;
		using Vec4 = Bear::GraphicsMath::Vec4;

		using String = Bear::Collections::String;
		#endif

		#ifdef USE_VULKAN
		struct VulkanInfo
		{
			VkPhysicalDevice physicalDevice;
			VkDevice device;
			VkQueue queue;
			VkSurfaceKHR surface;
			VkSwapchainKHR* swapchain;

			UInt framesInFlightCount;
			VkPresentModeKHR presentMode;
		};

		struct VulkanFrameInfo
		{
			VkCommandBuffer commandBuffer;
			UInt frameInFlightIndex;
		};

		struct VulkanViewportInfo
		{
			VkViewport viewport;
			/*Unused - for future use*/
			UInt viewportIndex;

			VkRect2D scissor;
			/*Unused - for future use*/
			UInt scissorIndex;
		};
		#endif

		class Exception
		{
		private:
			const char* message;
		public:
			Exception(const char* message = nullptr)
				: message(message) 
			{
			}

			const char* GetMessage() const 
			{
				return message;
			}
		};
	}
}