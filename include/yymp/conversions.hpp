//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/** \file yymp/conversions.hpp
 * \brief Conversions between `std` types and \ref yymp::typelist "typelists".
 */

#ifndef YYMP__CONVERSIONS_HPP
#define YYMP__CONVERSIONS_HPP

#include <type_traits>  // std::conditional, std::integral_constant
#include <utility>      // std::integer_sequence

#include <yymp/typelist_fwd.hpp> // yymp::is_typelist

namespace yymp {

/////////////////////////////////
// DECLARATIONS
//

/** \brief Defines `type` as `T` if `T` is a \ref yymp::typelist "typelist", otherwise `type` is a \ref yymp::typelist "typelist" containing only `T`. 
 */
template< class T >
using as_typelist = std::conditional< yymp::is_typelist<T>::value, T, typelist<T> >;

#if __cpp_lib_integer_sequence >= 201304
/** \brief Converts a `std::integer_sequence` to a \ref yymp::typelist "typelist" of `std::integral_constant`s of the same type. 
 *
 * Requires C++14 or `std::integer_sequence`.
 */
template< class IndexSequence >
struct integer_sequence_to_typelist;

/** \brief Converts a \ref yymp::typelist "typelist" of homogeneous `std::integral_constant`s to a `std::integer_sequence` of the same type.
 *
 * Requires C++14 or `std::integer_sequence`.
 */
template< class TypeList >
struct typelist_to_integer_sequence;
#endif

/////////////////////////////////
// IMPLEMENTATION
//

#if __cpp_lib_integer_sequence >= 201304
template< class T, T... Integers >
struct integer_sequence_to_typelist< ::std::integer_sequence< T, Integers... > > {
    using type = typelist< ::std::integral_constant<T, Integers>... >;
};

template< class T, T... Integers >
struct typelist_to_integer_sequence< typelist< std::integral_constant<T, Integers>... > > {
    using type = ::std::integer_sequence<T, Integers...>;
};
#endif
    
}

#endif // YYMP__CONVERSIONS_HPP