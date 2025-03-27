
//! \file


#ifndef Y_Chemical_ShowList_Included
#define Y_Chemical_ShowList_Included 1

#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //! show list of indexed entities
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
                    if(pfx) os << pfx;
                    os << (**node).name;
                    if(sfx) os << sfx;
                    fmt.pad(os,**node);
                    os << " = ";
                    os << fcn( (**node)(arr,lvl) );
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

