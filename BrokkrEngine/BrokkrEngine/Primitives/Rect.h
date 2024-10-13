#pragma once
#include <algorithm>
#include "Vector2.h"

namespace Brokkr
{
    template<typename TypeName>
    class Rect
    {
        Vector2<TypeName> m_size;
        Vector2<TypeName> m_position;

    public:
        Rect() = default;

        Rect(const Vector2<TypeName>& position, const Vector2<TypeName>& size)
            : m_size(size), m_position(position)
        {}

        [[nodiscard]] TypeName GetHeight() const { return m_size.y; }
        [[nodiscard]] TypeName GetWidth() const { return m_size.x; }
        [[nodiscard]] TypeName GetX() const { return m_position.x; }
        [[nodiscard]] TypeName GetY() const { return m_position.y; }

        void SetHeight(TypeName y) { m_size.y = y; }
        void SetWidth(TypeName x) { m_size.x = x; }
        void SetX(TypeName x) { m_position.x = x; }
        void SetY(TypeName y) { m_position.y = y; }

        void AdjustHeight(TypeName y) { m_size.y += y; }
        void AdjustWidth(TypeName x) { m_size.x += x; }
        void AdjustX(TypeName x) { m_position.x += x; }
        void AdjustY(TypeName y) { m_position.y += y; }

        [[nodiscard]] TypeName GetTop() const { return m_position.y; }
        [[nodiscard]] TypeName GetBottom() const { return m_position.y + m_size.y; }
        [[nodiscard]] TypeName GetLeft() const { return m_position.x; }
        [[nodiscard]] TypeName GetRight() const { return m_position.x + m_size.x; }
        [[nodiscard]] TypeName GetAdjustedY(TypeName y) const { return m_position.y + y; }
        [[nodiscard]] TypeName GetAdjustedX(TypeName x) const { return m_position.x + x; }

        bool operator==(const Rect<TypeName>& other) const
        {
            return m_position == other.m_position && m_size == other.m_size;
        }

        void MoveTo(const Vector2<TypeName>& position)
        {
            m_position = position;
        }

        void Resize(TypeName width, TypeName height)
        {
            m_size = { width, height };
        }

        [[nodiscard]] bool Contains(const Vector2<TypeName>& point) const
        {
            return point.x >= m_position.x && point.x < m_position.x + m_size.x
                && point.y >= m_position.y && point.y < m_position.y + m_size.y;
        }

        [[nodiscard]] bool Intersects(const Rect<TypeName>& other) const
        {
            return m_position.x < other.m_position.x + other.m_size.x &&
                m_position.x + m_size.x > other.m_position.x &&
                m_position.y < other.m_position.y + other.m_size.y &&
                m_position.y + m_size.y > other.m_position.y;
        }

        // Returns a Rect of the two Rects intersection area
        [[nodiscard]] Rect<TypeName> Intersection(const Rect<TypeName>& other) const
        {
            // Coordinates of the top left corner of the intersection rect
            int x1 = std::max<TypeName>(m_position.x, other.m_position.x);
            int y1 = std::max<TypeName>(m_position.y, other.m_position.y);

            // Coordinates of the bottom right corner of the intersection rect
            const int x2 = std::min<TypeName>(m_position.x + m_size.x, other.m_position.x + other.m_size.x);
            const int y2 = std::min<TypeName>(m_position.y + m_size.y, other.m_position.y + other.m_size.y);

            // Check if the rects intersect
            if (x2 <= x1 || y2 <= y1) 
            {
                // The rects do not intersect
                return Rect({ 0, 0 }, { 0, 0 });
            }

            // return the intersection rects
            return Rect({ x1, y1 }, { x2 - x1, y2 - y1 });
        }

        Vector2<TypeName>& GetPosition()
        {
            return m_position;
        }
    };
}
