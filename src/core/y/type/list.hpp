//! \file
#ifndef Y_Type_List_Included
#define Y_Type_List_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //! 'N/A' class for typelists.
    //
    //__________________________________________________________________________
    class NullType
    {
    };

    //__________________________________________________________________________
    //
    //
    //! 'do nothing' class for typelists.
    //
    //__________________________________________________________________________
    struct EmptyType
    {
    };
    
    //__________________________________________________________________________
    //
    //
    //! basic type_list
    //
    //__________________________________________________________________________
    template <class T, class U>
    struct TypeList {
        typedef T Head; //!< head type
        typedef U Tail; //!< recursive tail type of null_type to stop
    };


#define TL1(T1)                TypeList< T1, NullType>            //!< TL1
#define TL2(T1,T2)             TypeList< T1, TL1(T2) >             //!< TL2
#define TL3(T1,T2,T3)          TypeList< T1, TL2(T2,T3) >          //!< TL3
#define TL4(T1,T2,T3,T4)       TypeList< T1, TL3(T2,T3,T4) >       //!< TL4
#define TL5(T1,T2,T3,T4,T5)    TypeList< T1, TL4(T2,T3,T4,T5) >    //!< TL5
#define TL6(T1,T2,T3,T4,T5,T6) TypeList< T1, TL5(T2,T3,T4,T5,T6) > //!< TL6


    namespace TL
    {

        //______________________________________________________________________
        //
        //
        // Length Of a TypeList
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! declaration
        //______________________________________________________________________
        template <class TList> struct LengthOf;

        //______________________________________________________________________
        //
        //! specialized length for NullType
        //______________________________________________________________________
        template <> struct LengthOf<NullType> {
            enum { Value = 0 };
        };

        //______________________________________________________________________
        //
        //! recursive length algorithm
        //______________________________________________________________________
        template <class T, class U>
        struct LengthOf< TypeList<T,U> > {
            enum { Value = 1 + LengthOf<U>::Value };
        };

        //______________________________________________________________________
        //
        //
        // TypeAt
        //
        //______________________________________________________________________

        //! declaration
        template <class TList, size_t index> struct TypeAt;

        //! specialized TypeAt<0> algorithm.
        template <class head, class tail> struct TypeAt< TypeList<head,tail>, 0> {
            typedef head Result; //!< initial case
        };

        //! recursive TypeAt algorithm.
        template <class head, class tail, size_t i> struct TypeAt< TypeList<head,tail>,i > {
            typedef typename TypeAt<tail,i-1>::Result Result; //!< recursive call
        };


        //______________________________________________________________________
        //
        //
        // SafeTypeAt
        //
        //______________________________________________________________________

        //! declaration
        template <class TList, size_t index, typename default_type = NullType>
        struct SafeTypeAt {
            typedef default_type Result; //!< when bad index
        };

        //! specialized SafeTypeAt algorithm.
        template <class head, class tail, typename default_type>
        struct SafeTypeAt< TypeList<head,tail>, 0, default_type> {
            typedef head Result; //!< initial case
        };

        //! recursive SafeTypeAt algorithm.
        template <class head, class tail, size_t i, typename default_type>
        struct SafeTypeAt< TypeList<head, tail>, i, default_type> {
            typedef typename
            SafeTypeAt< tail, i-1, default_type>::Result Result; //!< recursive case
        };


        //______________________________________________________________________
        //
        //
        // IndexOf
        //
        //______________________________________________________________________

        //! default index_of algorithm.
        template <class TList, class T> class IndexOf;

        //! specialized index_of algorithm (not found).
        template <class T>
        class IndexOf<NullType,T> {
        public:
            enum {
                Value = -1 //!< when not in type_list.
            };
        };

        //! specialized index_of algorithm (at tail).
        template <class T, class tail>
        class  IndexOf< TypeList<T, tail>, T>
        {
        public:
            enum { Value = 0 };
        };

        //! recursive index_of algorithm.
        template <class head, class tail, class T>
        class IndexOf<TypeList<head, tail>, T> {
        private:
            enum { temp = IndexOf<tail, T>::Value };
        public:
            enum { Value = (temp == -1 ? -1 : 1 + temp) };
        };

        //======================================================================
        // append_to
        //======================================================================
        //! default append_to
        template <class TList,class T> struct AppendTo;

        //! special null_type to null_type
        template <> struct AppendTo<NullType,NullType> {
            typedef NullType result; //!< is null_type
        };

        //! special append T to null_type
        template <class T> struct AppendTo<NullType,T> {
            typedef TL1(T) result; //!< same list
        };

        //! special append type_list to null_type
        template <class head,class tail> struct AppendTo<NullType, TypeList<head,tail> >
        {
            typedef TypeList<head,tail> result; //!< compound list
        };

        //! generic valid case
        template <class head,class tail,class T> struct AppendTo< TypeList<head,tail>, T>
        {
            typedef TypeList< head, typename AppendTo<tail,T>::result> result; //!< compound list
        };

    } // tl
}

#endif

