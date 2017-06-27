//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef YYMP__FILTER_HPP
#define YYMP__FILTER_HPP

#include <yymp/typelist_fwd.hpp>

#include <yymp/transform.hpp>
#include <yymp/group.hpp>

#include <yymp/detail/indexed_typelist.hpp>

#include <type_traits>  // std::conditional, std::is_same

namespace yymp {

template< template< class... > class Predicate, class TypeList >
struct filter;

template< class T, class TypeList >
struct filter_of;

template< class TypeList >
struct filter_duplicates;

#if __cpp_lib_integer_sequence >= 201304

/** \brief Retrieves the indices of types in \a TypeList that pass the \a Predicate, as the `std::index_sequence` type member `type`. 
 *
 * The order of and duplicates in \a TypeList are preserved.
 * Requires C++14 or higher, or `std::integer_sequence`s to be available.
 */
template< template< class... > class Predicate, class TypeList >
struct indices_where;

/** \brief Retrieves the indices of \a T in \a TypeList , as the `std::index_sequence` type member `type`. 
 *
 * Requires C++14 or higher, or `std::integer_sequence`s to be available.
 */
template< class T, class TypeList >
struct indices_of;

#endif 

/** \brief Retains all types `T` in \a Ts for which <code>Predicate<T>::value</code> evaluates to \c true in-order as \ref yymp::typelist "typelist" `type`.
 *
 * Duplicates are retained.
 *
 * \tparam Predicate The template class predicate that passes types that meet the criteria.
 * \tparam Ts The sequence of types to filter.
 */
template< template< class... > class Predicate, class... Ts >
struct filter< Predicate, typelist< Ts... > > {
    template< class T >
    struct TPredicate {
        using type = typename std::conditional< Predicate< T >::value, std::true_type, std::false_type >::type;
    };

    /** \brief The \ref yymp::typelist "typelist" containing the types in \a Ts that pass \a Predicate, in the order they occur in \a Ts.
     *
     * Duplicates are retained.
     */
    using type = typename get_group<
        std::true_type,
        typename group_by<
            TPredicate,
            typelist< Ts... >
        >::type
    >::type;
};

/** \brief Retains all occurrences of \a T in the sequence \a Ts, as \ref yymp::typelist "typelist" `type`.
 *
 * Duplicates are retained.
 *
 * \tparam T The type to retain.
 * \tparam Ts The sequence of types to filter.
 */
template< class T, class... Ts >
struct filter_of< T, typelist< Ts... > > {    
    template< class U >
    using is_T = std::is_same< T, U >;
    
    /** \brief The \ref yymp::typelist "typelist" containing all occurrences of \a T in \a Ts.
     *
     * Duplicates are retained.
     */
    using type = typename filter<
        is_T,
        typelist< Ts... >
    >::type;
};

/** \brief Retains only the first occurrence of each type in \a Ts as \ref yymp::typelist "typelist" `type`.
 *
 * \tparam Ts The type sequence to filter for duplicates.
 */
template< class... Ts >
struct filter_duplicates< typelist< Ts... > > {
    template< class T >
    struct identity { using type = T; };

    /** \brief The \ref yymp::typelist "typelist" containing the distinct types of \a Ts, in the order they occur. */
    using type = typename transform<
        get_group_key,
        typename group_by< identity, typelist< Ts... > >::type
    >::type;
};

/////////////////////////
// C++14+ REQUIRED
#if __cpp_lib_integer_sequence >= 201304

template< template< class... > class Predicate, class... Ts >
struct indices_where< Predicate, typelist<Ts...> > {
    using TypeList = typelist<Ts...>;
    using ZippedTypeList = typename detail::make_zipped< TypeList >::type;
    
    template< class ZippedType >
    struct zipped_predicate {
        static constexpr bool value = Predicate< typename ZippedType::type >::value;
    };
    
    using type = typename detail::make_unzipped<
        typename filter<
            zipped_predicate,
            ZippedTypeList
        >::type
    >::type;
};

template< class T, class... Ts >
struct indices_of< T, typelist<Ts...> > {
    using TypeList = typelist<Ts...>;
    
    template< class U >
    using is_T = std::is_same< T, U >;
    
    using type = typename indices_where< is_T, TypeList >::type;
};

#endif 
//
/////////////////////////

}

#endif // YYMP__FILTER_HPP
