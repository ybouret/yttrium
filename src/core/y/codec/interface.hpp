
//! \file

#ifndef Y_Codec_Included
#define Y_Codec_Included 1

#include "y/type/identifiable.hpp"
#include "y/stream/input.hpp"
#include "y/stream/output.hpp"

namespace Yttrium
{

    //! base class for Codecs
    class Codec : public InputStream, public OutputStream
    {
    protected:
        explicit Codec() noexcept;
        
    public:
        virtual ~Codec() noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Codec);
    };

}

#endif

