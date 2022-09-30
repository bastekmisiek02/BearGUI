#pragma once

#include "Defines.h"

namespace Bear
{
	namespace GUI
	{
		struct Vertex
		{
			Vec2 position;
			Vec4 color;
		};

		class Renderer
		{
		private:
			static ULInt maxBufferSize;
		private:
			static DynamicArray<DynamicArray<Vertex>*> vertices;
			static DynamicArray<DynamicArray<UInt>*> indices;
		#ifdef USE_VULKAN
		private:
			friend class VulkanRenderer;
		#endif
		public:
			static void Init(void* data);
			static void Dispose();
		public:
			static void Render(void* data);
		public:
			static void AddRenderData(DynamicArray<Vertex>* vertices, DynamicArray<UInt>* indices);
			static void CleanRenderData();
		public:
			static void SetViewportInfo(void* info);
		};
	}
}