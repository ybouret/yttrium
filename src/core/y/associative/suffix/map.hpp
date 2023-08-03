
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
    class SuffixMap : public SuffixAlloy<KEY,T, SuffixMapNode<KEY,T>, Glossary<KEY,T>, Core::SuffixMap>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef SuffixMapNode<KEY,T> NodeType;                          //!< alias
        typedef Glossary<KEY,T>      KindType;                          //!< alias
        typedef Core::SuffixMap      BaseType;                          //!< alias
        typedef SuffixAlloy<KEY,T,NodeType,KindType,BaseType> SelfType; //!< alias
        Y_ARGS_DECL(T,Type);                                            //!< aliases
        Y_ARGS_DECL(KEY,Key);                                           //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit SuffixMap() noexcept : SelfType() {} //!< setup
        inline virtual ~SuffixMap() noexcept {}              //!< cleanup

        //! copy
        inline SuffixMap(const SuffixMap &other) :
        Identifiable(), Collection(),
        SelfType(other)         {}

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________

        //! insert pair into tree
        virtual bool insert(ParamKey k, ParamType t)
        { return  this->insert_(k,t); }

    private:
        Y_DISABLE_ASSIGN(SuffixMap);
    };

}

#endif

