
//! \file


#ifndef Y_Chemical_ShowList_Included
#define Y_Chemical_ShowList_Included 1

#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //! transfering array contents
        /**
         \param list        list of indexed entities
         \param target      target array
         \param targetLevel target level
         \param source      source array
         \param sourceLevel source level
         */
        template <typename LIST, typename TARGET, typename SOURCE> inline
        TARGET & TransferList(const LIST &list,
                              TARGET &target, const Level targetLevel,
                              SOURCE &source, const Level sourceLevel)
        {
            for(const typename LIST::NodeType *node=list.head;node;node=node->next)
            {
                const Indexed & _ = **node;
                _(target,targetLevel) = _(source,sourceLevel);
            }
            return target;
        }

        //! apply proc(data) for each indexed species in listg
        template <typename LIST, typename PROC, typename DATA> inline
        void ForEachIn(const LIST &list, PROC &proc, DATA &data, const Level L)
        {
            for(const typename LIST::NodeType *node=list.head;node;node=node->next)
            {
                proc( (**node)(data,L) );
            }
        }



        namespace {

            //! helper to zset
            struct _Generic0 {
                //! set argument to 0
                template <typename T> inline
                void operator()(T &_) const { _=0; }
            };
        }

        //! zset data for each species in list
        template <typename LIST, typename DATA> inline
        void ZeroList(const LIST &list, DATA &data, const Level L) {
            _Generic0 ld0;
            ForEachIn(list,ld0,data,L);
        }


        //! show list of indexed entities with transformation
        template <typename LIST, typename ARRAY, typename PROC> inline
        std::ostream & ShowList(std::ostream   &   os,
                                const LIST     &   lst,
                                const Assembly &   fmt,
                                const Level        lvl,
                                const char * const pfx,
                                ARRAY          &   arr,
                                const char * const sfx,
                                PROC              &fcn)
        {
            os << '{';
            if(lst.size>0)
            {
                os << std::endl;
                for(const typename LIST::NodeType *node=lst.head;node;node=node->next)
                {
                    const Indexed & _ = **node;
                    if(pfx) os << pfx;
                    os << _.name;
                    if(sfx) os << sfx;
                    fmt.pad(os,_);
                    os << " = ";
                    os << fcn( _(arr,lvl) );
                    os << std::endl;
                }
            }
            os << '}';
            return os;
        }

        namespace
        {
            struct _GenericId {
                template <typename T> inline
                T & operator()(T &_) noexcept { return _; }
            };
        }

        //! show list of indexed entities
        template <typename LIST, typename ARRAY> inline
        std::ostream & ShowList(std::ostream   &   os,
                                const LIST     &   lst,
                                const Assembly &   fmt,
                                const Level        lvl,
                                const char * const pfx,
                                ARRAY          &   arr,
                                const char * const sfx)
        {
            _GenericId id;
            return ShowList<LIST,ARRAY,_GenericId>(os,lst,fmt,lvl,pfx,arr,sfx,id);
        }

    }

}

#endif

