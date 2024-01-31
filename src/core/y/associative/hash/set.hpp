 
//! \file


#ifndef Y_Associative_Hash_Set_Included
#define Y_Associative_Hash_Set_Included 1

#include "y/associative/registry.hpp"
#include "y/associative/hash/linked.hpp"
#include "y/associative/hash/set/node.hpp"
#include "y/hashing/fnv.hpp"
#include "y/hashing/to/hash-key.hpp"
#include "y/type/capacity.hpp"

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
        class HashSet
        {
        public:
            static const char * const CallSign; //!< "HasSet"
            explicit HashSet() noexcept;        //!< setup
            virtual ~HashSet() noexcept;        //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(HashSet);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! HashSet
    //
    //
    //__________________________________________________________________________
    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = Hashing::ToHashKey<Hashing::FNV>
    >
    class HashSet : public HashLinked<KEY,T,Registry<KEY,T>,HashSetNode<KEY,T>,KEY_HASHER,Core::HashSet>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);                                                           //!< aliases
        Y_ARGS_DECL(KEY,Key);                                                          //!< aliases
        typedef HashSetNode<KEY,T> NodeType;                                           //!< alias
        typedef Registry<KEY,T>    API_Type;                                           //!< alias
        typedef HashLinked<KEY,T,API_Type,NodeType,KEY_HASHER,Core::HashSet> SelfType; //!< alias

        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        inline explicit     HashSet() : SelfType(0) { }                                    //!< setup with minimal memory
        inline explicit     HashSet(const size_t n, const AsCapacity_ &) : SelfType(n) {}  //!< setup with capacity
        inline virtual     ~HashSet() noexcept { }                                         //!< cleanup

        //! copy
        inline HashSet(const HashSet &other) :
        Identifiable(), Collection(), SelfType(other) {}

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! insert type using type.key()
        inline virtual bool insert(ParamType t) { return this->insert_(t); }



    private:
        Y_DISABLE_ASSIGN(HashSet);
    };



}

#endif

