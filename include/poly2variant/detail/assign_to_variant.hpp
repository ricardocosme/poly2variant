#pragma once

#include <boost/type_index.hpp>
#include <type_traits>

namespace poly2variant { namespace detail {

template<typename Variant, typename Poly>
struct AssignToVariant
{
    using poly_pointed_t = typename std::remove_pointer<Poly>::type;
    
    AssignToVariant(Variant& variant, Poly* poly)
        : variant(variant)
        , poly(poly) {}
    
    template<typename T>
    bool assign(std::false_type) const
    {
        variant = poly;
        return true;
    }
    
    template<typename T>
    bool assign(std::true_type) const
    {
        using pointed_t = typename std::remove_pointer<T>::type;
        if(boost::typeindex::type_id<pointed_t>() ==
           boost::typeindex::type_id_runtime(*poly))
        {
            variant = static_cast<T>(poly);
            return true;
        }
        return false;
    }
    
    template<typename T>
    bool operator()(T) const
    {
        using pointed_t = typename std::remove_pointer<T>::type;
        return assign<T>
            (typename std::is_base_of<poly_pointed_t, pointed_t>::type());
    }
    
    Variant& variant;
    mutable Poly* poly;
};
        
}}
