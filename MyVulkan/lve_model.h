#pragma once

#include"lve_device.h"
#include "lve_buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace lve {
	class LveModel {

	public:
		struct Vertex {
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& rhs) const {
				return position == rhs.position &&
					color == rhs.color &&
					normal == rhs.normal &&
					uv == rhs.uv;
			}
		};

		struct Builder {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filePath);
		};

		
		static std::unique_ptr<LveModel> createModelFromFile(LveDevice& device, const std::string& filePath);

		LveModel(LveDevice &device, const LveModel::Builder builder);
		~LveModel();

		LveModel(const LveModel&) = delete;
		LveModel& operator=(const LveModel&) = delete;

		void bind(VkCommandBuffer commandBUffer);
		void draw(VkCommandBuffer commandBUffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);

		LveDevice& lveDevice;
	
		std::unique_ptr<LveBuffer> vertexBuffer;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;
		std::unique_ptr<LveBuffer> indexBuffer;

		uint32_t indexCount;
	};
}