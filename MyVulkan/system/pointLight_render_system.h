#pragma once

#include "../lve_pipeline.h"
#include "../lve_device.h"
#include "../lve_swap_chain.h"
#include "../lve_model.h"
#include "../lve_game_object.h"
#include "../lve_camera.h"
#include "../lve_frame_info.h"

#include <memory>
#include <vector>
#include<unordered_map>

namespace lve {
	class PointLightSystem {
	public:

		PointLightSystem(LveDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;

		void render(FrameInfo& frameInfo);
		
	private:
		//void loadModel();
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		LveDevice& lveDevice;

		std::unordered_map<std::string, std::unique_ptr<LvePipeline>> lvePipelines;
		VkPipelineLayout pipelineLayout;

	};
}
