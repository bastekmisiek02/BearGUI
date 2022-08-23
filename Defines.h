#pragma once

#ifdef BEAR_LIBRARY_INCLUDED
	#include <BearLibrary.h>

	template<typename T>
	using DynamicArray = Bear::Collections::DynamicArray<T>;

	using IVec2 = Bear::GraphicsMath::IVec2;
	using Vec4 = Bear::GraphicsMath::Vec4;
	
	using String = Bear::Collections::String;
#else
	#error BearGUI doesn't support others libraries yet
#endif

#ifdef USE_VULKAN
	#include <vulkan/vulkan.h>
	
	struct VulkanInfo
	{
		VkDevice device;
		VkSurfaceKHR surface;
	};
#endif
