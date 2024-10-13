#pragma once


namespace Brokkr
{
    class ScriptSystemManager;
    class CoreSystems;

    class ScriptSystem
    {

    public:
        explicit ScriptSystem(ScriptSystemManager* pSystemManager, CoreSystems* pCoreSystems)
            :m_pSystemManager(pSystemManager)
            ,m_pCoreSystems(pCoreSystems)
        {
            //
        }
        virtual ~ScriptSystem() = default;
        virtual void Init() = 0;
        virtual void Destroy() = 0;

    protected:
        ScriptSystemManager* m_pSystemManager = nullptr;
        CoreSystems* m_pCoreSystems = nullptr;
    };
}