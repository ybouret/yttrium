
//! \file

#ifndef Y_Light_Array_Included
#define Y_Light_Array_Included 1

#include "y/container/writable.hpp"
#include "y/container/iterator/writable-contiguous.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Core
    {

        //______________________________________________________________________
        //
        //
        //! Light Array base class
        //
        //______________________________________________________________________
        class LightArray
        {
        public:
            explicit LightArray(const size_t) noexcept; //!< setup room
            virtual ~LightArray()             noexcept; //!< cleanup

            const size_t              room;     //!< number of accessible objects
            static const char * const CallSign; //!< "LightArray"

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LightArray);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! Light Array on C-style tableau
    //
    //
    //__________________________________________________________________________
    template <typename T> class LightArray:
    public Core::LightArray,
    public Writable<T>,
    public WritableContiguous<T>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type); //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! cleanup
        inline virtual ~LightArray() noexcept
        {
            Coerce(data)     = 0;
            Coerce(item)     = 0;
            Coerce(lastData) = 0;
            Coerce(lastItem) = 0;

        }

        //! setup boundaries
        inline explicit LightArray(T *entry, const size_t count) noexcept :
        Core::LightArray(count),
        data( Memory::OutOfReach::Cast<MutableType>( (void *)entry ) ),
        item( data-1 ),
        lastData(data+room),
        lastItem(item+room)
        {
            assert(Good(entry,count));
        }


        //! copy boudnaries
        inline LightArray(const LightArray &other) noexcept :
        Core::LightArray(other.room),
        data(other.data),
        item(other.item),
        lastData(other.lastData),
        lastItem(other.lastItem)
        {
        }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual size_t       size()     const noexcept { return room;     } //!< accessible objecys
        virtual const char * callSign() const noexcept { return CallSign; } //!< CallSign

        //! access
        inline Type & operator[](const size_t indx) noexcept
        { assert(indx>=1); assert(indx<=size()); return item[indx]; }

        //! access, const
        inline ConstType & operator[](const size_t indx) const noexcept
        { assert(indx>=1); assert(indx<=size()); return item[indx]; }


    protected:
        MutableType * const data;     //!< [0..room-1]
        MutableType * const item;     //!< [1..room]
        MutableType * const lastData; //!< data+room
        MutableType * const lastItem; //!< item+room

    private:
        Y_DISABLE_ASSIGN(LightArray);


        virtual ConstType * getBaseForward() const noexcept { return data;     }
        virtual ConstType * getLastForward() const noexcept { return lastData; }
        virtual ConstType * getBaseReverse() const noexcept { return lastItem; }
        virtual ConstType * getLastReverse() const noexcept { return item;     }

    };

}

#endif
