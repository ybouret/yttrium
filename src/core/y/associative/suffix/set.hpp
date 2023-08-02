//! \file

#ifndef Y_Associative_Suffix_Set_Included
#define Y_Associative_Suffix_Set_Included 1

#include "y/associative/suffix/alloy.hpp"
#include "y/associative/suffix/set/node.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! base class for SuffixSet
        //
        //______________________________________________________________________
        class SuffixSet
        {
        public:
            static const char * const CallSign; //!< "SuffixSet"
            explicit SuffixSet() noexcept;      //!< setup
            virtual ~SuffixSet() noexcept;      //!< cleanup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(SuffixSet);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! Set of keyed objects
    //
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class SuffixSet :
    public SuffixAlloy<KEY,T, SuffixSetNode<KEY,T>, Core::SuffixSet>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef SuffixAlloy<KEY,T, SuffixSetNode<KEY,T>, Core::SuffixSet> BaseType; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit SuffixSet() noexcept : BaseType() {} //!< setup
        inline virtual ~SuffixSet() noexcept {}              //!< cleanup

        //! copy
        inline SuffixSet(const SuffixSet &other) :
        Identifiable(), Collection(),
        BaseType(other) {}

    private:
        Y_DISABLE_ASSIGN(SuffixSet);
    };

}

#endif

