#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

namespace nc
{
    bool World06::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene_framebuffer.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(512, 512);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "Materials/postprocess.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }

        return true;
    }

    void World06::Shutdown()
    {
    }

    void World06::Update(float dt)
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        // set postprocess gui
        ImGui::Begin("Post-Process");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);
		///GRAYSCALE
        bool effect = m_params & GRAYSCALE_MASK;
        if (ImGui::Checkbox("Grayscale", &effect))
        {
            (effect) ? m_params |= GRAYSCALE_MASK : m_params &= ~GRAYSCALE_MASK;
        }
        ///INVERSION
        effect = m_params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect))
        {
            (effect) ? m_params |= INVERT_MASK : m_params &= ~INVERT_MASK;
        }
        ///COLOR TINT
        effect = m_params & COLORTINT_MASK;
        if (ImGui::Checkbox("Color Tint", &effect)) {
            if (effect)
                m_params |= COLORTINT_MASK;
            else
                m_params &= ~COLORTINT_MASK;
        }
        if (effect)ImGui::ColorEdit4("Tint Color", glm::value_ptr(m_tint)); //color picker only when enabled
		///GRAIN
        effect = m_params & GRAIN_MASK;
        if (ImGui::Checkbox("grain", &effect))
        {
            (effect) ? m_params |= GRAIN_MASK : m_params &= ~GRAIN_MASK;
        }
        ///SCANLINES
        effect = m_params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanlines", &effect))
        {
            (effect) ? m_params |= SCANLINE_MASK : m_params &= ~SCANLINE_MASK;
        }
        if (effect) ImGui::SliderFloat("Scanline Intensity", &m_scanlineIntensity, 0.0f, 1.0f);
        ///RIPPLE
        effect = m_params & RIPPLE_MASK;
        if (ImGui::Checkbox("Ripple Effect", &effect))
        {
            (effect) ? m_params |= RIPPLE_MASK : m_params &= ~RIPPLE_MASK;
        }
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

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        // *** PASS 1 ***
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame({ 0, 0, 0 });
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        // *** PASS 2 ***
        m_scene->GetActorByName("postprocess")->active = true;

        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->GetActorByName("postprocess")->Draw(renderer);

        // post-render
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
