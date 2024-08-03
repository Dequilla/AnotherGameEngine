#ifndef GE_VEC2_H_
#define GE_VEC2_H_

namespace ge
{
    
    template<typename Type>
    class Vec2
    {
    public:
        Type x;
        Type y;

        Vec2() = default;
        Vec2(Type x, Type y) : x(x), y(y) {}
        Vec2(const Vec2<Type>& other) : x(other.x), y(other.y) {}

        Vec2<Type>& operator=(const Vec2<Type>& other) = default;
        Vec2<Type>& operator=(Vec2<Type>&& other) = default;
    };

}

#endif
