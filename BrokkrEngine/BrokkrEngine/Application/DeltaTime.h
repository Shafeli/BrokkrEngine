#pragma once
#include "../Application/CoreSystems/CoreSubsystem/CoreSubsystem.h"
#include "CoreSystems/CoreSystems.h"

namespace Brokkr
{
    class DeltaTime final : public CoreSubsystem
    {
        inline static Logger m_log{ "DeltaTimeLog" };
        double m_timeStep = 0.;
    public:

        explicit DeltaTime(CoreSystems* pCoreManager)
            : CoreSubsystem(pCoreManager)
        {
            //
        }

        void SetTime(const double time) { m_timeStep = time; }

        [[nodiscard]] double GetSeconds()const { return m_timeStep; }

        [[nodiscard]] double GetMilliseconds()const { return m_timeStep * 1000.; }

        // for cool thing like casting to a float by default timeStep is seconds
        explicit operator double() const { return m_timeStep; }
        virtual void Destroy() override { }
    };
}