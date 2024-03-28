#pragma once

#include "lve_window.h"
#include "lve_pipeline.h"
#include "lve_device.h"

namespace lve{
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;


		void run();

	private:
		LveWindow lveWindow{ WIDTH, HEIGHT, "fuck" };
		LveDevice lveDevice{ lveWindow };
		LvePipeline lvePipeline{ lveDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv" , LvePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};

};
}
