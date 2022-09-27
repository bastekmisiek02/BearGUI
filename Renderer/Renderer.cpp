#include "Renderer.h"

#ifdef USE_VULKAN
	#include "Renderer/VulkanRenderer/VulkanRenderer.h"
#endif

namespace Bear
{
	namespace GUI
	{
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
		
		void Renderer::Resize(const UInt& newWidth, const UInt& newHeight)
		{
			VulkanRenderer::Resize();
		}
		#endif
	}
}