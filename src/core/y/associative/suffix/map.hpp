
//! \file


#ifndef Y_Associative_Suffix_Map_Included
#define Y_Associative_Suffix_Map_Included 1

#include "y/associative/glossary.hpp"
#include "y/associative/suffix/alloy.hpp"
#include "y/associative/suffix/map/node.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! base class for SuffixMap
        //
        //______________________________________________________________________
        class SuffixMap
        {
        public:
            static const char * const CallSign; //!< "SuffixMap
            explicit SuffixMap() noexcept;      //!< setup
            virtual ~SuffixMap() noexcept;      //!< cleanup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(SuffixMap);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! Map of key/objects
    //
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class SuffixMap :
    public SuffixAlloy<KEY,T, SuffixMapNode<KEY,T>, Glossary, Core::SuffixMap>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef SuffixAlloy<KEY,T, SuffixMapNode<KEY,T>, Glossary, Core::SuffixMap> BaseType; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit SuffixMap() noexcept : BaseType() {} //!< setup
        inline virtual ~SuffixMap() noexcept {}              //!< cleanup

        //! copy
        inline SuffixMap(const SuffixMap &other) :
        Identifiable(), Collection(),
        BaseType(other)         {}

    private:
        Y_DISABLE_ASSIGN(SuffixMap);
    };

}

#endif

