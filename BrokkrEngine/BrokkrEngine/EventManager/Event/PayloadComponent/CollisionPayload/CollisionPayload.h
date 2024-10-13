#pragma once

#include <vector>

#include "../../../BrokkrEngine/Primitives/Rect.h"
#include "../../PayloadComponent/PayloadComponent.h"

namespace Brokkr
{
    class Collider;

    class CollisionPayload final : public PayloadComponent
    {
        using EntityID = int;

        Collider* m_ObjectMoving;
        std::vector<EntityID> m_objectsHit;
        Vector2<float> m_displacementVector;

    public:
        explicit CollisionPayload(Event* pOwner, Collider* movingObject, const std::vector<EntityID>& objectsHit, const Vector2<float>& displacementVector) // i want to copy the list of ids
            : PayloadComponent(pOwner)
            , m_ObjectMoving(movingObject)
            , m_objectsHit(objectsHit)
            , m_displacementVector(displacementVector)
        {
            //
        }

        [[nodiscard]] std::vector<EntityID> GetObjectHit() const { return m_objectsHit; }
        [[nodiscard]] Collider* GetObjectMoving() const { return m_ObjectMoving; }
        [[nodiscard]] Vector2<float> GetMovingObjectsDisplacement() const { return m_displacementVector; }

        virtual const char* ToString() override;
    };

}
