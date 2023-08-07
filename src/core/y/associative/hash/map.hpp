
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
        //! base class for hashMap
        //
        //______________________________________________________________________
        class HashMap
        {
        public:
            static const char * const CallSign; //!< "HasMap"
            explicit HashMap() noexcept;        //!< setup
            virtual ~HashMap() noexcept;        //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(HashMap);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! HashMap
    //
    //
    //__________________________________________________________________________
    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = Hashing::ToHashKey<Hashing::FNV>
    >
    class HashMap : public HashLinked<KEY,T, Glossary<KEY,T>, HashMapNode<KEY,T>,KEY_HASHER,Core::HashMap>
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
        typedef HashMapNode<KEY,T> NodeType;                                           //!< alias
        typedef Glossary<KEY,T>    API_Type;                                           //!< alias
        typedef HashLinked<KEY,T,API_Type,NodeType,KEY_HASHER,Core::HashMap> SelfType; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit HashMap() : SelfType(0) { }                                    //!< setup with minimal memory
        inline explicit HashMap(const size_t n, const AsCapacity_ &) : SelfType(n) {}  //!< setup with capacity
        inline virtual ~HashMap() noexcept { }                                         //!< cleanup

        //! copy
        inline          HashMap(const HashMap &other) :
        Identifiable(), Collection(), SelfType(other) {}
        
        //! insert (key,value)
        inline virtual bool insert(ParamKey k, ParamType t)
        {
            return this->insert_(k,t);
        }
        


    private:
        Y_DISABLE_ASSIGN(HashMap);
    };

    



}

#endif
