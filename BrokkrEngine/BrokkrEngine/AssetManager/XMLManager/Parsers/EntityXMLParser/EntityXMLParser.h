#pragma once

#include "../../XMLManager.h"

namespace Brokkr
{
    class CoreSystems;
    class GameEntityManager;

    class EntityXMLParser final : public XMLParser
    {
     
    public:
        explicit EntityXMLParser(CoreSystems* pCoreSystems)
            : XMLParser(pCoreSystems)
        {
            //
        }

        virtual bool Parse(tinyxml2::XMLDocument& doc) override;
    };
}
