#include "Editor.h"
#include "scene.h"
#include "Components/CameraComponent.h"

namespace nc
{
	void Editor::Update()
	{
		if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent)) m_active = !m_active;
		
	}

	void Editor::ProcessGui(Scene* scene)
	{

		if (!m_active) return;
		//show recoures
		ImGui::Begin("resources");
		auto resources = GET_RESOURCES(Resource);
		for (auto& resource : resources)
		{
			if(ImGui::Selectable(resource->name.c_str(),resource.get() == m_selected))
			{
				m_selected = resource.get();
			}
		}
		ImGui::End();

		//show scene
		ImGui::Begin("Scene");
		scene->ProcessGui();
		ImGui::Separator();
		//show actors
		ImGui::BeginChild("actors");
		if (ImGui::BeginPopupContextWindow())
		{
			auto cameras = scene->GetComponents<CameraComponent>();
			auto camera = (cameras.empty()) ? cameras[0] : nullptr;
			if(ImGui::MenuItem("Create Empty"))
			{
				auto actor = CREATE_CLASS(Actor);
				actor->name = StringUtils::CreateUnique(actor->GetClassName());
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 2.0f;
				}
				actor->Initialize();
				m_selected = actor.get();
				scene->Add(std::move(actor));
			}
			if(ImGui::MenuItem("Sphere"))
			{
				auto actor = CREATE_CLASS_BASE(Actor,"Sphere");
				actor->name = StringUtils::CreateUnique(actor->name);
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 2.0f;
				}
				actor->Initialize();
				m_selected = actor.get();
				scene->Add(std::move(actor));
			}
			if(ImGui::MenuItem("Camera"))
			{
				auto actor = CREATE_CLASS_BASE(Actor, "Camera");
				actor->name = StringUtils::CreateUnique(actor->name);
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 2.0f;
				}
				actor->Initialize();
				m_selected = actor.get();
				scene->Add(std::move(actor));
			}
			if(ImGui::MenuItem("Light"))
			{
				auto actor = CREATE_CLASS_BASE(Actor, "Light");
				actor->name = StringUtils::CreateUnique(actor->name);
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 2.0f;
				}
				actor->Initialize();
				m_selected = actor.get();
				scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Cube"))
			{
				auto actor = CREATE_CLASS_BASE(Actor, "Cube");
				actor->name = StringUtils::CreateUnique(actor->name);
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 2.0f;
				}
				actor->Initialize();
				m_selected = actor.get();
				scene->Add(std::move(actor));
			}
			ImGui::EndPopup();
		}

		for (auto& actor : scene->m_actors)
		{
			if (ImGui::Selectable(actor->name.c_str(), actor.get() == m_selected))
			{
				m_selected = actor.get();

			}
		}
		ImGui::EndChild();
		ImGui::End();
		//show inspector
		ImGui::Begin("Inspector");
		if (m_selected)
		{
			m_selected->ProcessGui();
		}
		
		ImGui::End();

	}
}
