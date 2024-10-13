#pragma once
#include "../TileComponent.h"
#include "../../../Primitives/Rect.h"

namespace Brokkr
{
    class WorldTile;
    class CoreSystems;

    class TileTransformComponent final : public TileComponent
    {
        Rect<float> m_transform{}; // Default constructed 
    public:
        TileTransformComponent(WorldTile* pOwner, CoreSystems* pCoreSystems);

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;

        [[nodiscard]] Rect<float> GetTransform() const { return m_transform; }
        [[nodiscard]] bool IsPointOnTransform(const Vector2<float>& position) const;

        // TODO: event for moving so Tile Manager class can sort by positions
        void SetPosition(const Vector2<float>& position);
        void SetSize(const Vector2<float>& size);

        template <typename Type>
        static std::string ToString()
        {
            return "TransformComponent";
        }

        template <typename Type, typename ... Args>
        static std::unique_ptr<TileTransformComponent> FromString(const std::string& str, Args&&... args)
        {
            // Convert the string to a TransformComponent object
            if (str == ToString<TileTransformComponent>())
            {
                return std::make_unique<TileTransformComponent>(args);
            }
            return nullptr; // failed to make 
        }
    };
}


