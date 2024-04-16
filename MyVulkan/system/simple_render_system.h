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
	class SimpleRenderSystem {
	public:

		SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(FrameInfo& frameInfo, std::vector<LveGameObject> &gameObjects);

	private:
		//void loadModel();
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		LveDevice& lveDevice;

		std::unordered_map<std::string, std::unique_ptr<LvePipeline>> lvePipelines;
		VkPipelineLayout pipelineLayout;

	};
}
