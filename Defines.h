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
		using Vec4 = Bear::GraphicsMath::Vec4;

		using String = Bear::Collections::String;
		#endif

		#ifdef USE_VULKAN
		struct VulkanInfo
		{
			VkDevice device;
			ULInt deviceQueueFamilyIndex;
			VkQueue queue;
			VkSurfaceKHR surface;

			ULInt framesCount;
		};
		#endif

		enum class Exception : char
		{
			DataNotPass = 0
		};
	}
}