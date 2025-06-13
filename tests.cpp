#define DOCTEST_CAT_(x,y) x##y
#define DOCTEST_CAT(x,y)  DOCTEST_CAT_(x,y)
#define DOCTEST_TEST_CASE(name)                       \
    static void DOCTEST_CAT(t_,__LINE__)();           \
    static bool DOCTEST_CAT(r_,__LINE__) =            \
        (DOCTEST_CAT(t_,__LINE__)(), true);           \
    static void DOCTEST_CAT(t_,__LINE__)()
#define DOCTEST_CHECK(expr)                           \
    if(!(expr)) throw std::runtime_error(#expr);

#include <stdexcept>
#include <iostream>
#include "models.h"

DOCTEST_TEST_CASE("Stats positive") {
    Student s("x","x",{5,4,3});
    DOCTEST_CHECK(s.average()==4.0);
    DOCTEST_CHECK(s.min().value()==3);
    DOCTEST_CHECK(s.max().value()==5);
}

DOCTEST_TEST_CASE("Stats empty") {
    Student s("y","y");
    DOCTEST_CHECK(s.average()==0.0);
    DOCTEST_CHECK(!s.min());
    DOCTEST_CHECK(!s.max());
}

int main() try {
    std::cout<<"[ALL TESTS PASSED]\n";
    return 0;
} catch(const std::exception& e){
    std::cerr<<"Test failed: "<<e.what()<<'\n';
    return 1;
}