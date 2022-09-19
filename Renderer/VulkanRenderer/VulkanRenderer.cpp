#ifdef USE_VULKAN
	#include "VulkanRenderer.h"

	#include "Renderer/Renderer.h"

#define CHECK_RESULT(x) if(x != VkResult::VK_SUCCESS) throw String("Error ") + __FILE__ + " " + __LINE__;
#define STRUCTURE_TYPE(x) VkStructureType::VK_STRUCTURE_TYPE_##x
#define OFFSETOF(type, member) (ULInt)&((type*)0)->member

	namespace Bear
	{
		namespace GUI
		{
			VulkanInfo VulkanRenderer::info = {};

			VkPhysicalDeviceMemoryProperties VulkanRenderer::memoryProperties;

			VulkanRenderer::SurfaceInfo VulkanRenderer::surfaceInfo = {};

			VkRenderPass VulkanRenderer::renderPass;
			Collections::DynamicArray<VkImageView> VulkanRenderer::imageViews;
		
			VkPipelineLayout VulkanRenderer::pipelineLayout;
			VkPipeline VulkanRenderer::pipeline;
		
			Collections::DynamicArray<VkFramebuffer> VulkanRenderer::framebuffers;

			VulkanRenderer::Buffer VulkanRenderer::vertexBuffer = {};
			VulkanRenderer::Buffer VulkanRenderer::indexBuffer = {};
			VulkanRenderer::DynamicData VulkanRenderer::dynamicData = {};

			static const Collections::DynamicArray<Vertex> vertices
			{
				{
					{0.0f, -1.0f},
					{0.0f, 1.0f, 0.0f, 1.0f}
				},
				{
					{1.0f, 1.0f},
					{0.0f, 1.0f, 0.0f, 1.0f}
				},
				{
					{-1.0f, 1.0f},
					{0.0f, 1.0f, 0.0f, 1.0f}
				}
			};

			static const Collections::DynamicArray<UInt> indices
			{
				0, 1, 2
			};

			UInt VulkanRenderer::FindMemoryIndex(const UInt& memoryType, const VkMemoryPropertyFlags& memoryPropertyFlags)
			{
				for (UInt i = 0; i < memoryProperties.memoryTypeCount; i++)
				{
					if ((memoryType & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags))
						return i;
				}

				return ~0;
			}

			VkMemoryRequirements VulkanRenderer::CreateBuffer(Buffer& buffer, const ULInt& size, const VkBufferUsageFlags& bufferUsage, const VkMemoryPropertyFlags& memoryPropertyFlags)
			{
				//Buffer
				{
					VkBufferCreateInfo createInfo
					{
						STRUCTURE_TYPE(BUFFER_CREATE_INFO),			//sType
						nullptr,									//pNext
						0,											//flags
						size,										//size
						bufferUsage,								//usage
						VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,	//sharingMode
						0,											//queueFamilyIndexCount
						nullptr										//pQueueFamilyIndices
					};

					CHECK_RESULT(vkCreateBuffer(info.device, &createInfo, nullptr, &buffer.buffer));
				}

				VkMemoryRequirements memoryRequirements;
				vkGetBufferMemoryRequirements(info.device, buffer.buffer, &memoryRequirements);

				//DeviceMemory
				{
					VkMemoryAllocateInfo allocateInfo
					{
						STRUCTURE_TYPE(MEMORY_ALLOCATE_INFO),									//sType
						nullptr,																//pNext
						memoryRequirements.size,												//allocationSize
						FindMemoryIndex(memoryRequirements.memoryTypeBits, memoryPropertyFlags)	//memoryTypeIndex
					};

					CHECK_RESULT(vkAllocateMemory(info.device, &allocateInfo, nullptr, &buffer.memory));
				}

				CHECK_RESULT(vkBindBufferMemory(info.device, buffer.buffer, buffer.memory, 0));

				return memoryRequirements;
			}

			void VulkanRenderer::DestroyBuffer(Buffer& buffer)
			{
				vkFreeMemory(info.device, buffer.memory, nullptr);
				vkDestroyBuffer(info.device, buffer.buffer, nullptr);
			}

			void VulkanRenderer::Init(VulkanInfo* info)
			{
				VulkanRenderer::info = *info;

				UInt count = 0;

				//PhysicalDevice
				{
					vkGetPhysicalDeviceMemoryProperties(info->physicalDevice, &memoryProperties);
				}

				//Surface
				{
					CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(info->physicalDevice, info->surface, &count, nullptr));
					DynamicArray<VkSurfaceFormatKHR> formats(count);
					CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(info->physicalDevice, info->surface, &count, formats.Data()));

					surfaceInfo.format = formats[0];

					for (auto& format : formats)
					{
						if (format.format == VkFormat::VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
						{
							surfaceInfo.format = format;
							break;
						}
					}

					CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info->physicalDevice, info->surface, &surfaceInfo.capabilities));

					dynamicData.viewport.width = surfaceInfo.capabilities.currentExtent.width;
					dynamicData.viewport.height = surfaceInfo.capabilities.currentExtent.height;
					dynamicData.viewport.x = 0;
					dynamicData.viewport.y = 0;
					dynamicData.viewport.minDepth = 0.01f;
					dynamicData.viewport.maxDepth = 0.01f;

					dynamicData.scissor.extent = surfaceInfo.capabilities.currentExtent;
					dynamicData.scissor.offset = { 0, 0 };
				}

				//RenderPass
				{

					VkAttachmentDescription attachmentDescription
					{
						0,															//flags
						surfaceInfo.format.format,									//format
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

				CreateSizingObjects();

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
					VkPipelineShaderStageCreateInfo stages[shaderCount]{};
					VkShaderModule shaderModules[shaderCount]{};

					//Shaders
					{
						constexpr VkShaderStageFlagBits shaderStages[shaderCount] = { VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT };

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
						constexpr UInt vertCode[] =
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

						constexpr UInt fragCode[] =
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

						VkShaderModuleCreateInfo shaderCreateInfos[shaderCount]{};
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
						0,													//binding
						sizeof(Vertex),										//stride
						VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX		//inputRate
					};

					DynamicArray<VkVertexInputAttributeDescription> vertexAttributeDescriptions
					{
						{
							0,											//location
							0,											//binding
							VkFormat::VK_FORMAT_R32G32_SFLOAT,			//format
							OFFSETOF(Vertex, position)					//offset
						},
						{
							1,											//location
							0,											//binding
							VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT,	//format
							OFFSETOF(Vertex, color)						//offset
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

					VkPipelineInputAssemblyStateCreateInfo inputAssemblyState
					{
						STRUCTURE_TYPE(PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO),	//sType
						nullptr,													//pNext
						0,															//flags
						VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,	//topology
						false														//primitiveRestartEnable
					};

					VkViewport viewport
					{
						0.0f,													//x
						0.0f,													//y
						(float)surfaceInfo.capabilities.currentExtent.width,	//width
						(float)surfaceInfo.capabilities.currentExtent.height,   //height
						0.01f,													//minDepth
						1.0f,													//maxDepth
					};

					VkRect2D scissor
					{
						{												//offset
							0,	//x
							0	//y
						},
						surfaceInfo.capabilities.currentExtent.width	//extent
					};

					VkPipelineViewportStateCreateInfo viewportState
					{
						STRUCTURE_TYPE(PIPELINE_VIEWPORT_STATE_CREATE_INFO),	//sType
						nullptr,												//pNext
						0,														//flags
						1,														//viewportCount
						&viewport,												//pViewports
						1,														//scissorCount
						&scissor												//pScissors
					};															

					VkPipelineRasterizationStateCreateInfo rasterizationState
					{
						STRUCTURE_TYPE(PIPELINE_RASTERIZATION_STATE_CREATE_INFO),	//sType
						nullptr,													//pNext
						0,															//flags
						false,														//depthClampEnable
						false,														//rasterizerDiscardEnable
						VkPolygonMode::VK_POLYGON_MODE_FILL,						//polygonMode
						VkCullModeFlagBits::VK_CULL_MODE_NONE,						//cullMode
						VkFrontFace::VK_FRONT_FACE_CLOCKWISE,						//frontFace
						false,														//depthBiasEnable
						0.0f,														//depthBiasConstantFactor
						0.0f,														//depthBiasClamp
						0.0f,														//depthBiasSlopeFactor
						1.0f														//lineWidth
					};

					VkPipelineMultisampleStateCreateInfo multisampleState
					{
						STRUCTURE_TYPE(PIPELINE_MULTISAMPLE_STATE_CREATE_INFO),	//sType
						nullptr,												//pNext
						0,														//flags
						VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,			//rasterizationSamples
						false,													//sampleShadingEnable
						0.0f,													//minSampleShading
						nullptr,												//pSampleMask
						false,													//alphaToCoverageEnable
						false													//alphaToOneEnable
					};

					VkPipelineColorBlendAttachmentState attachment
					{
						false,																																																				//blendEnable
						VkBlendFactor::VK_BLEND_FACTOR_ONE,																																													//srcColorBlendFactor
						VkBlendFactor::VK_BLEND_FACTOR_ONE,																																													//dstColorBlendFactor
						VkBlendOp::VK_BLEND_OP_ADD,																																															//colorBlendOp
						VkBlendFactor::VK_BLEND_FACTOR_ONE,																																													//srcAlphaBlendFactor
						VkBlendFactor::VK_BLEND_FACTOR_ONE,																																													//dstAlphaBlendFactor
						VkBlendOp::VK_BLEND_OP_ADD,																																															//alphaBlendOp
						VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_A_BIT	//colorWriteMask
					};

					VkPipelineColorBlendStateCreateInfo colorBlendState
					{
						STRUCTURE_TYPE(PIPELINE_COLOR_BLEND_STATE_CREATE_INFO),	//sType
						nullptr,												//pNext
						0,														//flags
						false,													//logicOpEnable
						VkLogicOp::VK_LOGIC_OP_COPY,							//logicOp
						1,														//attachmentCount
						&attachment,											//pAttachments
						{														//blendConstants[4]
							0.0f,
							0.0f,
							0.0f,
							0.0f
						}
					};

					const VkDynamicState dynamicStates[] = {VkDynamicState::VK_DYNAMIC_STATE_VIEWPORT, VkDynamicState::VK_DYNAMIC_STATE_SCISSOR};

					VkPipelineDynamicStateCreateInfo dynamicState
					{
						STRUCTURE_TYPE(PIPELINE_DYNAMIC_STATE_CREATE_INFO),	//sType
						nullptr,											//pNext
						0,													//flags
						sizeof(dynamicStates) / sizeof(VkDynamicState),		//dynamicStateCount
						dynamicStates										//pDynamicStates
					};

					VkGraphicsPipelineCreateInfo createInfo
					{
						STRUCTURE_TYPE(GRAPHICS_PIPELINE_CREATE_INFO),	//sType
						nullptr,										//pNext
						0,												//flags
						shaderCount,									//stageCount
						stages,											//pStages
						&vertexInputState,								//pVertexInputState
						&inputAssemblyState,							//pInputAssemblyState
						nullptr,										//pTessellationState
						&viewportState,									//pViewportState
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

					for (ULInt i = 0; i < shaderCount; i++)
						vkDestroyShaderModule(info->device, shaderModules[i], nullptr);
				}

				//VertexBuffer
				{
					CreateBuffer(vertexBuffer, 1000, VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

					void* data;
					vkMapMemory(info->device, vertexBuffer.memory, 0, sizeof(Vertex) * vertices.Length(), 0, &data);
					{
						std::memcpy(data, (const void*)vertices.Data(), sizeof(Vertex) * vertices.Length());
					}
					vkUnmapMemory(info->device, vertexBuffer.memory);
				}

				//IndexBuffer
				{
					CreateBuffer(indexBuffer, 1000, VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

					void* data;
					vkMapMemory(info->device, indexBuffer.memory, 0, sizeof(UInt) * indices.Length(), 0, &data);
					{
						std::memcpy(data, (const void*)indices.Data(), sizeof(UInt) * indices.Length());
					}
					vkUnmapMemory(info->device, indexBuffer.memory);
				}
			}

			void VulkanRenderer::Dispose()
			{
				vkDeviceWaitIdle(info.device);

				DestroyBuffer(indexBuffer);
				DestroyBuffer(vertexBuffer);

				DisposeSizingObjects();

				vkDestroyPipelineLayout(info.device, pipelineLayout, nullptr);
				vkDestroyPipeline(info.device, pipeline, nullptr);
				vkDestroyRenderPass(info.device, renderPass, nullptr);
			}

			void VulkanRenderer::CreateSizingObjects()
			{
				UInt count = 0;

				//ImageViews
				{
					CHECK_RESULT(vkGetSwapchainImagesKHR(info.device, info.swapchain, &count, nullptr));
					DynamicArray<VkImage> images(count);
					CHECK_RESULT(vkGetSwapchainImagesKHR(info.device, info.swapchain, &count, images.Data()));

					imageViews.Resize(count);

					VkImageViewCreateInfo createInfo
					{
						STRUCTURE_TYPE(IMAGE_VIEW_CREATE_INFO),					//sType
						nullptr,												//pNext
						0,														//flags
						nullptr,												//image
						VkImageViewType::VK_IMAGE_VIEW_TYPE_2D,					//viewType
						surfaceInfo.format.format,								//format
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

					for (UInt i = 0; i < count; i++)
					{
						createInfo.image = images[i];

						CHECK_RESULT(vkCreateImageView(info.device, &createInfo, nullptr, &imageViews[i]));
					}
				}

				//Framebuffers
				{
					framebuffers.Resize(info.framesInFlightCount);

					VkFramebufferCreateInfo createInfo
					{
						STRUCTURE_TYPE(FRAMEBUFFER_CREATE_INFO),		//sType;
						nullptr,										//pNext;
						0,												//flags;
						renderPass,										//renderPass;
						1,												//attachmentCount
						nullptr,										//pAttachments;
						surfaceInfo.capabilities.currentExtent.width,	//width;
						surfaceInfo.capabilities.currentExtent.height,	//height;
						1												//layers;
					};

					for (ULInt i = 0; i < imageViews.Length(); i++)
					{
						createInfo.pAttachments = &imageViews[i];

						CHECK_RESULT(vkCreateFramebuffer(info.device, &createInfo, nullptr, &framebuffers[i]))
					}
				}
			}

			void VulkanRenderer::DisposeSizingObjects()
			{
				for (UInt i = 0; i < info.framesInFlightCount; i++)
				{
					vkDestroyFramebuffer(info.device, framebuffers[i], nullptr);

					vkDestroyImageView(info.device, imageViews[i], nullptr);
				}
			}

			void VulkanRenderer::RecreateSizingObjects()
			{
				vkDeviceWaitIdle(info.device);

				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info.physicalDevice, info.surface, &surfaceInfo.capabilities);

				dynamicData.viewport.width = surfaceInfo.capabilities.currentExtent.width;
				dynamicData.viewport.height = surfaceInfo.capabilities.currentExtent.height;

				dynamicData.scissor.extent = surfaceInfo.capabilities.currentExtent;

				DisposeSizingObjects();
				CreateSizingObjects();
			}

			void VulkanRenderer::Render(VulkanFrameInfo* frameInfo)
			{
				VkClearValue clearValue{};
				clearValue.color = { 0.0f, 0.0f, 0.0f, 1.0f };

				ULInt offset = 0;

				VkRenderPassBeginInfo renderPassBeginInfo
				{
					STRUCTURE_TYPE(RENDER_PASS_BEGIN_INFO),		//sType
					nullptr,									//pNext
					renderPass,									//renderPass
					framebuffers[frameInfo->frameInFlightIndex],//framebuffer
					{											//renderArea
						{									//offset
							dynamicData.viewport.x,		//x
							dynamicData.viewport.y		//y
						},
						{									//extent
							dynamicData.viewport.width,	//width
							dynamicData.viewport.height	//height
						}
					},
					1,											//clearValueCount
					&clearValue									//pClearValues
				};

				vkCmdBeginRenderPass(frameInfo->commandBuffer, &renderPassBeginInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
				{
					vkCmdBindPipeline(frameInfo->commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
					vkCmdSetViewport(frameInfo->commandBuffer, 0, 1, &dynamicData.viewport);
					vkCmdSetScissor(frameInfo->commandBuffer, 0, 1, &dynamicData.scissor);

					vkCmdBindIndexBuffer(frameInfo->commandBuffer, indexBuffer.buffer, 0, VkIndexType::VK_INDEX_TYPE_UINT32);
					vkCmdBindVertexBuffers(frameInfo->commandBuffer, 0, 1, &vertexBuffer.buffer, &offset);
					vkCmdDrawIndexed(frameInfo->commandBuffer, indices.Length(), 1, 0, 0, 0);
				}
				vkCmdEndRenderPass(frameInfo->commandBuffer);
			}

			void VulkanRenderer::Resize()
			{
				RecreateSizingObjects();
			}
		}
	}
#endif