#ifdef USE_VULKAN
	#include "VulkanRenderer.h"

#define CHECK_RESULT(x) if(x != VkResult::VK_SUCCESS) throw String("Error ") + __FILE__ + " " + __LINE__;
#define STRUCTURE_TYPE(x) VkStructureType::VK_STRUCTURE_TYPE_##x

	namespace Bear
	{
		namespace GUI
		{
			VulkanInfo VulkanRenderer::info = {};

			VkRenderPass VulkanRenderer::renderPass;
			VkSwapchainKHR VulkanRenderer::swapchain;
			Collections::DynamicArray<VkImageView> VulkanRenderer::imageViews;
		
			VkPipelineLayout VulkanRenderer::pipelineLayout;
			VkPipeline VulkanRenderer::pipeline;
		
			VkCommandPool VulkanRenderer::commandPool;
			Collections::DynamicArray<VkCommandBuffer> VulkanRenderer::commandBuffer;
		
			Collections::DynamicArray<VkFramebuffer> VulkanRenderer::framebuffers;
		
			Collections::DynamicArray<VkSemaphore> VulkanRenderer::semaphores[2];
			Collections::DynamicArray<VkFence> VulkanRenderer::fences;

			VulkanRenderer::VertexBuffer VulkanRenderer::vertexBuffer = {};
			VulkanRenderer::IndexBuffer VulkanRenderer::indexBuffer = {};
			VulkanRenderer::DynamicData VulkanRenderer::dynamicData = {};
		
			static struct
			{
				VkBuffer buffer;
				VkDeviceMemory memory;
			}indexBuffer;
		
			static struct
			{
				VkViewport viewport;
				VkRect2D scissor;
			}dynamicData;

			void VulkanRenderer::Init(VulkanInfo* info)
			{
				VulkanRenderer::info = *info;

				uint32_t count = 0;

				VkSurfaceFormatKHR surfaceFormat;
				VkSurfaceCapabilitiesKHR surfaceCapabilities;

				//Surface
				{
					CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(info->physicalDevice, info->surface, &count, nullptr));
					DynamicArray<VkSurfaceFormatKHR> formats(count);
					CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(info->physicalDevice, info->surface, &count, formats.Data()));

					surfaceFormat = formats[0];

					for (auto& format : formats)
					{
						if (format.format == VkFormat::VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
						{
							surfaceFormat = format;
							break;
						}
					}

					CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info->physicalDevice, info->surface, &surfaceCapabilities));
				}

				//RenderPass
				{

					VkAttachmentDescription attachmentDescription
					{
						0,															//flags
						surfaceFormat.format,										//format
						VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,				//samples
						VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD,				//loadOp
						VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE,			//storeOp
						VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE,		//stencilLoadOp
						VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE,		//stencilStoreOp
						VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,				//initialLayout
						VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR				//finalLayout
					};

					VkAttachmentReference colorAttachmentReference
					{
						0,														//attachment
						VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL	//layout
					};

					VkSubpassDescription subpassDescription
					{
						0,														//flags
						VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS,	//pipelineBindPoint
						0,														//inputAttachmentCount
						nullptr,												//pInputAttachments
						1,														//colorAttachmentCount
						&colorAttachmentReference,								//pColorAttachments
						nullptr,												//pResolveAttachments
						nullptr,												//pDepthStencilAttachment
						0,														//preserveAttachmentCount
						nullptr													//pPreserveAttachments
					};

					VkSubpassDependency subpassDependency
					{
						VK_SUBPASS_EXTERNAL,														//srcSubpass
						0,																			//dstSubpass
						VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,		//srcStageMask
						VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,		//dstStageMask
						0,																			//srcAccessMask
						VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,						//dstAccessMask
						VkDependencyFlagBits::VK_DEPENDENCY_DEVICE_GROUP_BIT						//dependencyFlags
					};

					VkRenderPassCreateInfo createInfo
					{
						STRUCTURE_TYPE(RENDER_PASS_CREATE_INFO),					//sType
						nullptr,													//pNext
						0,															//flags
						1,															//attachmentCount
						&attachmentDescription,										//pAttachments
						1,															//subpassCount
						&subpassDescription,										//pSubpasses
						1,															//dependencyCount
						&subpassDependency											//pDependencies
					};

					CHECK_RESULT(vkCreateRenderPass(info->device, &createInfo, nullptr, &renderPass));
				}

				//Swapchain
				{
					VkSwapchainCreateInfoKHR createInfo
					{
						STRUCTURE_TYPE(SWAPCHAIN_CREATE_INFO_KHR),								//sType
						nullptr,																//pNext
						0,																		//flags
						info->surface,															//surface
						info->framesCount,														//minImageCount
						surfaceFormat.format,													//imageFormat
						surfaceFormat.colorSpace,												//imageColorSpace
						surfaceCapabilities.currentExtent,										//imageExtent
						1,																		//imageArrayLayers
						VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,				//imageUsage
						VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,								//imageSharingMode
						0,																		//queueFamilyIndexCount
						nullptr,																//pQueueFamilyIndices
						VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,	//preTransform
						VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,			//compositeAlpha
						info->presentMode,														//presentMode
						true,																	//clipped
						nullptr																	//oldSwapchain
					};

					CHECK_RESULT(vkCreateSwapchainKHR(info->device, &createInfo, nullptr, &swapchain));
				}

				//ImageViews
				{
					CHECK_RESULT(vkGetSwapchainImagesKHR(info->device, swapchain, &count, nullptr));
					DynamicArray<VkImage> images(count);
					CHECK_RESULT(vkGetSwapchainImagesKHR(info->device, swapchain, &count, images.Data()));

					imageViews.Resize(count);

					for (uint32_t i = 0; i < count; i++)
					{
						VkImageViewCreateInfo createInfo
						{
							STRUCTURE_TYPE(IMAGE_VIEW_CREATE_INFO),					//sType
							nullptr,												//pNext
							0,														//flags
							images[i],												//image
							VkImageViewType::VK_IMAGE_VIEW_TYPE_2D,					//viewType
							surfaceFormat.format,									//format
							{														//components
								VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY,		//r
								VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY,		//g
								VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY,		//b
								VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY		//a
							},
							{														//subresourceRange
								VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT,		//aspectMask
								0,														//baseMipLevel
								1,														//levelCount
								0,														//baseArrayLayer
								1														//layerCount
							}
						};

						CHECK_RESULT(vkCreateImageView(info->device, &createInfo, nullptr, &imageViews[i]));
					}
				}

				//Pipeline
				{
					//Pipeline Layout
					{
						VkPipelineLayoutCreateInfo createInfo
						{
							STRUCTURE_TYPE(PIPELINE_LAYOUT_CREATE_INFO),	//sType
							nullptr,										//pNext
							0,												//flags
							0,												//setLayoutCount
							nullptr,										//pSetLayouts
							0,												//pushConstantRangeCount
							nullptr											//pPushConstantRanges
						};

						CHECK_RESULT(vkCreatePipelineLayout(info->device, &createInfo, nullptr, &pipelineLayout));
					}

					constexpr char shaderCount = 2;
					VkPipelineShaderStageCreateInfo stages[shaderCount];

					//Shaders
					{
						constexpr VkShaderStageFlagBits shaderStages[shaderCount] = { VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT };

						VkShaderModule shaderModules[shaderCount];

						/*
							#version 450
							#extension GL_ARB_separate_shader_objects : enable

							layout (location = 0) in vec2 iPos;
							layout (location = 1) in vec4 iColor;

							layout (location = 0) out vec4 oColor;

							void main()
							{
								gl_Position = vec4(iPos, 0.0f, 1.0f);
								oColor = iColor;
							}
						*/
						constexpr uint32_t vertCode[] =
						{
							0x07230203, 0x00010000, 0x0008000a, 0x0000001f, 0x00000000, 0x00020011, 0x00000001, 0x0006000b,
							0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001,
							0x0009000f, 0x00000000, 0x00000004, 0x6e69616d, 0x00000000, 0x0000000d, 0x00000012, 0x0000001b,
							0x0000001d, 0x00030003, 0x00000002, 0x000001c2, 0x00090004, 0x415f4c47, 0x735f4252, 0x72617065,
							0x5f657461, 0x64616873, 0x6f5f7265, 0x63656a62, 0x00007374, 0x00040005, 0x00000004, 0x6e69616d,
							0x00000000, 0x00060005, 0x0000000b, 0x505f6c67, 0x65567265, 0x78657472, 0x00000000, 0x00060006,
							0x0000000b, 0x00000000, 0x505f6c67, 0x7469736f, 0x006e6f69, 0x00070006, 0x0000000b, 0x00000001,
							0x505f6c67, 0x746e696f, 0x657a6953, 0x00000000, 0x00070006, 0x0000000b, 0x00000002, 0x435f6c67,
							0x4470696c, 0x61747369, 0x0065636e, 0x00070006, 0x0000000b, 0x00000003, 0x435f6c67, 0x446c6c75,
							0x61747369, 0x0065636e, 0x00030005, 0x0000000d, 0x00000000, 0x00040005, 0x00000012, 0x736f5069,
							0x00000000, 0x00040005, 0x0000001b, 0x6c6f436f, 0x0000726f, 0x00040005, 0x0000001d, 0x6c6f4369,
							0x0000726f, 0x00050048, 0x0000000b, 0x00000000, 0x0000000b, 0x00000000, 0x00050048, 0x0000000b,
							0x00000001, 0x0000000b, 0x00000001, 0x00050048, 0x0000000b, 0x00000002, 0x0000000b, 0x00000003,
							0x00050048, 0x0000000b, 0x00000003, 0x0000000b, 0x00000004, 0x00030047, 0x0000000b, 0x00000002,
							0x00040047, 0x00000012, 0x0000001e, 0x00000000, 0x00040047, 0x0000001b, 0x0000001e, 0x00000000,
							0x00040047, 0x0000001d, 0x0000001e, 0x00000001, 0x00020013, 0x00000002, 0x00030021, 0x00000003,
							0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004,
							0x00040015, 0x00000008, 0x00000020, 0x00000000, 0x0004002b, 0x00000008, 0x00000009, 0x00000001,
							0x0004001c, 0x0000000a, 0x00000006, 0x00000009, 0x0006001e, 0x0000000b, 0x00000007, 0x00000006,
							0x0000000a, 0x0000000a, 0x00040020, 0x0000000c, 0x00000003, 0x0000000b, 0x0004003b, 0x0000000c,
							0x0000000d, 0x00000003, 0x00040015, 0x0000000e, 0x00000020, 0x00000001, 0x0004002b, 0x0000000e,
							0x0000000f, 0x00000000, 0x00040017, 0x00000010, 0x00000006, 0x00000002, 0x00040020, 0x00000011,
							0x00000001, 0x00000010, 0x0004003b, 0x00000011, 0x00000012, 0x00000001, 0x0004002b, 0x00000006,
							0x00000014, 0x00000000, 0x0004002b, 0x00000006, 0x00000015, 0x3f800000, 0x00040020, 0x00000019,
							0x00000003, 0x00000007, 0x0004003b, 0x00000019, 0x0000001b, 0x00000003, 0x00040020, 0x0000001c,
							0x00000001, 0x00000007, 0x0004003b, 0x0000001c, 0x0000001d, 0x00000001, 0x00050036, 0x00000002,
							0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x0004003d, 0x00000010, 0x00000013,
							0x00000012, 0x00050051, 0x00000006, 0x00000016, 0x00000013, 0x00000000, 0x00050051, 0x00000006,
							0x00000017, 0x00000013, 0x00000001, 0x00070050, 0x00000007, 0x00000018, 0x00000016, 0x00000017,
							0x00000014, 0x00000015, 0x00050041, 0x00000019, 0x0000001a, 0x0000000d, 0x0000000f, 0x0003003e,
							0x0000001a, 0x00000018, 0x0004003d, 0x00000007, 0x0000001e, 0x0000001d, 0x0003003e, 0x0000001b,
							0x0000001e, 0x000100fd, 0x00010038
						};

						/*
							#version 450
							#extension GL_ARB_separate_shader_objects : enable

							layout (location = 0) in vec4 iColor;

							layout (location = 0) out vec4 oColor;

							void main()
							{
								oColor = iColor;
							}
						*/

						constexpr uint32_t fragCode[] =
						{
							0x07230203, 0x00010000, 0x0008000a, 0x0000000d, 0x00000000, 0x00020011, 0x00000001, 0x0006000b,
							0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001,
							0x0007000f, 0x00000004, 0x00000004, 0x6e69616d, 0x00000000, 0x00000009, 0x0000000b, 0x00030010,
							0x00000004, 0x00000007, 0x00030003, 0x00000002, 0x000001c2, 0x00090004, 0x415f4c47, 0x735f4252,
							0x72617065, 0x5f657461, 0x64616873, 0x6f5f7265, 0x63656a62, 0x00007374, 0x00040005, 0x00000004,
							0x6e69616d, 0x00000000, 0x00040005, 0x00000009, 0x6c6f436f, 0x0000726f, 0x00040005, 0x0000000b,
							0x6c6f4369, 0x0000726f, 0x00040047, 0x00000009, 0x0000001e, 0x00000000, 0x00040047, 0x0000000b,
							0x0000001e, 0x00000000, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016,
							0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00040020, 0x00000008,
							0x00000003, 0x00000007, 0x0004003b, 0x00000008, 0x00000009, 0x00000003, 0x00040020, 0x0000000a,
							0x00000001, 0x00000007, 0x0004003b, 0x0000000a, 0x0000000b, 0x00000001, 0x00050036, 0x00000002,
							0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x0004003d, 0x00000007, 0x0000000c,
							0x0000000b, 0x0003003e, 0x00000009, 0x0000000c, 0x000100fd, 0x00010038
						};

						VkShaderModuleCreateInfo shaderCreateInfos[shaderCount];
						shaderCreateInfos[0].codeSize = sizeof(vertCode);
						shaderCreateInfos[0].pCode = vertCode;

						shaderCreateInfos[1].codeSize = sizeof(fragCode);
						shaderCreateInfos[1].pCode = fragCode;

						for (ULInt i = 0; i < shaderCount; i++)
						{
							{
								shaderCreateInfos[i].sType = STRUCTURE_TYPE(SHADER_MODULE_CREATE_INFO);
								shaderCreateInfos[i].pNext = nullptr;
								shaderCreateInfos[i].flags = 0;

								CHECK_RESULT(vkCreateShaderModule(info->device, &shaderCreateInfos[i], nullptr, &shaderModules[i]));
							}

							auto& stage = stages[i];

							stage.sType = STRUCTURE_TYPE(PIPELINE_SHADER_STAGE_CREATE_INFO);	//sType
							stage.pNext = nullptr;												//pNext
							stage.flags = 0;													//flags
							stage.stage = shaderStages[i];										//stage
							stage.module = shaderModules[i];									//module
							stage.pName = "main";												//pName
							stage.pSpecializationInfo = nullptr;								//pSpecializationInfo
						}
					}

					VkVertexInputBindingDescription vertexBindingDescription
					{

					};
					
					DynamicArray<VkVertexInputAttributeDescription> vertexAttributeDescriptions
					{
						{

						},
						{

						}
					};

					VkPipelineVertexInputStateCreateInfo vertexInputState
					{
						STRUCTURE_TYPE(PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO),	//sType
						nullptr,													//pNext
						0,															//flags
						1,															//vertexBindingDescriptionCount
						&vertexBindingDescription,									//pVertexBindingDescriptions
						vertexAttributeDescriptions.Length(),						//vertexAttributeDescriptionCount
						vertexAttributeDescriptions.Data()							//pVertexAttributeDescriptions
					};

					VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
					VkPipelineTessellationStateCreateInfo tessellationState;
					VkPipelineRasterizationStateCreateInfo rasterizationState;
					VkPipelineMultisampleStateCreateInfo multisampleState;
					VkPipelineColorBlendStateCreateInfo colorBlendState;
					VkPipelineDynamicStateCreateInfo dynamicState;

					VkGraphicsPipelineCreateInfo createInfo
					{
						STRUCTURE_TYPE(GRAPHICS_PIPELINE_CREATE_INFO),	//sType
						nullptr,										//pNext
						0,												//flags
						shaderCount,									//stageCount
						stages,											//pStages
						&vertexInputState,								//pVertexInputState
						&inputAssemblyState,							//pInputAssemblyState
						&tessellationState,								//pTessellationState
						nullptr,										//pViewportState
						&rasterizationState,							//pRasterizationState
						&multisampleState,								//pMultisampleState
						nullptr,										//pDepthStencilState
						&colorBlendState,								//pColorBlendState
						&dynamicState,									//pDynamicState
						pipelineLayout,									//layout
						renderPass,										//renderPass
						0,												//subpass
						nullptr,										//basePipelineHandle
						-1												//basePipelineIndex
					};

					CHECK_RESULT(vkCreateGraphicsPipelines(info->device, nullptr, 1, &createInfo, nullptr, &pipeline));
				}
			}

			void VulkanRenderer::Dispose()
			{
				vkDestroyPipelineLayout(info.device, pipelineLayout, nullptr);
				vkDestroyPipeline(info.device, pipeline, nullptr);
				for (auto& imageView : imageViews)
					vkDestroyImageView(info.device, imageView, nullptr);
				vkDestroyRenderPass(info.device, renderPass, nullptr);
				vkDestroySwapchainKHR(info.device, swapchain, nullptr);
			}

			void VulkanRenderer::Render()
			{
			}

			void VulkanRenderer::Resize(const ULInt& newWidth, const ULInt& newHeight)
			{
			}
		}
	}
#endif