#pragma once

#include <list>
#include <string>

#include "QuadTree.h"
#include "../Application/CoreSystems/CoreSystems.h"
#include "../EventManager/EventManager.h"
#include "../EventManager/Event/Event.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                             CollisionManager: 
// Sources:
// Quad & Oct Trees - Data Structures For Performance - https://www.youtube.com/watch?v=OKiBmQ6ZNyU
// Quadtree Data Structure" chapter in the book "Data Structures and Algorithms in C++"
// K-d Trees - Computerphile - https://www.youtube.com/watch?v=BK5x7IUTIyU
// Quadtrees and Octrees for Representing Spatial Information - https://www.youtube.com/watch?v=xFcQaig5Z2A
// Quirky Quad Trees Part1: Static Spatial Acceleration - https://www.youtube.com/watch?v=ASAowY6yJII&t=134s&ab_channel=javidx9
// Quirky Quad Trees Part2: Dynamic Objects In Trees - https://www.youtube.com/watch?v=wXF3HIhnUOg&t=5s
// C++ Quadtree Implementation Part 1 - https://www.youtube.com/watch?v=Ha0n9XMIOhI&t=439s
//
////////////////////////////////////////////////////////////////////////////////////////////

#define DEBUG_LOGGING 0 

#define BROKKR_OVERLAP_STATIC 0
#define BROKKR_OVERLAP_DYNAMIC 1
#define BROKKR_OVERLAP_ALL 2

namespace Brokkr
{
    class EventManager;

    //TODO: this is getting more complex move to its own file
    class Collider
    {
        inline static Logger m_fileLog{ "ColliderLog" };
    public:
        Rect<float> m_collider;
        std::vector<Vector2<float>> m_displacements{};
        std::vector<Vector2<float>> m_corrections{};

        int m_ownerID = -1;
        int m_overlapType = -1;
        bool m_moveable = false;
        bool m_inprocess = false;

        EventManager::EventHandler m_updateHandler;
        Event m_event;
        std::string m_eventString;

        Collider()
            : m_event(Event::EventType("ColliderError", Event::kPriorityNormal))
        {
            //
        }

        Collider(const Collider&) = delete; // Disable copy constructor
        Collider& operator=(const Collider&) = delete; // Disable copy assignment operator

        void Update([[maybe_unused]] const Event& event)
        {
            if (m_moveable)
            {

                for (const auto& i : m_displacements)
                {
                    m_collider.AdjustX(i.x);
                    m_collider.AdjustY(i.y);
                }

                for (const auto& i : m_corrections)
                {
                    m_collider.AdjustX(i.x);
                    m_collider.AdjustY(i.y);
                }

                m_inprocess = false;
                m_corrections.clear();
                m_displacements.clear();
            }
        }

        void init(EventManager* pEventManager)
        {
            m_eventString = "UpdatePosition" + std::to_string(m_ownerID);

           // Event Handler for Update Complete event
            m_updateHandler.first = Event::kPriorityHigh; // update before render data is pushed
            m_updateHandler.second = [this](auto&& event) { Update(std::forward<decltype(event)>(event)); };


            pEventManager->AddHandler(m_eventString.c_str(), m_updateHandler); // this is kinda of hacked in i guess
        }

        bool operator==(const Collider& other) const
        {
            return m_ownerID == other.m_ownerID;
        }

        void DispatchUpdateEvent(EventManager* pEventManager)
        {
            m_event = Event::EventType(m_eventString.c_str(), Event::kPriorityNormal);
            pEventManager->PushEvent(m_event);
        }
    };

    class PhysicsManager final : public CoreSubsystem
    {
        using ObjectID = int;

        inline static Logger m_fileLog{ "PhysicsManagerLog" };
        inline static constexpr size_t kMaxDepth = 10;

        EventManager::EventHandler m_updateHandler;
        EventManager* m_pEventManager;
        std::string m_eventString;
        Event m_event;

        // TODO: sense moving trees to id only the colliders can now be in a vector and use cache better
        std::list<std::unique_ptr<Collider>> m_dynamicRects;
        std::list<std::unique_ptr<Collider>> m_staticRects;

        std::queue<Collider*> m_processQueue;

        Rect<float> m_worldSize;

        QuadTree m_staticColliderRoot;
        QuadTree m_dynamicColliderRoot;

    public:
        explicit PhysicsManager(CoreSystems* pCoreManager)
            : CoreSubsystem(pCoreManager)
            , m_eventString("OnEnter") // Defaulting the starting world rect size
            , m_event(Event::EventType("ColliderError", Event::kPriorityNormal))
            , m_staticColliderRoot()
            , m_dynamicColliderRoot()
        {
            m_pEventManager = m_pCoreManager->GetCoreSystem<EventManager>(); // Event manager access
        }

        void Init();
        Collider* CreateCollider(const Rect<float>& rect, int ownerID, bool isMoveable, int overLap = 0);

        void SetWorldSize(float width, float numHorizontalTiles, float height, float numVerticalTiles);
        void SetWorldSize(const Vector2<float>& size);
        [[nodiscard]] Rect<float> GetWorldSize() const { return m_worldSize; }

        // This does not send an event but will return if it hits something
        bool TestMove(const Collider* pCollider, const Vector2<float>& move) const;

        // moves incoming collider and send events to all colliders it overlaps
        bool MoveNotify(Collider* pCollider, const Vector2<float>& newPosition, const Vector2<float>& displacementVector);

        [[nodiscard]] std::vector<ObjectID> QueryAreaDynamics(const Rect<float>& area) const;
        [[nodiscard]] std::vector<ObjectID> QueryAreaStatics(const Rect<float>& area) const;
        [[nodiscard]] std::vector<ObjectID> QueryAreaAll(const Rect<float>& area) const;

        virtual void Destroy() override;
        void Remove(const Collider* pCollider);
        static void RequestMoveCorrection(Collider* collider, const Vector2<float>& move);
        void RequestMove(Collider* collider, const Vector2<float>& move);

        // Reaction to the update requests
        void ProcessUpdate([[maybe_unused]] const Event& event);

    private:

        // Refresh Dynamic tree
        void RefreshDynamicTree();
        void RefreshStaticTree();

        // Sends the event for overlap
        void DispatchOnEnterEvent(Collider* movingObject, int IDofObjectSendingTo, const std::vector<ObjectID>& hitIDs, const Vector2<float>& displacementVector);
    };
}
