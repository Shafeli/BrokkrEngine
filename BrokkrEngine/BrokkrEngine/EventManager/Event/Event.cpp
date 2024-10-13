#include "Event.h"
#include <memory>
#include <vector>

#include "../../Utilities/Hash.h"
#include "PayloadComponent/PayloadComponent.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// Event Type
//////////////////////////////////////////////////////////////////////////////////////////////

Brokkr::Event::EventType::EventType(const char* eventTypeName, const unsigned& priority)
    : m_priority(priority)
    , m_eventTypeName(eventTypeName)
{
    m_typeName = Hash::Murmur3Hash(eventTypeName, std::strlen(eventTypeName), s_kHashSeed);
}

uint32_t Brokkr::Event::EventType::HashEventString(const char* eventTypeName)
{
    return Hash::Murmur3Hash(eventTypeName, std::strlen(eventTypeName), s_kHashSeed);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Event
//////////////////////////////////////////////////////////////////////////////////////////////

Brokkr::Event::Event(const EventType& type)
    :m_event(type)
    , m_type(type.GetTypeHash())
{}
