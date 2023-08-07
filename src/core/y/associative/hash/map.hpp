
//! \file


#ifndef Y_Associative_Hash_Map_Included
#define Y_Associative_Hash_Map_Included 1

#include "y/associative/glossary.hpp"
#include "y/associative/hash/linked.hpp"
#include "y/associative/hash/map/node.hpp"
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
        class HashMap
        {
        public:
            static const char * const CallSign; //!< "SuffixMap
            explicit HashMap() noexcept;      //!< setup
            virtual ~HashMap() noexcept;      //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(HashMap);
        };
    }

    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = Hashing::ToHashKey<Hashing::FNV>
    >
    class HashMap : public HashLinked<KEY,T, Glossary<KEY,T>, HashMapNode<KEY,T>,KEY_HASHER,Core::HashMap>
    {
    public:
        Y_ARGS_DECL(T,Type);
        Y_ARGS_DECL(KEY,Key);

        typedef HashMapNode<KEY,T> NodeType;
        typedef Glossary<KEY,T>    API_Type;
        typedef HashLinked<KEY,T,API_Type,NodeType,KEY_HASHER,Core::HashMap> SelfType;

        inline explicit HashMap() : SelfType(0) { }
        inline explicit HashMap(const size_t n, const AsCapacity_ &) : SelfType(n) {}
        inline          HashMap(const HashMap &other) : SelfType(other) {}
        inline virtual ~HashMap() noexcept { }
        

        inline virtual bool insert(ParamKey k, ParamType t)
        {
            return this->insert_(k,t);
        }
        


    private:
        Y_DISABLE_ASSIGN(HashMap);
    };

    



}

#endif
