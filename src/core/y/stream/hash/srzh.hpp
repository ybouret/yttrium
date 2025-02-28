
//! \file

#ifndef Y_Stream_Hash_Serializable_Included
#define Y_Stream_Hash_Serializable_Included 1

#include "y/stream/serializable.hpp"
#include "y/hashing/function.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //!  Hasher of Serializable objects
    //
    //
    //__________________________________________________________________________
    class SerializableHasher
    {
    public:
        explicit SerializableHasher(Hashing::Function &) noexcept; //!< setup
        SerializableHasher(const SerializableHasher &)   noexcept; //!< duplicate
        virtual ~SerializableHasher()                    noexcept; //!< cleanup

        //! hash binary representation of object
        Hashing::Function & hash(const Serializable &obj) noexcept;

    private:
        Y_DISABLE_ASSIGN(SerializableHasher);
        Hashing::Function &hfcn;
    };

    //__________________________________________________________________________
    //
    //
    //
    //! Key Hasher to use Serializable as keys
    //
    //
    //__________________________________________________________________________
    template <typename HASHING_FUNCTION>
    class SerializableKeyHasher : public HASHING_FUNCTION, public SerializableHasher
    {
    public:
        //! setup
        inline explicit SerializableKeyHasher() noexcept : HASHING_FUNCTION(), SerializableHasher( static_cast<HASHING_FUNCTION&>(*this) ) {}

        //! cleanup
        inline virtual ~SerializableKeyHasher() noexcept {}

        //! return key from hashed object
        inline size_t operator()(const Serializable &obj) noexcept
        {
            size_t res = 0;
            hash(obj).get(&res,sizeof(res));
            return res;
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(SerializableKeyHasher);
    };


}

#endif
