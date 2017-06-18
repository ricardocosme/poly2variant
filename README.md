# poly2variant

poly2variant is an experience using a variant like [Boost.Variant](http://www.boost.org/doc/libs/1_64_0/doc/html/variant.html)
as a safe type switching mechanism to discover derived types from
bases when there are a known set of polymorphic types.

This solution is safer and faster than a bundle of `dynamic_casts` but
is not better than [Poly
Visitor](http://github.com/ricardocosme/poly_visitor). The last
complains at compile time about new types introduced in the class hierarchy
and it is more efficient too.

Maybe, the only reason to use poly2variant versus [Poly
Visitor](http://github.com/ricardocosme/poly_visitor) is its
non-intrusive approach in the class hierarchy: the last demands the
addition of visitable macros.


## Building blocks
`poly2variant` takes a variant type and a polymorphic object and returns a variant.

`apply_visitor` visits a polymorphic object with a visitor (It
constructs a variant under the hood.)


## Demo
```c++
using visitables = boost::mpl::vector<cockatiel, eagle>;

struct bird { virtual ~bird() = default; };
struct cockatiel : bird {};
struct eagle : bird {};
    
struct visitor
{
    using result_type = void;
    using visitables = ::visitables;
    void operator()(const cockatiel* o) const
    { std::cout << "cockatiel" << std::endl; }
    void operator()(const eagle* o) const
    { std::cout << "eagle" << std::endl; }
};

int main()
{
    cockatiel o;
    const bird* base = &o;
    poly2variant::apply_visitor(visitor{}, base);
}
```
