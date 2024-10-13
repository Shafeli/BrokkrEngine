#pragma once
#include <memory>
#include <vector>
#include "../Utilities/IDGenerator.h"
#include "TileComponent/TileComponent.h"
#include "TileComponent/TileRenderComponent/TileRenderComponent.h"

namespace Brokkr
{
    class WorldTile
    {

        inline static Logger m_log{ "TileLog" };

        int m_id;

        TileRenderComponent* m_renderComponent = nullptr;

        std::vector<std::unique_ptr<TileComponent>> m_pComponents;

    public:

        // Create Object Creation / Deletion 
        ///////////////////////////////////////////
        WorldTile() : m_id(IDGenerator::GenerateUniqueID()) {}

        WorldTile(const WorldTile& other) = delete;
        WorldTile& operator=(const WorldTile& other) = delete;

        WorldTile(WorldTile&& other) noexcept
            : m_id(other.m_id), m_pComponents(std::move(other.m_pComponents))
        {
            //
        }

        WorldTile& operator=(WorldTile&& other) noexcept = delete;

        ~WorldTile();

        [[nodiscard]] int GetId() const { return m_id; }

        void Init();

        // No error == true
        bool Reset();

        // Object Update Components
        ///////////////////////////////////////////
        void Update() const;

        // Object Serialize TODO: Flesh out this system more
        // Object Deserialize TODO: Flesh out this system more

        // Object Get a Component 
        ///////////////////////////////////////////
        template <typename ComponentType>
        ComponentType* GetComponent()
        {
            // Iterate through all components in the vector
            for (auto& component : m_pComponents)
            {
                // Attempt to dynamically cast the component to the specified type
                ComponentType* target = dynamic_cast<ComponentType*>(component.get());

                // If the cast is successful
                if (target)
                {
                    return target;
                }
            }

            // If no component of type is found
            return nullptr;
        }

        // Object Add a Component
        ///////////////////////////////////////////
        template <typename ComponentType, typename ... Args>
        ComponentType* AddComponent(Args&&... args)
        {
            // Create a instance of the component type 
            std::unique_ptr<ComponentType> newComponent = std::make_unique<ComponentType>(std::forward<Args>(args)...);

            // Get a raw pointer to the component
            ComponentType* result = newComponent.get();

            // Add the component to the vector
            m_pComponents.emplace_back(std::move(newComponent));

            // Return a pointer
            return result;
        }

        //TODO: new Idea:
        // is to take in a str name of component to type check like lua as well as a vector of 
        // strings that are the XML attributes of so the component can construct it self and the XML will
        // still hold to SoC and but still kind of break it but making the component responsible for
        // their behavior as well as their construction.
        //
        // another idea is to just build a long string in xml parsing and just send the string to the component
        // and the component and string parse for the data it need to construct might be faster then string and vector
        //
        // another idea pack the data into a buffer and reinterpret it as a struct of the data needed to build
        // the component
        template <typename ComponentType>
        ComponentType* AddComponentFromString(const std::string& str, std::vector<const std::string&> strDataArray);

        void RenderProcess();
    };
}
