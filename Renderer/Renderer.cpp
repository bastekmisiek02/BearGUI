#include "Renderer.h"

#ifdef USE_VULKAN
	#include "Renderer/VulkanRenderer/VulkanRenderer.h"
#endif

namespace Bear
{
	namespace GUI
	{
		ULInt Renderer::maxBufferSize = sizeof(Vertex) * 1000;
	
		DynamicArray<DynamicArray<Vertex>*> Renderer::vertices;
		DynamicArray<DynamicArray<UInt>*> Renderer::indices;

		#ifdef USE_VULKAN
		void Renderer::Init(void* data)
		{
			VulkanRenderer::Init((VulkanInfo*)data);
		}
		
		void Renderer::Dispose()
		{
			VulkanRenderer::Dispose();
		}
		
		void Renderer::Render(void* data)
		{
			VulkanRenderer::Render((VulkanFrameInfo*)data);
		}

		void Renderer::AddRenderData(DynamicArray<Vertex>* vertices, DynamicArray<UInt>* indices)
		{
			Renderer::vertices.Add(vertices);
			Renderer::indices.Add(indices);
		}

		void Renderer::CleanRenderData()
		{
			vertices.Clear();
			indices.Clear();
		}

		void Renderer::SetViewportInfo(void* info)
		{
			VulkanRenderer::SetViewportInfo((VulkanViewportInfo*)info);
		}
		#endif
	}
}