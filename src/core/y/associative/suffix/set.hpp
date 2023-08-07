//! \file

#ifndef Y_Associative_Suffix_Set_Included
#define Y_Associative_Suffix_Set_Included 1

#include "y/associative/registry.hpp"
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
    public SuffixAlloy<KEY,T, SuffixSetNode<KEY,T>, Registry<KEY,T>, Core::SuffixSet>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef SuffixSetNode<KEY,T> NodeType;                          //!< alias
        typedef Registry<KEY,T>      KindType;                          //!< alias
        typedef Core::SuffixSet      BaseType;                          //!< alias
        typedef SuffixAlloy<KEY,T,NodeType,KindType,BaseType> SelfType; //!< alias
        Y_ARGS_DECL(T,Type);                                            //!< aliases
        Y_ARGS_DECL(KEY,Key);                                           //!< aliases
        
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit SuffixSet() : SelfType(0) {} //!< setup with minimal memory
        inline virtual ~SuffixSet() noexcept {}      //!< cleanup

        //! setup with extra memory
        inline explicit SuffixSet(const size_t n, const AsCapacity_ &) : SelfType(n) {}

        //! copy
        inline SuffixSet(const SuffixSet &other) :
        Identifiable(), Collection(),
        SelfType(other) {}

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual bool insert(ParamType t)
        {
            return  this->insert_(t);
        }

    private:
        Y_DISABLE_ASSIGN(SuffixSet);
    };

}

#endif

