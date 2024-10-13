#pragma once
#include <vector>
#include "../Primitives/Rect.h"

namespace Brokkr
{
    class QuadTree
    {
        inline static constexpr size_t kMaxObjectPerNode = 4;

        using ObjectID = int;
        using NodeDataContainer = std::vector<std::pair<ObjectID, Rect<float>>>;
        
        ObjectID m_objectID; // Objects ID
        Rect<float> m_rect;

        size_t m_MaxObjectPerNode = kMaxObjectPerNode;

        bool m_isLeaf = true;

        NodeDataContainer m_colliderNodes;
        std::vector<QuadTree> m_leafs;

    public:
        QuadTree() = default;
        QuadTree(ObjectID data, const Rect<float>& rect, const size_t maxObjectPerNode);

        void Init(const Rect<float>& rect);
        void Insert(const ObjectID& data, const Rect<float>& rect);

        [[nodiscard]] std::vector<ObjectID> Query(const Rect<float>& rect) const;

        //TODO: Revisit removing from tree this is leaving empty nodes and over time will cause a overflow 
        void Remove(const  ObjectID& data, const Rect<float>& rect); // - temp fix destroy and rebuild after a move 
        void Destroy();

    private:
        void Divide();
        void CreateLeafNodes();
        void MoveObjectsToLeafs();
        void ClearObjects();
        void RemoveEmptyNodes();
    };
}
