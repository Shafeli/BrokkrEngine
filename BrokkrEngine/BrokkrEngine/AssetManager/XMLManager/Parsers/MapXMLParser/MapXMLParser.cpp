#include "MapXMLParser.h"

#include <iostream>
#include <sstream>

#include "../../../../Application/CoreSystems/CoreSystems.h"
#include "../../../../ExternalLibs/tinyxml2.h"
#include "../../../../WorldTile/WorldTile.h"
#include "../../../../WorldTile/TileComponent/TileTransformComponent/TileTransformComponent.h"
#include "../../../../WorldTile/TileComponent/TileRenderComponent/TileRenderComponent.h"
#include "../../../../WorldTileManager/WorldTileManager.h"
#include "../../../../PhysicsManager/PhysicsManager.h"

namespace Brokkr
{
    class PhysicsManager;
}

bool Brokkr::MapXMLParser::Parse(tinyxml2::XMLDocument& doc)
{
    tinyxml2::XMLElement* root = doc.RootElement();

    // Check if the root element is a "Map" element
    if (!root || strcmp(root->Name(), "map") != 0)
    {
        // The XML file is not a valid Map file
        return false;
    }

    // Properties
    int tileWidth = root->IntAttribute("tilewidth");
    int tileHeight = root->IntAttribute("tileheight");

    // Properties
    int mapWidth = root->IntAttribute("width");
    int mapHeight = root->IntAttribute("height");

    // The layer element containing tile data
    tinyxml2::XMLElement* layerElement = root->FirstChildElement("layer");
    if (!layerElement)
    {
        m_log.Log(Logger::LogLevel::kError, "Map Parse Error : layer check failed");
        return false;
    }

    // The tile data from the layer element
    tinyxml2::XMLElement* dataElement = layerElement->FirstChildElement("data");
    if (!dataElement)
    {
        m_log.Log(Logger::LogLevel::kError, "Map Parse Error : CSV Data check failed");
        return false;
    }

    // Parse the tile data as a CSV string
    const char* csvData = dataElement->GetText();
    std::istringstream stream(csvData);
    std::vector<int> tileValues;

    // Read each line of the CSV data
    std::string line;
    while (std::getline(stream, line))
    {
        // Remove the newline character from the end of the line
        if (!line.empty() && line[line.size() - 1] == '\n')
        {
            line.erase(line.size() - 1);
        }

        // Parse the comma-separated values in the line
        std::istringstream lineStream(line);
        std::string value;
        while (std::getline(lineStream, value, ','))
        {
            if (tileValues.size() == static_cast<size_t>(mapWidth * mapHeight))
            {
                break;
            }
            int intValue = std::stoi(value);
            tileValues.push_back(intValue);
        }
    }

    // numberTilesHorizontal = windowWidth / tileWidth = 1920 / 32 = 60;
    // numberTilesVertical = windowHeight / tileHeight = 1080 / 32 = 33.75;
    // totalNumberTiles = numTilesHorizontal * numTilesVertical = 60 * 34 = 2040;

    // roundNumberTiles = 64 * 64 = 4096;
    m_pCoreSystems->GetCoreSystem<PhysicsManager>()->SetWorldSize({ (float)mapWidth * tileWidth, (float)mapHeight * tileHeight });

    static int count = 0;
    // Create WorldTiles for each tile
    auto worldTileManager = m_pCoreSystems->GetCoreSystem<WorldTileManager>();

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            // Retrieve the value of the tile at (x, y)
            int tileValue = tileValues[y * mapWidth + x];

            // Create a new WorldTile object
            auto temp = worldTileManager->GetAvailable();
            ++count;

            // Make a transform and set its position
            TileTransformComponent* transform;
            transform = temp->AddComponent<TileTransformComponent>(temp,m_pCoreSystems);  // NOLINT(clang-diagnostic-undefined-func-template)
            transform->SetPosition({ static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight) });
            transform->SetSize({ static_cast<float>(tileWidth),static_cast<float>(tileHeight) });

            // Make a TileRenderComponent and set it
            TileRenderComponent* pDEBUG;
            pDEBUG = temp->AddComponent<TileRenderComponent>(temp, m_pCoreSystems);  // NOLINT(clang-diagnostic-undefined-func-template)

            temp->Init();

            // based on its value do x thing
            if (tileValue != 0)
            {
                pDEBUG->SetTexture("Test");
            }
        }
    }

    std::string testing = "WorldTile Built : ";
    testing += std::to_string(count);

    m_log.Log(Logger::LogLevel::kDebug, testing);

    return true;
}

// TODO: Idea: Add a builder component?
//  to Tiles and Entities so i can pass it a ID from the XML map and based on that ID it adds the appropriate
//  Sprite & Components to build the Objects that can be made from an Object XML or something
//  then later on Object tool can be made to build the XML for the objects 