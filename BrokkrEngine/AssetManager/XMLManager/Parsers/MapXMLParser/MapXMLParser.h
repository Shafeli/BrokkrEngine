#pragma once
#include "../../XMLManager.h"

namespace Brokkr
{
    class MapXMLParser final : public XMLParser
    {

    public:
        explicit MapXMLParser(CoreSystems* pCoreSystems)
            : XMLParser(pCoreSystems)
        {
            //
        }

        virtual bool Parse(tinyxml2::XMLDocument& doc) override;
    };
}

