#pragma once

/////////////////////////////////////////////////
// State interface
/////////////////////////////////////////////////

namespace Brokkr
{
    class State
    {
    public:
        /////////////////////////////////////////////////
        // override state methods
        /////////////////////////////////////////////////
        virtual ~State() = default;
        virtual void Init() = 0;
        virtual void ResetStateValues() = 0;
        virtual void Update() = 0;
        virtual void Draw() = 0;
        virtual void Destroy() = 0;

        /////////////////////////////////////////////////
        // optional state methods
        /////////////////////////////////////////////////
        virtual void Pause() {}
        virtual void Resume() {}

    };
}