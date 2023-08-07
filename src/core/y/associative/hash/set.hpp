
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
            static const char * const CallSign; //!< "SuffixMap
            explicit HashSet() noexcept;      //!< setup
            virtual ~HashSet() noexcept;      //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(HashSet);
        };
    }

    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = Hashing::ToHashKey<Hashing::FNV>
    >
    class HashSet : public HashLinked<KEY,T, Registry<KEY,T>, HashSetNode<KEY,T>,KEY_HASHER,Core::HashSet>
    {
    public:
        Y_ARGS_DECL(T,Type);
        Y_ARGS_DECL(KEY,Key);

        typedef HashSetNode<KEY,T> NodeType;
        typedef Registry<KEY,T>    API_Type;
        typedef HashLinked<KEY,T,API_Type,NodeType,KEY_HASHER,Core::HashSet> SelfType;

        inline explicit     HashSet() : SelfType(0) { }
        inline explicit     HashSet(const size_t n, const AsCapacity_ &) : SelfType(n) {}
        inline virtual     ~HashSet() noexcept { }
        inline              HashSet(const HashSet &other) : SelfType(other) {}

        inline virtual bool insert(ParamType t) { return this->insert_(t); }



    private:
        Y_DISABLE_ASSIGN(HashSet);
    };



}

#endif

