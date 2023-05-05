#pragma once

#include "Vertex/Vertex.h"

namespace Bear
{
	namespace GUI
	{
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
			static void Init(void* data = nullptr);
			static void Dispose();
		public:
			static void Render(void* data = nullptr);
		public:
			static void AddRenderData(DynamicArray<Vertex>* vertices, DynamicArray<UInt>* indices);
			static void RemoveRenderData(DynamicArray<Vertex>* vertices, DynamicArray<UInt>* indices);
		public:
			static void SetViewportInfo(void* info = nullptr);
		};
	}
}