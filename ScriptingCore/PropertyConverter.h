/**********************************************************\
Original Author: Georg Fritzsche
Generated on: 2009-11-07
License:      Eclipse Public License - Version 1.0
              http://www.eclipse.org/legal/epl-v10.html

Copyright 2009 Georg Fritzsche, Firebreath development team
\**********************************************************/


#if !defined(PROPERTY_CONVERTER_H)
#define PROPERTY_CONVERTER_H

#if defined(_MSC_VER)
#  pragma once
#endif

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "boost/mpl/equal.hpp"
#include "APITypes.h"
#include "ConverterUtils.h"

namespace FB
{
	namespace detail
	{
		template<class C, typename T>
		FB::variant getter(C* instance, T (C::*getter)())
		{
			return (instance->*getter)();
		}

		template<class C, typename T>
		void setter(C* instance, void (C::*setter)(T), const FB::variant& v)
		{
			typedef typename FB::detail::plain_type<T>::type Ty;
			(instance->*setter)(v.convert_cast<Ty>());
		}

		template<typename T>
		FB::variant dummySetter(const FB::variant&) 
		{
			return FB::variant();
		}
	}

	// make read/write property functor

	template<class C, typename T1, typename T2>
	PropertyFunctors 
	make_property(C* instance, T1 (C::*getter)(), void (C::*setter)(T2))
	{
		typedef typename FB::detail::plain_type<T1>::type _T1;
		typedef typename FB::detail::plain_type<T2>::type _T2;
		enum { equal_types = boost::mpl::equal<_T1, _T2>::type::value };
		BOOST_MPL_ASSERT_MSG(equal_types,
							 PROPERTY_SETTER_AND_GETTER_TYPES_HAVE_TO_BE_EQUAL,
							 (_T1, _T2));

		return PropertyFunctors(
			boost::bind(FB::detail::getter<C, T1>, instance, getter),
			boost::bind(FB::detail::setter<C, T2>, instance, setter, _1));
	}

	// make read-only property

	template<class C, typename T>
	PropertyFunctors
	make_property(C* instance, T (C::*getter)())
	{
		typedef typename FB::detail::plain_type<T>::type _T;

		return PropertyFunctors(
			boost::bind(FB::detail::getter<C, T>, instance, getter),
			boost::bind(FB::detail::dummySetter<_T>, _1));
	}
}

#endif // #if !defined(PROPERTY_CONVERTER_H)