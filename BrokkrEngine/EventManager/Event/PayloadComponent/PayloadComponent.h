#pragma once


namespace Brokkr
{
    class Event;

    class PayloadComponent
    {

    public:
        explicit PayloadComponent(Event* pOwner)
            : m_pOwner(pOwner)
        {
            //
        }

        virtual ~PayloadComponent() = default;
        virtual const char* ToString() = 0;

    protected:
        Event* m_pOwner = nullptr;
    };

    class PayloadTest final : public PayloadComponent
    {
    public:
        PayloadTest(Event* pOwner)
            : PayloadComponent(pOwner)
        {

        }

        virtual const char* ToString() override;
    };

    inline const char* PayloadTest::ToString()
    {
        return "Event Payload Test\n";
    }
}
