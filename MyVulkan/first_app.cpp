#include "first_app.h"
#include "system/Simple_render_system.h"
#include "system/pointLight_render_system.h"
#include "keyboard_movement_controller.h"
#include "lve_buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include<glm/gtc/constants.hpp>

#include <stdexcept>
#include <chrono>
#include <memory>
#include <array>
#include <numeric>

#define MAX_FRAME_TIME 0.16666f

namespace lve {

    struct GlobalUbo {
        glm::mat4 prjection{ 1.f };
        glm::mat4 view{ 1.f };
        glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
        glm::vec3 lightPosition{ -1.f };
        alignas(16) glm::vec4 lightColor{ 1.f };
    };

	FirstApp::FirstApp()
	{
		//loadModel();
        globalPool = LveDescriptorPool::Builder(lveDevice)
            .setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LveSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();

		loadGameObject();

	}
	FirstApp::~FirstApp()
	{
		
	}
	void FirstApp::run()
	{
        auto minOffetAlignment = std::lcm(
            lveDevice.properties.limits.minUniformBufferOffsetAlignment,
            lveDevice.properties.limits.nonCoherentAtomSize
        );
        //LveBuffer globalUboBuffer{
        //    lveDevice,
        //    sizeof(GlobalUbo),
        //    LveSwapChain::MAX_FRAMES_IN_FLIGHT,
        //    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        //    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        //    minOffetAlignment
        //};
        //globalUboBuffer.map();

        std::vector<std::unique_ptr<LveBuffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<LveBuffer>(
                lveDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                minOffetAlignment
            );
            uboBuffers[i]->map();
        }


        auto globalSetLayout = LveDescriptorSetLayout::Builder(lveDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            LveDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

		SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
		PointLightSystem pointLightSystem{ lveDevice, lveRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};

        LveCamera camera{};

      //  camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));

        auto viewObject = LveGameObject::createGameObject();

        viewObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

      //  auto lightDirect = glm::vec3{ -1.f, -1.f,  .0f };
		while (!lveWindow.shouldClose())
		{
			glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(lveWindow.getWindow(), frameTime, viewObject);

            camera.setViewYXZ(viewObject.transform.translation, viewObject.transform.rotation);

            float aspect = lveRenderer.getAspectRatio();
            //camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

			if (auto commandBuffer = lveRenderer.beginFrame()) {
                int frameIndex = lveRenderer.getFrameIndex();

                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex]
                };


             //   lightDirect =lightDirect + glm::normalize(glm::cross(lightDirect, glm::vec3{ 0.f, 0.f, -1.f })) * 1.2f *frameTime;
                
                GlobalUbo ubo{};
                ubo.prjection = camera.getProjection() ;
                ubo.view =camera.getView() ;
                
                
             //   ubo.lightDirection = lightDirect;
              /*  globalUboBuffer.writeToIndex(&ubo, frameIndex);
                globalUboBuffer.flushIndex(frameIndex);*/

                uboBuffers[frameIndex]->writeToBuffer(&ubo); 
               

                uboBuffers[frameIndex]->flush();

				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
                pointLightSystem.render(frameInfo);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}
	


    //std::unique_ptr<LveModel> createCubeModel(LveDevice& device, glm::vec3 offset) {
    //    LveModel::Builder modelBuilder{};


    //    modelBuilder.vertices = {

    //        // left face (white)
    //        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
    //        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
    //        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
    //        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
    //        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
    //        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

    //        // right face (yellow)
    //        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
    //        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
    //        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
    //        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
    //        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
    //        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

    //        // top face (orange, remember y axis points down)
    //        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    //        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    //        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    //        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    //        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    //        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

    //        // bottom face (red)
    //        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    //        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
    //        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
    //        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    //        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    //        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

    //        // nose face (blue)
    //        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    //        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    //        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    //        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    //        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    //        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

    //        // tail face (green)
    //        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    //        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    //        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    //        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    //        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    //        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}}

    //    };
    //    for (auto& v : modelBuilder.vertices) {
    //        v.position += offset;
    //    }

    //    modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
    //                      12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

    //    return std::make_unique<LveModel>(device, modelBuilder);
    //}

    void FirstApp::loadGameObject()
    {
        
        std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "models/flat_vase.obj");
        auto cube = LveGameObject::createGameObject();
        cube.model = lveModel;
        //cube.color = { 1.f, .1f, 1.f };
        cube.transform.translation = { .0f, .0f, .0f };
        cube.transform.scale = { 1.f, .5f, .5f };
        gameObjects.push_back(std::move(cube));

            std::shared_ptr<LveModel> lveModel1 = LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");
        auto cube1 = LveGameObject::createGameObject();
        cube1.model = lveModel1;
        //cube.color = { 1.f, .1f, 1.f };
        cube1.transform.translation = { 0.5f, .0f, .0f };
        cube1.transform.scale = { 1.f, .5f, .5f };
        gameObjects.push_back(std::move(cube1));

        std::shared_ptr<LveModel> lveModel2 = LveModel::createModelFromFile(lveDevice, "models/quad.obj");
        auto floor = LveGameObject::createGameObject();
        floor.model = lveModel2;
        //cube.color = { 1.f, .1f, 1.f };
        floor.transform.translation = { 0.f, .5f, .0f };
        floor.transform.scale = { 3.f, 1.f, 3.f };
        gameObjects.push_back(std::move(floor));
    
    }
}