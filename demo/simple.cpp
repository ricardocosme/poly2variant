#include "poly2variant/poly2variant.hpp"
#include <iostream>

struct bird;
struct cockatiel;
struct eagle;

using visitables = boost::mpl::vector<cockatiel, eagle, bird>;

struct animal { virtual ~animal() = default; };
struct bird : animal { virtual ~bird() = default; };
struct cockatiel : bird {};
struct eagle : bird {};
    
struct visitor
{
    using result_type = void;
    using visitables = ::visitables;
    void operator()(const bird* o) const
    { std::cout << "bird" << std::endl; }
    void operator()(const cockatiel* o) const
    { std::cout << "cockatiel" << std::endl; }
    void operator()(const eagle* o) const
    { std::cout << "eagle" << std::endl; }
};

int main()
{
    cockatiel o;
    const animal* base = &o;
    poly2variant::apply_visitor(visitor{}, base);
}
