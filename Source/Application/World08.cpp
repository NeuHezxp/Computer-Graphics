#include "World08.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

namespace nc
{
    bool World08::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene_editor.json");
        m_scene->Load("Scenes/scene_cel_shading.json");
        m_scene->Initialize();

		m_editor = std::make_unique<Editor>(); // make unique it gets deleted if this world is deleted

        //create depth texture
        auto texture = std::make_shared<Texture>(); //make shared it does not get deleted and is for multi-ownership
        texture->CreateDepthTexture(1024, 1024);
        ADD_RESOURCE("depth_texture", texture);
		//create debth buffer
        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateDepthBuffer(texture);
        ADD_RESOURCE("depth_buffer", framebuffer);

        //set teaxture to debug sprite
        auto material = GET_RESOURCE(Material, "Materials/sprite.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }
        auto materials = GET_RESOURCES(Material);
		for (auto& material : materials)
		{
			material->depthTexture = texture;
		}

        return true;
    }

    void World08::Shutdown()
    {
    }

    void World08::Update(float dt)
    {
        m_time += dt;
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
		m_editor->Update();
        m_editor->ProcessGui(m_scene.get());
        ImGui::Begin("cel");
        ImGui::SliderInt("Cel Shading Levels", &levels, 1, 10);
        ImGui::End();

        // set postprocess shader
        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
        if (program)
        {
            program->Use();
            program->SetUniform("blend", m_blend);
            program->SetUniform("params", m_params);
            program->SetUniform("tint", m_tint);
            program->SetUniform("grainAmount", m_grainAmount);
            program->SetUniform("scanLine", m_scanlineIntensity);
            program->SetUniform("time", m_time);
            program->SetUniform("rippleIntensity", m_rippleIntensity);

        }

        auto celShaderProgram = GET_RESOURCE(Program, "shaders/cel_shading.prog");
        if (celShaderProgram) {
            celShaderProgram->Use();
            celShaderProgram->SetUniform("levels", levels);
        }

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World08::Draw(Renderer& renderer)
    {
        // *** PASS 1 ***
      //  m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "depth_buffer");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

		renderer.ClearDepth();
        auto program = GET_RESOURCE(Program, "shaders/shadow_depth.prog");
        program->Use();

        auto lights = m_scene->GetComponents<LightComponent>();
		for (auto& light : lights)
		{
			if(light->castShadow)
			{
                glCullFace(GL_FRONT);
				glm::mat4 shadowMatrix = light->GetShadowMatrix();
				program->SetUniform("shadowVP", shadowMatrix);
			}
		}
		auto models = m_scene->GetComponents<ModelComponent>();
        for(auto model : models)
        {
            if(model->castShadow)
            {
				program->SetUniform("model",model->m_owner->transform.GetMatrix());
				model->model->Draw();
            }
        }

        framebuffer->Unbind();

        // *** PASS 2 ***
        //m_scene->GetActorByName("postprocess")->active = true;
       

        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);

        // post-render
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
