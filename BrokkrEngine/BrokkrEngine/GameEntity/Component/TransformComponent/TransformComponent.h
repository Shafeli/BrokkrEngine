#pragma once
#include "../Component.h"
#include "../../../Primitives/Rect.h"
#include "../../BrokkrEngine/Logging/Logger.h"
#include "../../BrokkrEngine/PhysicsManager/PhysicsManager.h"

/*#define BROKKR_IGNORE_COORDINATE_Y 1
#define BROKKR_IGNORE_COORDINATE_X 2
#define BROKKR_IGNORE_NONE 3*/

namespace Brokkr
{
    class ColliderComponent;
    class GameEntity;

    class TransformComponent final : public Component
    {
        Rect<float> m_transform;
        Logger m_transformLog;

        ColliderComponent* m_collider = nullptr;
        GameEntity* m_pOwner = nullptr;
        EventManager* m_pEventManager = nullptr;

        EventManager::EventHandler m_updateHandler;

    public:
        TransformComponent(GameEntity* pOwner, CoreSystems* pCoreSystems, Rect<float> transform);

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;

        [[nodiscard]] Rect<float> GetTransform() const
        {
            return m_transform;
        }

        void AddCollider(ColliderComponent* pColliderComponent);

        // When ever Physics updates the final displacement for the frame update this after 
        void UpdatePosition([[maybe_unused]] const Event& event);

        void Resize(float width, float height); // TODO: this should now have an event that physics needs to respond to 

        /*
        virtual void Serialize(const std::string& filePath) override;
        virtual void Deserialize(const std::string& filePath) override;
        */
    };
}
