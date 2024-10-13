#pragma once

//TODO: add an init to interface in next refactor
namespace Brokkr
{
    class CoreSystems;

    class CoreSubsystem
    {
        int m_refCount;
    public:
        explicit CoreSubsystem(CoreSystems* pCoreManager)
            : m_refCount(0)
            , m_pCoreManager(pCoreManager)
        {
            //
        }

        virtual ~CoreSubsystem() = default;
        virtual void Destroy() = 0;

        void AddRef() { m_refCount++; }
        int Release()
        {
            return --m_refCount;
        }

    protected:
        CoreSystems* m_pCoreManager;
    };

}
