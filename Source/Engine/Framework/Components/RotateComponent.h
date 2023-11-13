#pragma once
#include "Component.h"

namespace nc
{
	class RotateComponent : public Component
	{
	public:
		CLASS_DECLARATION(RotateComponent)

		bool Initialize() override;
		void Update(float dt) override;

		void ProcessGui() override;

	public:
		glm::vec3 euler{ 0 };
	};
}
