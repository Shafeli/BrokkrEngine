#include "PhysicsManager.h"
#include <cassert>
#include <algorithm>
#include "../../BrokkrEngine/EventManager/Event/PayloadComponent/CollisionPayload/CollisionPayload.h"

void Brokkr::PhysicsManager::Init()
{
    // Event Handler for Update Complete event
    m_updateHandler.first = Event::kPriorityHigh; // This needs to happen before colliders update to future
    m_updateHandler.second = [this](auto&& event) { ProcessUpdate(std::forward<decltype(event)>(event)); };

    m_pEventManager->AddHandler("PhysicsUpdate", m_updateHandler);
}

Brokkr::Collider* Brokkr::PhysicsManager::CreateCollider(const Rect<float>& rect, int ownerID, bool isMoveable, int overLap)
{
    std::unique_ptr<Collider> newCollider = std::make_unique<Collider>(); // Create a unique pointer to a new Collider object
    newCollider->m_collider = rect;
    newCollider->m_ownerID = ownerID;
    newCollider->m_overlapType = overLap;
    newCollider->m_moveable = isMoveable;
    newCollider->init(m_pEventManager);

    if (!isMoveable)
    {
        // if static object
        Collider* rawPtr = newCollider.get(); // Get the raw pointer from the unique pointer
        m_staticRects.push_back(std::move(newCollider)); // Transfer ownership to the list
        m_staticColliderRoot.Insert(rawPtr->m_ownerID, rect);
        return rawPtr;
    }

    // if dynamic object
    Collider* rawPtr = newCollider.get(); // Get the raw pointer from the unique pointer
    m_dynamicRects.push_back(std::move(newCollider)); // Transfer ownership to the list
    m_dynamicColliderRoot.Insert(rawPtr->m_ownerID, rect);
    return rawPtr;
}


void Brokkr::PhysicsManager::SetWorldSize(const float width, const float numHorizontalTiles, const float height,
    const float numVerticalTiles)
{
    m_worldSize.MoveTo({ 0.f, 0.f });
    m_worldSize.Resize(width * numHorizontalTiles, height * numVerticalTiles);

    // resizing invalidates the trees
    m_staticColliderRoot.Destroy();
    m_dynamicColliderRoot.Destroy();

    m_staticColliderRoot.Init(m_worldSize);
    m_dynamicColliderRoot.Init(m_worldSize);

    // reinsert rects into trees after resizing
    for (auto& i : m_dynamicRects)
    {
        m_dynamicColliderRoot.Insert(i->m_ownerID, i->m_collider);
    }

    for (auto& i : m_staticRects)
    {
        m_staticColliderRoot.Insert(i->m_ownerID, i->m_collider);
    }
}

void Brokkr::PhysicsManager::SetWorldSize(const Vector2<float>& size)
{
    m_worldSize.MoveTo({ 0.f, 0.f });
    m_worldSize.Resize(size.x, size.y);

    // resizing invalidates the trees
    m_staticColliderRoot.Destroy();
    m_dynamicColliderRoot.Destroy();

    m_staticColliderRoot.Init(m_worldSize);
    m_dynamicColliderRoot.Init(m_worldSize);

    // reinsert rects into trees after resizing
    for (auto& i : m_dynamicRects)
    {
        m_dynamicColliderRoot.Insert(i->m_ownerID, i->m_collider);
    }

    for (auto& i : m_staticRects)
    {
        m_staticColliderRoot.Insert(i->m_ownerID, i->m_collider);
    }
}

bool Brokkr::PhysicsManager::TestMove(const Collider* pCollider, const Vector2<float>& move) const
{
    bool noHit = true;

    Rect<float> moveRect = pCollider->m_collider;
    moveRect.MoveTo(move);

    int overLap = pCollider->m_overlapType;

    std::vector<int> result;
    std::vector<int> resultTwo;

    switch (overLap) // Add to the int requested 
    {
        // if it overlaps all static object
    case BROKKR_OVERLAP_STATIC:

        result = m_staticColliderRoot.Query(moveRect);
        for (auto& i : result)
        {
            if (pCollider->m_ownerID == i)
            {
                continue;
            }
            noHit = false;
        }

        break;

        // if it overlaps all dynamic object
    case BROKKR_OVERLAP_DYNAMIC:

        result = m_dynamicColliderRoot.Query(moveRect);
        for (auto& i : result)
        {
            if (pCollider->m_ownerID == i)
            {
                continue;
            }
            noHit = false;
        }
        break;

        // if it overlaps all objects
    case BROKKR_OVERLAP_ALL:

        result = m_dynamicColliderRoot.Query(moveRect);
        resultTwo = m_staticColliderRoot.Query(moveRect);

        for (auto& i : result)
        {
            if (pCollider->m_ownerID == i)
            {
                continue;
            }
            noHit = false;
        }
        for (auto& i : resultTwo)
        {
            if (pCollider->m_ownerID == i)
            {
                continue;
            }
            noHit = false;
        }
        break;

        // if passed the wrong config report error and return nullptr
    default:
        assert(false);  //TODO: Add Logging
    }
    return noHit;
}

bool Brokkr::PhysicsManager::MoveNotify(Collider* pCollider, const Vector2<float>& newPosition, const Vector2<float>& displacementVector)
{
    bool noHit = true;
    Rect<float> testColliderMove = pCollider->m_collider;

    testColliderMove.MoveTo(newPosition);
    const int overLap = pCollider->m_overlapType;

    std::vector<int> result;
    std::vector<int> resultTwo;

    // TODO: Switch to its own method ProcessOverlap or something to help readability
    if (overLap == BROKKR_OVERLAP_STATIC) // Add to the int requested 
    {
        // if it overlaps all static object
        result = m_staticColliderRoot.Query(testColliderMove);
        for (auto& i : result)
        {
            if (pCollider->m_ownerID == i)
            {
                continue;
            }

            DispatchOnEnterEvent(pCollider, i, result, displacementVector);
            noHit = false;
        }
    }
    else if (overLap == BROKKR_OVERLAP_DYNAMIC)
    {
        // if it overlaps all dynamic object
        result = m_dynamicColliderRoot.Query(testColliderMove);
        for (auto& i : result)
        {
            if (pCollider->m_ownerID == i)
            {
                continue;
            }
            DispatchOnEnterEvent(pCollider, i, result, displacementVector);
            noHit = false;
        }
    }
    else if (overLap == BROKKR_OVERLAP_ALL)
    {
        // if it overlaps all objects
        result = m_dynamicColliderRoot.Query(testColliderMove);
        resultTwo = m_staticColliderRoot.Query(testColliderMove);

        result.insert(result.end(), resultTwo.begin(), resultTwo.end());

        for (auto& i : result)
        {
            if (pCollider->m_ownerID == i)
            {
                continue;
            }
            DispatchOnEnterEvent(pCollider, i, result, displacementVector);
            noHit = false;
        }
    }

    if (!noHit)
    {
        DispatchOnEnterEvent(pCollider, pCollider->m_ownerID, result, displacementVector);
    }

    return noHit;
}

std::vector<Brokkr::PhysicsManager::ObjectID> Brokkr::PhysicsManager::QueryAreaDynamics(const Rect<float>& area) const
{
    return m_dynamicColliderRoot.Query(area);
}

std::vector<Brokkr::PhysicsManager::ObjectID> Brokkr::PhysicsManager::QueryAreaStatics(const Rect<float>& area) const
{
    return m_staticColliderRoot.Query(area);
}

std::vector<Brokkr::PhysicsManager::ObjectID> Brokkr::PhysicsManager::QueryAreaAll(const Rect<float>& area) const
{
    std::vector<ObjectID> resultDynamic = QueryAreaDynamics(area);
    std::vector<ObjectID> resultStatic = QueryAreaStatics(area);

    // Merge
    resultDynamic.insert(resultDynamic.end(), resultStatic.begin(), resultStatic.end());

    return resultDynamic;
}

void Brokkr::PhysicsManager::Destroy()
{
    m_staticColliderRoot.Destroy();
    m_dynamicColliderRoot.Destroy();

    m_dynamicRects.clear();
    m_staticRects.clear();
}

//
void Brokkr::PhysicsManager::Remove(const Collider* pCollider)
{
    if (pCollider->m_moveable)
    {
        m_dynamicRects.remove_if([pCollider](const std::unique_ptr<Collider>& collider)
            {
                return *collider == *pCollider;
            }
        );
        RefreshDynamicTree();
    }
    else
    {
        m_staticRects.remove_if([pCollider](const std::unique_ptr<Collider>& collider)
            {
                return *collider == *pCollider;
            }
        );
        RefreshStaticTree();
    }
}

void Brokkr::PhysicsManager::RequestMoveCorrection(Collider* collider, const Vector2<float>& move)
{
    collider->m_corrections.emplace_back(move);
}

void Brokkr::PhysicsManager::RequestMove(Collider* collider, const Vector2<float>& move)
{
    collider->m_displacements.emplace_back(move);
    m_processQueue.push(collider); // Add to processing queue for manipulation 
}

void Brokkr::PhysicsManager::ProcessUpdate(const Event& event)
{
    // Create a temporary queue to hold the colliders to process
    std::queue<Collider*> tempQueue;

    // Transfer colliders from the process queue to the temporary queue
    while (!m_processQueue.empty())
    {
        Collider* pTemp = m_processQueue.front();
        auto currentPosition = pTemp->m_collider.GetPosition();

        for (auto& i : pTemp->m_displacements)
        {
            currentPosition.x += i.x;
            currentPosition.y += i.y;
            MoveNotify(pTemp, currentPosition, i); // Check the future move
        }
        m_processQueue.pop();
        tempQueue.push(pTemp);
    }

    // Process the colliders in the temporary queue
    while (!tempQueue.empty())
    {
        Collider* pTemp = tempQueue.front();
        tempQueue.pop();

        pTemp->DispatchUpdateEvent(m_pEventManager);
    }

    RefreshDynamicTree();
}


void Brokkr::PhysicsManager::RefreshDynamicTree()
{
    m_dynamicColliderRoot.Destroy();
    m_dynamicColliderRoot.Init(m_worldSize);

    for (const auto& i: m_dynamicRects)
    {
        m_dynamicColliderRoot.Insert(i->m_ownerID, i->m_collider);
    }
}

void Brokkr::PhysicsManager::RefreshStaticTree()
{
    m_staticColliderRoot.Destroy();
    m_staticColliderRoot.Init(m_worldSize);

    for (auto& i : m_staticRects)
    {
        m_staticColliderRoot.Insert(i->m_ownerID, i->m_collider);
    }
}

void Brokkr::PhysicsManager::DispatchOnEnterEvent(Collider* movingObject, int IDofObjectSendingTo, const std::vector<ObjectID>& hitIDs, const Vector2<float>& displacementVector)
{
    
    const std::string pTemp = m_eventString + std::to_string(IDofObjectSendingTo);
    m_event = Event::EventType(pTemp.c_str(), Event::kPriorityNormal);

    //Loading a payload of Data to that allows for responses to the collision
    m_event.AddComponent<CollisionPayload>(movingObject, hitIDs, displacementVector); // TODO: add logging for if the event payload fails

    m_pEventManager->PushEvent(m_event);

#if DEBUG_LOGGING
    const std::string debugMessage = "Dispatched Collider Event " + pTemp;
    m_fileLog.Log(Logger::LogLevel::kDebug, debugMessage);
#endif

}

