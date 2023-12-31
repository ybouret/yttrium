//! \file

#ifndef Y_System_Endian_Included
#define Y_System_Endian_Included 1

#include "y/singleton.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Providing Endianness routine
    //
    //
    //__________________________________________________________________________
    class Endianness : public Singleton<Endianness>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const      CallSign;                                //!< "Endianness"
        static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 8; //!< LifeTime

        //! possible types
        enum Type
        {
            Unknown,   //!< ??
            Big,       //!< Big Endian
            Little,    //!< Little Endian
            BigWord,   //!< Middle-endian, Honeywell 316 style
            LittleWord //!< Middle-endian, PDP-11 style
        };

        
        static const char * Text(const Type) noexcept; //!< human readable
        static Type         Detect()         noexcept; //!< Run-Time test

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        uint8_t  hton(const uint8_t)  const noexcept; //!< host to net
        uint16_t hton(const uint16_t) const noexcept; //!< host to net
        uint32_t hton(const uint32_t) const noexcept; //!< host to net
        uint64_t hton(const uint64_t) const noexcept; //!< host to net


        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        const Type type; //!< system type

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Endianness);
        explicit Endianness();
        virtual ~Endianness() noexcept;
        friend class Singleton<Endianness>;

    };

}

#endif
