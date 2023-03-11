#include <chrono>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <json/json.h>
#include <list>

template< typename T >
std::string TestType()
{
    if constexpr( json::IsPrimitive<T> )
    {
        return "Primitive";
    }
    else if constexpr( json::IsEnum<T> )
    {
        return "Enum";
    }
    else if constexpr( json::IsArray<T> )
    {
        return "Array";
    }
    else if constexpr( json::IsObject<T> )
    {
        return "Object";
    }
    else if constexpr( json::IsOptional<T> )
    {
        return "Optional";
    }
    else
    {
        return "Unknown";
    }
}

TEST_CASE("testing json type traits")
{
    CHECK( TestType<bool>() == "Primitive" );
    CHECK( TestType<int64_t>() == "Primitive" );
    CHECK( TestType<std::string>() == "Primitive" );
    CHECK( TestType< std::vector<int64_t> >() == "Array" );
    CHECK( TestType< std::vector<std::string> >() == "Array" );

    struct A{};
    CHECK( TestType<A>() == "Object" );
    CHECK( TestType< std::optional<int64_t> >() == "Optional" );
    CHECK( TestType< std::optional< std::vector<int64_t> > >() == "Optional" );
    CHECK( TestType< std::optional<A> >() == "Optional" );
    CHECK( TestType< std::list<int64_t> >() == "Unknown" );

    enum class B{};
    CHECK( TestType<B>() == "Enum" );

    //nlohmann::json json;
    //std::vector<int64_t> arr = { 1, 2, 3 };
    //json["a"] = arr;
    //arr.clear();
    //arr = json.get<decltype(arr)>();
    //std::cout << json.dump(2) << std::endl;
}

TEST_CASE("testing direct json convert")
{
    int a = 1;
    nlohmann::json json;
    json::JsonRef jsonValue( json );
    json::ToJson( jsonValue, json::bind( a, "a" ) );
    std::cout << (*jsonValue).dump(2) << std::endl;
}

namespace json
{

void ToJson( const util::TimePoint& tp, JsonRef& json )
{
    std::string t = util::ToString(tp);
    ToJson( t, json );
}

void FromJson( const JsonConstRef& json, util::TimePoint& tp )
{
    std::string t;
    FromJson( json, t );
    tp = util::ToTimePoint(t);
}

}

struct A
{
    bool a_ = true;
    int64_t b_ = 99;
    std::string c_ = "hello";
    std::vector<int64_t> d_ = { 1, 2, 3, 4, 5 };
    util::TimePoint e_ = util::TimeNow();
    std::optional<std::string> f_;
};

void ToJson( const A& a, json::JsonRef& json )
{
    using namespace json;
    ToJson( json, bind( a.a_, "a" ),
                  bind( a.b_, "b" ),
                  bind( a.c_, "c" ),
                  bind( a.d_, "d" ),
                  bind( a.e_, "e" ),
                  bind( a.f_, "f" ) );
}

void FromJson( const json::JsonConstRef& json, A& a )
{
    using namespace json;
    FromJson( json, bind( a.a_, "a" ),
                    bind( a.b_, "b" ),
                    bind( a.c_, "c" ),
                    bind( a.d_, "d" ),
                    bind( a.e_, "e" ),
                    bind( a.f_, "f" ) );
}

enum class C
{
    I = 1,
    J = 2
};

//void ToJson( const C& b, json::json& json )
//{
    //std::string value;
    //switch (b)
    //{
        //case C::I:
            //value = "one";
            //break;
        //case C::J:
            //value = "two";
            //break;
    //}
    //json::ToJson( value, json );
//}

//void FromJson( const json::json& json, C& b )
//{
    //std::string s;
    //json::FromJson( json, s );
    //if ( s == "one" )
    //{
        //b = C::I;
    //}
    //else if ( s == "two" )
    //{
        //b = C::J;
    //}
//}

struct B
{
    A x_;
    std::optional<std::string> y_ = "world";
    C z_ = C::J;
};

void ToJson( const B& b, json::JsonRef& json )
{
    using namespace json;
    ToJson( json, bind( b.x_, "x" ),
                  bind( b.y_, "y" ),
                  bind( b.z_, "z" ) );
}

void FromJson( const json::JsonConstRef& json, B& b )
{
    using namespace json;
    FromJson( json, bind( b.x_, "x" ),
                    bind( b.y_, "y" ),
                    bind( b.z_, "z" ) );
}

TEST_CASE("testing complex json convert")
{
    B b;
    nlohmann::json json;
    json::JsonRef jsonValue( json );
    ToJson( b, jsonValue );
    std::cout << (*jsonValue).dump(2) << std::endl;

    C h = C::J;
    auto i = std::underlying_type_t<C>(h);
    std::cout << "i: " << i << std::endl;

    B c{ .x_ = { .a_ = false, .b_ = 0, .c_ = {}, .d_ = {}, .e_ = {}, .f_ = {} }, .y_ = {}, .z_ = {} };
    FromJson( json, c );
    std::cout << b.y_.value_or( "null" ) << std::endl;
}
