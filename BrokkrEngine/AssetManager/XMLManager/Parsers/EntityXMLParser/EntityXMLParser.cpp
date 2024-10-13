#include "EntityXMLParser.h"
#include "../../../../Application/CoreSystems/CoreSystems.h"
#include "../../../../ExternalLibs/tinyxml2.h"
#include "../../../../GameEntityManager/GameEntityManager.h"
#include "../../../../GameEntity/GameEntity.h"
#include "../../../../GameEntity/Component/HealthComponent/HealthComponent.h"
#include "../../../../GameEntity/Component/ColliderComponent/ColliderComponent.h"
#include "../../../../GameEntity/Component/RandomColorComponent/RandomColorComponent.h"
#include "../../../../GameEntity/Component/RenderComponent/SpriteComponent.h"
#include "../../../../GameEntity/Component/TransformComponent/TransformComponent.h"
#include "../../../../GameEntity/Component/BrokkrMoveController/BrokkrMoveController.h"
#include "../../../../GameEntity/Component/TriggerComponent/TriggerComponent.h"

// TODO: Build a abstract factory so components can register building methods 
bool Brokkr::EntityXMLParser::Parse(tinyxml2::XMLDocument& doc)
{
    tinyxml2::XMLElement* root = doc.RootElement();

    // Check if the root element is a "Entity" element
    if (!root || strcmp(root->Name(), "Entity") != 0)
    {
        // The XML file is not a valid Entity file
        return false;
    }

    // Loop through the entities
    for (tinyxml2::XMLElement* entityElement = root->FirstChildElement("GameEntity");

        entityElement != nullptr;

        entityElement = entityElement->NextSiblingElement("GameEntity"))
    {
        // Create a new entity
        GameEntity* pEntity = m_pCoreSystems->GetCoreSystem<GameEntityManager>()->GetNextEntityAvailable();

        // Loop through the components of the entity
        for (tinyxml2::XMLElement* componentElement = entityElement->FirstChildElement();
            componentElement != nullptr;
            componentElement = componentElement->NextSiblingElement())
        {
            // Create a new component based on the XML element's name
            std::string componentName = componentElement->Name();

            if (componentName == "SpriteComponent") //TODO: change to a switch a bit easier to use
            {
                const char* texturePath = componentElement->Attribute("texture");

                // create SpriteComponent using texture name
                pEntity->AddComponent<SpriteComponent>(texturePath, m_pCoreSystems);
            }
            else if (componentName == "TransformComponent")
            {
                float x = componentElement->FloatAttribute("x");
                float y = componentElement->FloatAttribute("y");
                float width = componentElement->FloatAttribute("width");
                float height = componentElement->FloatAttribute("height");

                // create TransformComponent using x, y, width, height
                pEntity->AddComponent<TransformComponent>
                    (
                        m_pCoreSystems
                        , Brokkr::Rect<float>({ x, y }, { width, height })
                        );
            }
            else if (componentName == "RandomColorComponent")
            {
                [[maybe_unused]] auto hax = pEntity->AddComponent<RandomColorComponent>(m_pCoreSystems);
            }
            else if (componentName == "HealthComponent")
            {
                [[maybe_unused]] const auto hax = pEntity->AddComponent<HealthComponent>(m_pCoreSystems);
            }
            else if (componentName == "ColliderComponent")
            {
                const char* overlap = componentElement->Attribute("overlap");
                int overlapConvert = 0;
                if (overlap != nullptr)
                {
                    if (strcmp(overlap, "dynamic") == 0)
                    {
                        overlapConvert = BROKKR_OVERLAP_DYNAMIC;
                    }
                    else if (strcmp(overlap, "static") == 0)
                    {
                        overlapConvert = BROKKR_OVERLAP_STATIC;
                    }
                    else if (strcmp(overlap, "all") == 0)
                    {
                        overlapConvert = BROKKR_OVERLAP_ALL;
                    }
                }

                const char* passable = componentElement->Attribute("passable");
                bool moveConvert = false;
                if (passable != nullptr)
                {
                    if (strcmp(passable, "yes") == 0)
                    {
                        moveConvert = true;
                    }
                    else if (strcmp(passable, "no") == 0)
                    {
                        moveConvert = false;
                    }
                }

                const auto transformComponent = pEntity->GetComponent<TransformComponent>();

                const auto hax = pEntity->AddComponent<ColliderComponent>(m_pCoreSystems, transformComponent->GetTransform(), overlapConvert, moveConvert);

                transformComponent->AddCollider(hax);
            }
            else if (componentName == "DefaultControllerComponent")
            {
                const char* up = componentElement->Attribute("up");
                const char* down = componentElement->Attribute("down");
                const char* left = componentElement->Attribute("left");
                const char* right = componentElement->Attribute("right");
                [[maybe_unused]] const auto hax = pEntity->AddComponent<BrokkrMoveController>(m_pCoreSystems, up, left, right, down);
            }
            else if (componentName == "TriggerComponent")
            {
                float x = componentElement->FloatAttribute("x");
                float y = componentElement->FloatAttribute("y");
                float width = componentElement->FloatAttribute("width");
                float height = componentElement->FloatAttribute("height");

                const char* eventStr = componentElement->Attribute("eventStr");

                const char* target = componentElement->Attribute("target");
                if (target != nullptr)
                {
                    int overlapConvert = 0;
                    if (strcmp(target, "self") == 0)
                    {
                        overlapConvert = BROKKR_TRIGGER_SELF;
                    }
                    else if (strcmp(target, "all") == 0)
                    {
                        overlapConvert = BROKKR_TRIGGER_ALL;
                    }
                    else if (strcmp(target, "overlap") == 0)
                    {
                        overlapConvert = BROKKR_TRIGGER_OVERLAP;
                    }

                    [[maybe_unused]] const auto hax = pEntity->AddComponent<TriggerComponent>
                        (
                            m_pCoreSystems
                            , Brokkr::Rect<float>({ x, y }, { width, height })
                            , eventStr
                            , overlapConvert
                            );

                }
              
            }
        }

        pEntity->Init();
    }
    return true;
}
