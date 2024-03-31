#pragma once

#include <string>
#include <vector>

#include "lve_device.h"

namespace lve {

	struct PiplineConfigInfo {
		//PiplineConfigInfo() {};
		PiplineConfigInfo() = default;
		PiplineConfigInfo(const PiplineConfigInfo&) = delete;
		PiplineConfigInfo& operator=(const PiplineConfigInfo&) = delete;

		//VkViewport viewport;
		//VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class LvePipeline {
	public:
		LvePipeline(LveDevice& device, const std::string& vertFilepath, const std::string& fragFilepath, const PiplineConfigInfo& configInfo);

		~LvePipeline();

		LvePipeline(const LvePipeline&) = delete;
		LvePipeline& operator=(const LvePipeline&) = delete;

		static void defaultPipelineConfigInfo(PiplineConfigInfo& configInfo);

		void bind(VkCommandBuffer commandBuffer);
	private:
		static std::vector<char> readFile(const std::string& filepath);

		void createGraphicsPipline(const std::string& vertFilepath, const std::string& fragFilepath, const PiplineConfigInfo& configInfo);


		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		LveDevice& lveDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

	};
}