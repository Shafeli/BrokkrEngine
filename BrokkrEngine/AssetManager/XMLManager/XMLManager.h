#pragma once
#include <memory>
#include <vector>
#include "../../AssetManager/AssetManager.h"

namespace tinyxml2
{
    class XMLDocument;
}

namespace Brokkr
{
    class CoreSystems;

    class XMLParser 
    {
    public:
        XMLParser(CoreSystems* pCoreSystems)
            :m_pCoreSystems(pCoreSystems)
        {
            //
        }
        virtual ~XMLParser() = default;
        virtual bool Parse(tinyxml2::XMLDocument& doc) = 0;

    protected:
        CoreSystems* m_pCoreSystems = nullptr;
        inline static Logger m_log{ "XMLParserLog" };
    };

    class XMLManager final : public AssetSubsystem
    {
        CoreSystems* m_pCoreSystems = nullptr;
        std::vector<std::unique_ptr<XMLParser>> m_parsers;

    public:
        XMLManager(AssetManager* assetManager) : AssetSubsystem(assetManager) {}
        void Init(CoreSystems* coreSystems);
        void Load(const char* fileName) const;
        virtual void Destroy() override;

        template <typename ParserType, typename ... Args>
        ParserType* AddParser(Args&&... args)
        {
            // Create a instance of the Parser Type
            std::unique_ptr<ParserType> newParserType = std::make_unique<ParserType>(std::forward<Args>(args)...);

            // Get a raw pointer to the parser
            ParserType* result = newParserType.get();

            // Add the parser to the vector
            m_parsers.emplace_back(std::move(newParserType));

            // Return a pointer
            return result;
        }

        template <typename ParserType>
        ParserType* GetParser()
        {
            // Iterate through all components in the vector
            for (auto& parser : m_parsers)
            {
                // If the cast is successful
                if (ParserType* target = dynamic_cast<ParserType*>(parser.get()))
                {
                    return target;
                }
            }

            // If no type is found
            return nullptr;
        }
    };
}
