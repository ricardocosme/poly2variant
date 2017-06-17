#pragma once

#include "poly2variant/detail/assign_to_variant.hpp"

#include <boost/variant.hpp>
#include <boost/fusion/algorithm.hpp>
#include <stdexcept>

namespace poly2variant {
    
template<typename Variant, typename Poly>
Variant poly2variant(Poly* poly)
{
    Variant variant;
    auto ans = boost::fusion::any
        (typename Variant::types(),
         detail::AssignToVariant<Variant, Poly>(variant, poly));
    if(!ans) throw std::logic_error
                 ("The type '"
                  + boost::typeindex::type_id_runtime(*poly).pretty_name()
                  + "' can't be assigned to the variant '"
                  + boost::typeindex::type_id<Variant>().pretty_name()
                  + "'.'");
    return variant;
}

template<typename Variant, typename Poly>
Variant poly2variant(Poly& poly)
{
    Variant variant;
    auto ans = boost::fusion::any
        (typename Variant::types(),
         detail::AssignToVariant<Variant, Poly>(variant, &poly));
    if(!ans) throw std::logic_error
                 ("The type '"
                  + boost::typeindex::type_id_runtime(poly).pretty_name()
                  + "' can't be assigned to the variant '"
                  + boost::typeindex::type_id<Variant>().pretty_name()
                  + "'.'");
    return variant;
}

template<typename Visitor, typename Visitable>
inline typename Visitor::result_type
apply_visitor(Visitor& visitor, Visitable& visitable)
{
    using types = typename Visitor::visitables;
    using variant_t = typename boost::make_variant_over<
            typename boost::mpl::transform<
                types,
                std::add_pointer<boost::mpl::placeholders::_1>
                >::type        
        >::type;
    auto variant = poly2variant<variant_t>(visitable);
    return boost::apply_visitor(visitor, variant);
}

template<typename Visitor, typename Visitable>
inline typename Visitor::result_type
apply_visitor(Visitor& visitor, const Visitable& visitable)
{
    using types = typename Visitor::visitables;
    using variant_t = typename boost::make_variant_over<
        typename boost::mpl::transform<
            typename boost::mpl::transform<
                types,
                std::add_const<boost::mpl::placeholders::_1>
                >::type,
            std::add_pointer<boost::mpl::placeholders::_1>
            >::type        
        >::type;
    auto variant = poly2variant<variant_t>(visitable);
    return boost::apply_visitor(visitor, variant);
}

template<typename Visitor, typename Visitable>
inline typename Visitor::result_type
apply_visitor(const Visitor& visitor, Visitable& visitable)
{
    using types = typename Visitor::visitables;
    using variant_t = typename boost::make_variant_over<
            typename boost::mpl::transform<
                types,
                std::add_pointer<boost::mpl::placeholders::_1>
                >::type        
        >::type;
    auto variant = poly2variant<variant_t>(visitable);
    return boost::apply_visitor(visitor, variant);
}

template<typename Visitor, typename Visitable>
inline typename Visitor::result_type
apply_visitor(const Visitor& visitor, const Visitable& visitable)
{
    using types = typename Visitor::visitables;
    using variant_t = typename boost::make_variant_over<
        typename boost::mpl::transform<
            typename boost::mpl::transform<
                types,
                std::add_const<boost::mpl::placeholders::_1>
                >::type,
            std::add_pointer<boost::mpl::placeholders::_1>
            >::type        
        >::type;
    auto variant = poly2variant<variant_t>(visitable);
    return boost::apply_visitor(visitor, variant);
}

template<typename Visitor, typename Visitable>
inline typename Visitor::result_type
apply_visitor(Visitor& visitor, Visitable* visitable)
{
    return apply_visitor(visitor, *visitable);
}

template<typename Visitor, typename Visitable>
inline typename Visitor::result_type
apply_visitor(Visitor& visitor, const Visitable* visitable)
{
    return apply_visitor(visitor, *visitable);
}

template<typename Visitor, typename Visitable>
inline typename Visitor::result_type
apply_visitor(const Visitor& visitor, Visitable* visitable)
{
    return apply_visitor(visitor, *visitable);
}

template<typename Visitor, typename Visitable>
inline typename Visitor::result_type
apply_visitor(const Visitor& visitor, const Visitable* visitable)
{
    return apply_visitor(visitor, *visitable);
}

}
