#include "Renderer.h"

#ifdef USE_VULKAN
	#include "Renderer/VulkanRenderer/VulkanRenderer.h"

	#define RENDERER VulkanRenderer
#endif

namespace Bear
{
	namespace GUI
	{
		ULInt Renderer::maxBufferSize = sizeof(Vertex) * 1000;
	
		DynamicArray<DynamicArray<Vertex>*> Renderer::vertices;
		DynamicArray<DynamicArray<UInt>*> Renderer::indices;

		void Renderer::Init(void* data)
		{
			RENDERER::Init(data);
		}
		
		void Renderer::Dispose()
		{
			RENDERER::Dispose();
		}
		
		void Renderer::Render(void* data)
		{
			RENDERER::Render(data);
		}

		void Renderer::AddRenderData(DynamicArray<Vertex>* vertices, DynamicArray<UInt>* indices)
		{
			Renderer::vertices.Add(vertices);
			Renderer::indices.Add(indices);
		}

		void Renderer::RemoveRenderData(DynamicArray<Vertex>* vertices, DynamicArray<UInt>* indices)
		{
			Renderer::vertices.Remove(vertices);
			Renderer::indices.Remove(indices);
		}

		void Renderer::SetViewportInfo(void* info)
		{
			RENDERER::SetViewportInfo(info);
		}
	}
}