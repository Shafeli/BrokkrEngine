#pragma once
#include <cmath>

namespace Brokkr
{
    template <typename TypeName>
    class Vector2
    {
    public:
        TypeName x, y;

        // construction
        Vector2() { x = y = 0; }
        Vector2(const Vector2<TypeName>& v2) = default;

        Vector2(const TypeName x, const TypeName y)
            : x(x)
            , y(y)
        { }

        // prime operators
        Vector2<TypeName>& operator=(const Vector2<TypeName>& other) = default;
        Vector2<TypeName>& operator*=(TypeName scalar) { x *= scalar; y *= scalar; return (*this); }
        bool operator==(const Vector2<TypeName>& other) const { return (x == other.x && y == other.y); }
        bool operator!=(const Vector2<TypeName>& other) const { return (x != other.x || y != other.y); }
        Vector2<TypeName>& operator+=(const Vector2<TypeName>& other) { x += other.x; y += other.y; return (*this); }
        Vector2<TypeName>& operator-=(const Vector2<TypeName>& other) { x -= other.x; y -= other.y; return (*this); }
        Vector2<TypeName> operator-() const { return Vector2<TypeName>(-x, -y); }  // inverse of the vector 

        // other math
        [[nodiscard]] TypeName Length() const
        {
            return std::sqrt((x * x) + (y * y));
        }

        [[nodiscard]] TypeName LengthSquared() const { return ((x * x) + (y * y)); }

        void Normalize()
        {
            if (x == 0 && y == 0)
                return;
            const TypeName len = Length();
            x /= len;
            y /= len;
        }

        [[nodiscard]] bool IsExtremelyCloseTo(const Vector2<TypeName>& right) const
        {
            if (const Vector2<TypeName> diff = right - (*this); diff.Length() < 0.01f)
                return true;
            return false;
        }
    };

    template <typename TypeName>
    Vector2<TypeName> operator+(const Vector2<TypeName>& left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left.x + right.x, left.y + right.y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator-(const Vector2<TypeName>& left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left.x - right.x, left.y - right.y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator*(const Vector2<TypeName>& left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left.x * right.x, left.y * right.y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator*(const Vector2<TypeName>& left, const TypeName right)
    {
        return Vector2<TypeName>(left.x * right, left.y * right);
    }

    template <typename TypeName>
    Vector2<TypeName> operator*(const TypeName left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left * right.x, left * right.y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator/(const Vector2<TypeName>& left, const TypeName right)
    {
        return Vector2<TypeName>(left.x / right, left.y / right);
    }

}
