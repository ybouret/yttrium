
//! \file

#ifndef Y_APK_Element_Transmogrify_Included
#define Y_APK_Element_Transmogrify_Included 1

#include "y/config/starting.hpp"
#include "y/check/static.hpp"

namespace Yttrium
{

    namespace APK
    {
        //______________________________________________________________________
        //
        //
        //! Transmogrify Action
        //
        //______________________________________________________________________
        enum TransmogrifyAction
        {
            Collect, //!< TargetSize >  SourceSize
            Nothing, //!< TargetSize == SourceSize
            Scatter  //!< TargetSize <  SourceSize
        };

        //______________________________________________________________________
        //
        //
        //
        //! Transmogrify Algorithms
        //
        //
        //______________________________________________________________________
        template <typename TARGET, typename SOURCE>
        struct Transmogrify
        {
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef void               (*Proc)(TARGET * & ,const SOURCE * & ); //!< alias
            static const unsigned        TargetSize = sizeof(TARGET);          //!< alias
            static const unsigned        SourceSize = sizeof(SOURCE);          //!< alias
            //! select action
            static const TransmogrifyAction Action    = (TargetSize>SourceSize) ? Collect : ( TargetSize<SourceSize ? Scatter : Nothing );
            typedef Int2Type<Action>        Choice; //!< alias

            //! action selection to convert source to target
            static inline void To(TARGET * & target, const SOURCE * &source) noexcept
            {
                static const Choice choice = {};
                Run(target,source,choice);
            }

        private:
            //! collect source items into larger target item
            static inline void Run(TARGET * & target, const SOURCE * &source, const Int2Type<Collect> &) noexcept
            {
                Y_STATIC_CHECK(TargetSize>SourceSize,BadSetup);
                static const size_t   Words = TargetSize/SourceSize; assert(Words>0);
                static const unsigned Shift = SourceSize * 8;
                TARGET   result = *(source++);
                unsigned bshift = 0;
                for(size_t i=1;i<Words;++i)
                {
                    TARGET src = *(source++);
                    result |= (src <<= (bshift += Shift));
                }
                *(target++) = result;
            }

            //! scatter source item into smaller target items
            static inline void Run(TARGET * & target, const SOURCE * &source, const Int2Type<Scatter> &) noexcept
            {
                Y_STATIC_CHECK(TargetSize<SourceSize,BadSetup);
                static const size_t   Words = SourceSize/TargetSize; assert(Words>0);
                static const unsigned Shift = TargetSize * 8;

                SOURCE value = *(source++);
                for(size_t i=0;i<Words;++i,value>>=Shift)
                    *(target++) = static_cast<TARGET>(value);
            }
        };

        //! helper to perform checks
        struct TransmogrifyProc
        {
            static void *Table[4][4]; //!< address of functions
        };


    }

}

#endif

