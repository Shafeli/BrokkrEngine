#pragma once
#include "../../BrokkrEngine/Logging/Logger.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//
//  Tile Component base: 
//
//  Creation requires this type os setup in the derived classes
//
// template <typename Type>
// static std::string ToString()
// {
//     return "ExampleComponent";
// }
//
// template <typename Type, typename ... Args>
// static std::unique_ptr<ExampleComponent> FromString(const std::string& str, Args&&... args)
// {
//     // Convert the string to a ExampleComponent object
//     if (str == ToString<ExampleComponent>())
//     {
//           return std::make_unique<ExampleComponent>(args);
//     }
//     return nullptr; // failed to make 
// }
//
/////////////////////////////////////////////////////////////////////////////////////////////
namespace Brokkr
{
    class CoreSystems;
    class WorldTile;

    class TileComponent
    {
    public:
        explicit TileComponent(WorldTile* pOwner, CoreSystems* pCoreSystems)
            :m_pOwner(pOwner)
            ,m_pCoreSystems(pCoreSystems)
        {
            //
        }

        virtual ~TileComponent() = default;

        virtual bool Init() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;
        virtual void RenderProcess() { }

        // What string is == to the derived type
        template <typename Type>
        static std::string ToString()
        {
            static_assert(std::is_base_of_v<TileComponent, Type>, "ToString must be implemented by a derived class of TileComponent");
            return "";
        }

        template <typename Type, typename ... Args>
        static std::unique_ptr<Type> FromString(const std::string& str, Args&&... args)
        {
            // Convert the string to a TileComponent object
            static_assert(std::is_base_of_v<TileComponent, Type>, "FromString must be implemented by a derived class of TileComponent");
            if (str == ToString<Type>())
            {
                return std::make_unique<Type>(args);
            }
            return {};
        }

    protected:
        inline static Logger m_log{"TileComponentLog"};

        WorldTile* m_pOwner = nullptr;
        CoreSystems* m_pCoreSystems = nullptr;
    };
}

