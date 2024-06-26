//! \file


#ifndef Y_Tow_API_Included
#define Y_Tow_API_Included 1

#include "y/config/starting.hpp"
#include "y/check/static.hpp"

namespace Yttrium
{


    // Transformation Of Words
    namespace TOW
    {
        //______________________________________________________________________
        //
        //
        //! transformation action depending one type sizes
        //
        //______________________________________________________________________
        enum ActionType
        {
            Collect, //!< TargetSize >  SourceSize
            RawCopy, //!< TargetSize == SourceSize
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
        struct API
        {
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned   TargetSize = sizeof(TARGET);             //!< alias
            static const unsigned   SourceSize = sizeof(SOURCE);             //!< alias
            static const bool       RunCollect = (TargetSize >  SourceSize); //!< alias
            static const bool       RunScatter = (TargetSize <  SourceSize); //!< alias
            static const bool       RunRawCopy = (TargetSize == SourceSize); //!< alias
            static const ActionType Action     = RunCollect ? Collect : ( RunScatter ? Scatter : RawCopy ); //!< action

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! action selection to convert ONE source to ONE target
            /**
             \param target pointer to target type, updated to next target
             \param source pointer to source type, updated to next source
             */
            static inline void To(TARGET * & target, const SOURCE * &source) noexcept
            {
                static const Int2Type<Action>  action = {};
                Run(target,source,action);
            }

            //! compute cycles to run
            static inline size_t Cycles(const size_t targetBlocks, const size_t sourceBlocks) noexcept
            {
                static const Int2Type<Action>  action = {};
                return Cycles(targetBlocks,sourceBlocks,action);
            }

            //__________________________________________________________________
            //
            //! collect source items into larger target item
            //__________________________________________________________________
            static inline void Run(TARGET * & target, const SOURCE * &source, const Int2Type<RawCopy> &) noexcept
            {
                Y_STATIC_CHECK(TargetSize==SourceSize,BadSetup);
                *(target++) = *(source++);
            }

            //__________________________________________________________________
            //
            //! collect source items into larger target item
            //__________________________________________________________________
            static inline void Run(TARGET * & target, const SOURCE * &source, const Int2Type<Collect> &) noexcept
            {
                Y_STATIC_CHECK(TargetSize>SourceSize,BadSetup);
                static const unsigned Words = TargetSize/SourceSize; assert(Words>0);
                static const unsigned SBits = SourceSize * 8;

                TARGET   result = *(source++);
                unsigned bshift = 0;
                for(unsigned i=1;i<Words;++i)
                {
                    TARGET src = *(source++);
                    result    |= (src <<= (bshift += SBits));
                }
                *(target++) = result;
            }

            //__________________________________________________________________
            //
            //! scatter source item into smaller target items
            //__________________________________________________________________
            static inline void Run(TARGET * & target, const SOURCE * &source, const Int2Type<Scatter> &) noexcept
            {
                Y_STATIC_CHECK(TargetSize<SourceSize,BadSetup);
                static const size_t   Words = SourceSize/TargetSize; assert(Words>0);
                static const unsigned Shift = TargetSize * 8;

                SOURCE value = *(source++);
                for(size_t i=0;i<Words;++i,value>>=Shift)
                    *(target++) = static_cast<TARGET>(value);
            }

        private:
            static inline size_t Cycles(const size_t targetBlocks, const size_t, const Int2Type<Collect> &) noexcept
            {
                return targetBlocks;
            }

            static inline size_t Cycles(const size_t targetBlocks, const size_t  , const Int2Type<RawCopy> &) noexcept
            {
                return targetBlocks;
            }

            static inline size_t Cycles(const size_t  , const size_t sourceBlocks, const Int2Type<Scatter> &) noexcept
            {
                return sourceBlocks;
            }

        };



        //! transmute source to target
        template <typename TARGET, typename SOURCE>
        inline void Transmute(TARGET *       target,
                              const SOURCE * source,
                              size_t         cycles) noexcept
        {
            typedef API<TARGET,SOURCE>             TheAPI;
            static const Int2Type<TheAPI::Action>  action = {};
            assert(Good(target,cycles));
            assert(Good(source,cycles));
            while(cycles-- > 0)
               TheAPI::Run(target,source,action);
        }

    };

}

#endif

