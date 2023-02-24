#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "json/json.h"
#include "doctest/doctest.h"

struct A
{
    int64_t a_ = 0;
    bool b_ = false;

//private:
    //static inline constexpr std::array names_ = { "a", "b" };

    //friend inline void ToJson( const A& a, nlohmann::json& json )
    //{
        //json::ToJson( A::names_, json, a.a_, a.b_ );
    //}
};

template< typename T >
struct JsonConverter;

template<>
struct JsonConverter< A >
{
    static void ToJson( const A& a, nlohmann::json& json )
    {
        json::ToJson( json, { a.a_, "a" },
                            { a.b_, "b" } );
    }
};

TEST_CASE("testing the factorial function")
{
    A a { .a_ = 1, .b_ = true };
    nlohmann::json json;
    ToJson( a, json );
    std::cout << json.dump();
}
