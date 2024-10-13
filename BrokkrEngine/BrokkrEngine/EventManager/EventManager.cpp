#include "EventManager.h"

bool Brokkr::EventManager::EventHandlerComparer::operator()(const EventHandler& left, const EventHandler& right) const
{
    // priority values of the handlers
    if (left.first != right.first)
    {
        // Handlers with higher priority values should come before handlers with lower priority values
        return left.first > right.first;
    }

    // If priority values are equal, use target_type() comparison
    return left.second.target_type().before(right.second.target_type());
    // how this works :
    // left.second.target_type().before(right.second.target_type()) is used to compare the std::type_info
    // objects of the stored event handlers based on their names.
    // This comparison determines the order in which the event handlers are stored in the eventHandlers
    // container, with handlers whose types have "lower" names (lexicographically earlier)
    // being stored first.
    // Source : https://en.cppreference.com/w/cpp/types/type_info
    //              &
    //          https://stackoverflow.com/questions/53467813/comparing-two-type-info-from-typeid-operator
    //      For debugging down the line (lexicographically earlier)
    //      : simply means the name of the type comes first in alphabetical order
}

bool Brokkr::EventManager::EventComparer::operator()(const Event& left, const Event& right) const
{
    // Compare the priority levels of the events directly
    return left.GetPriorityLevel() < right.GetPriorityLevel();
}

void Brokkr::EventManager::AddHandler(const char* eventTypeString, const EventHandler& handler)
{
    AddHandler(Event::EventType::HashEventString(eventTypeString), handler);
}

void Brokkr::EventManager::RemoveHandler(const char* eventTypeString, const EventHandler& handler)
{
    RemoveHandler(Event::EventType::HashEventString(eventTypeString), handler);
}

void Brokkr::EventManager::AddHandler(uint32_t eventHash, const EventHandler& handler)
{
    // Add the handler to the set of handlers for the specified event type
    m_handlers[eventHash].insert(handler);
}

void Brokkr::EventManager::RemoveHandler(uint32_t eventHash, const EventHandler& handler)
{
    // Find the set of handlers for the given event type
    if (const auto it = m_handlers.find(eventHash); it != m_handlers.end())
    {
        auto& handlers = it->second;

        // Find the iterator to the specified handler in the set of handlers
        const auto handlerIt = handlers.find(handler);

        // If the handler was found, remove it from the set of handlers
        if (handlerIt != handlers.end())
        {
            handlers.erase(handlerIt);
        }
    }
}

void Brokkr::EventManager::PushEvent(const Event& event)
{
    m_eventQueue.push(event);
}

void Brokkr::EventManager::ProcessEvents()
{
    while (!m_eventQueue.empty())
    {
        // Get the next event in the queue
        const Event& event = m_eventQueue.top();

        // Find the set of handlers for the event type of the current event
        if (auto it = m_handlers.find(event.GetType()); it != m_handlers.end())
        {
            // Call each event handler in the set of handlers for the current event type
            const auto& handlers = it->second;
            for (const auto& handler : handlers) // 69.99% frame time here TODO: fix Idea: Asyc processing
            {
                handler.second(event);
            }
        }

        // Remove the processed event from the event queue
        m_eventQueue.pop();
    }
}

void Brokkr::EventManager::Destroy()
{
    //TODO:
}
