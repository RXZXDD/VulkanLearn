#include "lve_pipeline.h"


#include <fstream>
#include <exception>
#include <iostream>

namespace lve {

	LvePipeline::LvePipeline(LveDevice& device, const std::string& vertFilepath, const std::string& fragFilepath, const PiplineConfigInfo& configInfo) :
	lveDevice(device){
		createGraphicsPipline(vertFilepath, fragFilepath, configInfo);
	}




	std::vector<char> LvePipeline::readFile(const std::string& filepath) {

		std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file " + filepath);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	void LvePipeline::createGraphicsPipline(const std::string& vertFilepath, const std::string& fragFilepath, const PiplineConfigInfo& configInfo) {

		auto vertCode = readFile(vertFilepath);
		auto fragCode = readFile(fragFilepath);

		std::cout << "vertex shader code size: " << vertCode.size() << '\n';
		std::cout << "frag shader code size: " << fragCode.size() << '\n';
	}

	void LvePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("create shader module faild");
		}
	}

	PiplineConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
		PiplineConfigInfo configInfo{};

		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		configInfo.viewport.x = 0.0f;
		configInfo.viewport.y = 0.0f;
		configInfo.viewport.width = static_cast<float>(width);
		configInfo.viewport.height = static_cast<float>(height);
		configInfo.viewport.minDepth = 0.0f;
		configInfo.viewport.maxDepth = 1.0f;

		configInfo.scissor.offset = {0, 0};
		configInfo.scissor.extent = {width, height};

		configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.viewportInfo.viewportCount = 1;
		configInfo.viewportInfo.pViewports = &configInfo.viewport;
		configInfo.viewportInfo.scissorCount = 1;
		configInfo.viewportInfo.pScissors = &configInfo.scissor;

		configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.rasterizationInfo.lineWidth = 1.0f;
		configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
		configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
		configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

		return configInfo;
	}

}