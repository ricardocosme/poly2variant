#include "poly2variant/poly2variant.hpp"
#include "poly_visitor/visitor.hpp"
#include "timer.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/fusion/algorithm.hpp>
#include <iostream>
#include <string>
#include <vector>

struct D1;
struct D2;
struct D3;
struct D4;
struct D5;
struct D6;
struct D7;

using base_visitor = poly_visitor::base_visitor
    <D1, D2, D3, D4, D5, D6, D7>;

struct B
{ PURE_VISITABLE(base_visitor) };
struct D1 : B
{
    VISITABLE(base_visitor)
    virtual ~D1() = default;
};
struct D2 : B
{
    VISITABLE(base_visitor)
    virtual ~D2() = default;
};
struct D3 : B
{
    VISITABLE(base_visitor)
    virtual ~D3() = default;
};
struct D4 : B
{
    VISITABLE(base_visitor)
    virtual ~D4() = default;
};
struct D5 : B
{
    VISITABLE(base_visitor)
    virtual ~D5() = default;
};
struct D6 : B
{
    VISITABLE(base_visitor)
    virtual ~D6() = default;
};
struct D7 : B
{
    VISITABLE(base_visitor)
    virtual ~D7() = default;
};

struct Visitor
{
    using result_type = void;
    result_type operator()(const D1&) const
    {}
    result_type operator()(const D2&) const
    {}
    result_type operator()(const D3&) const
    {}
    result_type operator()(const D4&) const
    {}
    result_type operator()(const D5&) const
    {}
    result_type operator()(const D6&) const
    {}
    result_type operator()(const D7&) const
    {}
};

using visitables = boost::mpl::vector<D1, D2, D3, D4, D5, D6, D7>;

struct PtrVisitor
{
    using result_type = void;
    using visitables = ::visitables;
    result_type operator()(const D1*) const
    {}
    result_type operator()(const D2*) const
    {}
    result_type operator()(const D3*) const
    {}
    result_type operator()(const D4*) const
    {}
    result_type operator()(const D5*) const
    {}
    result_type operator()(const D6*) const
    {}
    result_type operator()(const D7*) const
    {}
};

struct FillVector
{
    FillVector(std::vector<std::unique_ptr<B>>& vec) : vec(vec) {}
    template<typename T>
    void operator()(T)
    { vec.emplace_back(new typename std::remove_pointer<T>::type); }
    std::vector<std::unique_ptr<B>>& vec;
};

int main(int argc, char **argv)
{
    std::size_t n(1);
    
    if(argc > 1)
        n = std::stol(argv[1]);
    
    std::cout << "usage: benchmark [number_of_iterations]"
              << std::endl << std::endl
              << "[Number of iterations] = " << n
              << std::endl;
    
    std::vector<std::unique_ptr<B>> vec;
    for(std::size_t i(0); i < n; ++i)
        boost::mpl::for_each<visitables>(FillVector(vec));
    
    {
        timer<std::chrono::microseconds> t("poly visitor ");
        for(std::size_t i(0); i < n; ++i)
        {
            auto base = vec[i].get();
            poly_visitor::apply_visitor(Visitor{}, *base);
        }
    }
    {
        timer<std::chrono::microseconds> t("boost variant");
        for(std::size_t i(0); i < n; ++i)
        {
            auto base = vec[i].get();
            poly2variant::apply_visitor(PtrVisitor{}, base);
        }
    }
    {
        timer<std::chrono::microseconds> t("dynamic_cast ");
        for(std::size_t i(0); i < n; ++i)
        {
            auto base = vec[i].get();
            if(dynamic_cast<D1*>(base))
            {}
            else if(dynamic_cast<D2*>(base))
            {}
            else if(dynamic_cast<D3*>(base))
            {}
            else if(dynamic_cast<D4*>(base))
            {}
            else if(dynamic_cast<D5*>(base))
            {}
            else if(dynamic_cast<D6*>(base))
            {}
            else if(dynamic_cast<D7*>(base))
            {}
            else
            {
                //Turn off optimizations. This branch forces the
                //compiler to consider the type switching at runtime.
                abort();
            }
        }
    }
}
