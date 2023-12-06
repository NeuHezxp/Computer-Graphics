#pragma once
namespace nc
{
	
	class Editor
	{
	public:
		void Update();
		void ProcessGui(class Scene* scene);

	private:
		bool m_active = true;
		class Object* m_selected = nullptr;
		enum ResourceType {
			TYPE_TEXTURE,
			TYPE_MODEL,
			TYPE_MATERIAL,
			TYPE_SHADER,
			ALL
		};

		ResourceType m_currentResourceType = ALL;
	};
}
