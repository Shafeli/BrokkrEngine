#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              --------------------
//                                How to use system:
//                              --------------------
//
//                              *Adding new Event*
//-------------------------------------------------------------------------------------------------------------
// Add an event type is now pretty easy its just:
//      
//      Event PlayerDiedEvent(Event::EventType("PlayerDied", Event::kPriorityNormal));
//
//      Event PlayerDiedEvent(Event::EventType("Event Name String", some int level of priority));
//
//
//      Event Name String Note: Doesn't really matter other then Best to not Space words out.
//                          HelloWord = Good : Hello World = Bad
//
//      Priority level Note: there are pre-defined levels, but are not required for the system to work 
//
//-------------------------------------------------------------------------------------------------------------
//                              *Event Handler for an Event*
//-------------------------------------------------------------------------------------------------------------
// Type alias for event handler functions, including a priority value is
// " using EventHandler = std::pair<int, std::function<void(const Event&)>> "
//
// All you need to have is the hash or the const char* for the event, then some int level of priority pre pre-defined or not
// and last a functor or function pointer / lambda for a call back 
//
//
//     eventManager.AddHandler("New Event", { priority level num for handlers sorting ,
//     functor or function pointer / lambda for a call back });
//
//                              *Event Handler Removal*
//---------------------------------------------------------------------------------------------------------------
// Caller/ Event Handler/ adding party is responsible for removing handlers from system currently
// This also means once register Handlers are not consumed on Event processing **Very important to note** 
//
// At the end of use of the handler should be removed from the EventManager
//         eventManager.RemoveHandler("New Event", Handler);
//                              or
//         eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerDied"), Handler);
//
// removal can also be done with hashing the string before the removal or the caller can store the hash and
// the handler for fast adding and removal
//----------------------------------------------------------------------------------------------------------------
//
//                              *Event Payloads
//----------------------------------------------------------------------------------------------------------------
// Event payloads are component like system with an interface things to remember when creating custom payloads first
// needs an owner pointer to the event container holding it. In add component function this pointer is the event container and
// implicitly added to the payload component upon adding to the container.
//
// Payloads are meant to be read only so to not have a chance to change event data before another Handler/ listener can do its thing
// this means in a Collision payload the Entity Ids should be returned and not their pointers that way events can get the
// entity pointer from the Object pool with the ids
//
//                          --- Important to remember when using this system---
//      **** EventHandlers are sorted for every event std::set so the higher priority levels are processed first ****
//      **** Events are sorted separate the Handlers priority level has noting to do with the Event priority level  ****
//
//-------------------
// ++Simple Example++ 
//-------------------
//
//        Custom event handler function for PlayerMoved event
//      void OnPlayerMoved(const Event& event)
//      {
//          std::cout << "PlayerMoved event handled Data payload test : " << event.GetComponent<PayloadTest>()->ToString() << "\n";
//      }
//
//       Custom event handler function for PlayerDied event
//      void OnPlayerDied(const Event& event)
//      {
//          std::cout << "PlayerDied event handled  \n";
//      }
//
//       Custom event handler function for PlayerDied event
//      void OnPlayerDiedPriority(const Event& event)
//      {
//          std::cout << "PlayerDied event handled with the High priority for Player death events \n";
//      }
//
//      int main()
//      {
//          // Create an instance of EventManager
//          EventManager eventManager;
//      
//          std::string player = "Player variable to capture";
//      
//          // Register event handlers for events
//          eventManager.AddHandler(Event::EventType::HashEventString("PlayerMoved"), { 0, [player]([[maybe_unused]] const Event& event)
//              {
//                  std::cout << player << "\n";
//              } });
//      
//          eventManager.AddHandler(Event::EventType::HashEventString("PlayerMoved"), { Event::kPriorityNormal, OnPlayerMoved });
//          eventManager.AddHandler("PlayerDied", { Event::kPriorityNormal, OnPlayerDied });
//
//          // Higher priority value for death events
//          eventManager.AddHandler(Event::EventType::HashEventString("PlayerDied"), { Event::kPriorityMax, OnPlayerDiedPriority });  
//      
//          const Event PlayerDiedEvent(Event::EventType("PlayerDied", Event::kPriorityNormal));
//      
//          // Create some events
//          Event playerMovedEvent(Event::EventType("PlayerMoved", Event::kPriorityMax));
//          if (playerMovedEvent.AddComponent<PayloadTest>())
//          {
//              std::cout << "Success the Payload component was added to the event\n";
//          }
//      
//          // Push the events to the event queue
//          eventManager.PushEvent(PlayerDiedEvent);
//          eventManager.PushEvent(playerMovedEvent);
//      
//          // Process the events
//          eventManager.ProcessEvents();
//      
//          // Remove event handlers
//          eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerMoved"), { 0, [player]([[maybe_unused]] const Event& event)
//              {
//                  std::cout << player << "\n";
//              } });
//      
//          eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerMoved"), { Event::kPriorityNormal, OnPlayerMoved });
//          eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerDied"), { Event::kPriorityNormal, OnPlayerDied });
//          eventManager.RemoveHandler("PlayerDied", { Event::kPriorityMax, OnPlayerDiedPriority });
//      
//          return 0;
//      }
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <functional>
#include <queue>
#include <set>
#include <unordered_map>
#include "../Application/CoreSystems/CoreSystems.h"
#include "../Application/CoreSystems/CoreSubsystem/CoreSubsystem.h"
#include "Event/Event.h"

// TODO: Think about borrowing how luaSystem does pushing a function to possible make working with events smoother
// TODO: Expanding this system later stretch goal:
// replacing / adding to this system a message process center and give classes ways to registers for a event manager with their IDs
// then the processing center could focus on things like helping sort events to entities in a range or other manners and aid in concurrency
namespace Brokkr
{

    class EventManager final : public CoreSubsystem
    {
    public:
        // event handler functions, including a priority value
        using EventHandler = std::pair<int, std::function<void(const Event&)>>;

        // Comparison functor for event handler functions
        struct EventHandlerComparer
        {
            // takes two EventHandler objects and returns a bool
            bool operator()(const EventHandler& left, const EventHandler& right) const;
        };

        // Comparison functor for events based on their enum value
        struct EventComparer
        {
            // Definition of operator() that takes two Event objects and returns a bool
            bool operator()(const Event& left, const Event& right) const;
        };

    private:
        // Map of event types to a set of event handlers, sorted using the custom comparator
        std::unordered_map<uint32_t, std::set<EventHandler, EventHandlerComparer>> m_handlers;

        // Priority queue of events waiting to be processed, sorted by their enum value
        std::priority_queue<Event, std::vector<Event>, EventComparer> m_eventQueue;

    public:
        explicit EventManager(CoreSystems* pCoreManager): CoreSubsystem(pCoreManager) { }

        // Add a handler for an event by string
        void AddHandler(const char* eventTypeString, const EventHandler& handler);

        // Remove a handler for an event by string
        void RemoveHandler(const char* eventTypeString, const EventHandler& handler);

        // Add an event handler for a given event type with a specified priority
        void AddHandler(uint32_t eventHash, const EventHandler& handler);

        // Remove an event handler for a given event type
        void RemoveHandler(uint32_t eventHash, const EventHandler& handler);

        // Add an event to the event queue
        void PushEvent(const Event& event);

        // Process all events currently in the event queue
        void ProcessEvents();

        virtual void Destroy() override;
    };
}


