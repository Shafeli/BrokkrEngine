#include "XMLManager.h"
#include "../../Application/CoreSystems/CoreSystems.h"
#include "../../ExternalLibs/tinyxml2.h"
#include "Parsers/EntityXMLParser/EntityXMLParser.h"
#include "Parsers/MapXMLParser/MapXMLParser.h"


void Brokkr::XMLManager::Init(CoreSystems* coreSystems)
{
    m_pCoreSystems = coreSystems;
    
    AddParser<EntityXMLParser>(coreSystems); // adding the Entity parser
    AddParser<MapXMLParser>(coreSystems); // adding the Entity parser 
}

void Brokkr::XMLManager::Load(const char* fileName) const
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(fileName) != tinyxml2::XML_SUCCESS) 
    {
        // Error handling
        return;
    }

    // Loop through the parsers until one of them successfull
    for (auto& parser : m_parsers) 
    {
        if (parser->Parse(doc)) 
        {
            // Parsing successful
            return;
        }
    }

    // Parsing unsuccessful
    //TODO: Add logging
}

void Brokkr::XMLManager::Destroy()
{
    m_pCoreSystems = nullptr;
    m_parsers.clear();
}

