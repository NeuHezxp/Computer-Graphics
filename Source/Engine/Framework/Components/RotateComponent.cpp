#include "RotateComponent.h"
#include "Framework/Actor.h"


namespace nc
{
    CLASS_DEFINITION(RotateComponent)

        bool RotateComponent::Initialize()
    {
        return true;
    }

    void RotateComponent::Update(float dt)
    {
        m_owner->transform.euler += euler * dt;
        // Convert Euler angles (in degrees) to a quaternion
        glm::quat rotation = EulerToQuaternion(euler * dt);
        // Apply the rotation
        m_owner->transform.rotation = m_owner->transform.rotation * rotation;

    }
    void RotateComponent::ProcessGui()
    {
        ImGui::DragFloat3("Rotate", glm::value_ptr(euler));
    }

    void RotateComponent::Read(const json_t& value)
    {
        READ_NAME_DATA(value, "Rotation", euler);
    }

}