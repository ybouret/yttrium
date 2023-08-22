//! \file

#ifndef Y_IO_Serializable_Included
#define Y_IO_Serializable_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Core    { template <typename> class String;   }
    namespace Hashing { class                     Function; }

    class OutputStream;

    //__________________________________________________________________________
    //
    //
    //
    //! Serializable interface
    //
    //
    //__________________________________________________________________________
    class Serializable
    {
    public:
        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! save data, return written bytes
        //______________________________________________________________________
        virtual size_t serialize(OutputStream &) const = 0;

        //______________________________________________________________________
        //
        //! encode call sign, serialize
        //______________________________________________________________________
        template <typename T> inline
        size_t marshall(OutputStream &fp, const T &identifiable) const
        {
            size_t written = emitMessage(fp,identifiable.callSign());
            return written + serialize(fp);
        }
        
        //______________________________________________________________________
        //
        //! write to string
        //______________________________________________________________________
        Core::String<char> toBinary() const;

        //______________________________________________________________________
        //
        //! write to file
        //______________________________________________________________________
        size_t toBinary(const Core::String<char> &fileName, const bool append=false) const;
        size_t toBinary(const char *              fileName, const bool append=false) const; //!< alias


        //______________________________________________________________________
        //
        //! send to hashing function
        //______________________________________________________________________
        void runHash( Hashing::Function &) const noexcept;

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        virtual ~Serializable() noexcept; //!< cleanup
    protected:
        explicit Serializable() noexcept; //!< setup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Serializable);
        size_t emitMessage(OutputStream &,const char *) const;

    };
}

#endif

