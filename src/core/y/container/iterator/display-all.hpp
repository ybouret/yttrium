
#ifndef Y_Container_Iterator_Display_Included
#define Y_Container_Iterator_Display_Included 1

#include "y/container/iterator/writable-contiguous.hpp"
#include <iostream>

namespace Yttrium
{

    namespace Iterating
    {
        //! helper to test iterators
        struct DisplayAll
        {
            //! display const iterators
            template <typename CLASS>
            static inline void ConstOf(const CLASS &cls)
            {
                std::cerr << "-- ConstIterator, Forward:";
                for(typename CLASS::ConstIterator it=cls.begin();it!=cls.end();++it)
                {
                    std::cerr << ' ' << *it;
                }
                std::cerr << std::endl;

                std::cerr << "-- ConstIterator, Reverse:";
                for(typename CLASS::ConstReverseIterator it=cls.rbegin();it!=cls.rend();++it)
                {
                    std::cerr << ' ' << *it;
                }
                std::cerr << std::endl;
            }

            //! display all iterators
            template <typename CLASS>
            static inline void Of(CLASS &cls)
            {
                std::cerr << "--      Iterator, Forward:";
                for(typename CLASS::Iterator it=cls.begin();it!=cls.end();++it)
                {
                    std::cerr << ' ' << *it;
                }
                std::cerr << std::endl;

                std::cerr << "--      Iterator, Reverse:";
                for(typename CLASS::ReverseIterator it=cls.rbegin();it!=cls.rend();++it)
                {
                    std::cerr << ' ' << *it;
                }
                std::cerr << std::endl;
                ConstOf(cls);
            }

        };
    }
}

#endif

