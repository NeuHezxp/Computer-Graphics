#include "World05.h"

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

namespace nc
{
    bool World05::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene.json");
        m_scene->Initialize();

        for (int i = 0; i < 10; i++)
        {
            auto actor = CREATE_CLASS_BASE(Actor, "tree");
            actor->name = StringUtils::CreateUnique("tree");
            actor->transform.position = glm::vec3{ randomf(-10, 10), 0, randomf(-10, 10) };
            actor->transform.scale = glm::vec3{ randomf(0.5f, 3.0f), randomf(0.5, 3.0f), 0 };
            actor->Initialize();
            m_scene->Add(std::move(actor));
        }

        return true;
    }

    void World05::Shutdown()
    {
    }

    void World05::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();


        auto actor = m_scene->GetActorByName<Actor>("Ogre");
        m_scene->GetActorByName<Actor>("Ogre Refract");
        m_scene->GetActorByName<Actor>("Ogre Reflect");
        
        auto material = actor->GetComponent<ModelComponent>()->material;

        material->ProcessGui();
        material->Bind();

        //material = GET_RESOURCE(Material, "materials/refraction.prog");
        if (material)
        {
            ImGui::Begin("Refraction");

            m_refraction = 1.0f + std::fabs(std::sin(m_time));

            ImGui::DragFloat("IOR", &m_refraction, 0.01f, 1, 3);
            auto program = material->GetProgram();
            program->Use();
            program->SetUniform("ior", m_refraction);

            ImGui::End();
        }

        m_time += dt;

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World05::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //shows grid
        m_scene->Draw(renderer);
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
