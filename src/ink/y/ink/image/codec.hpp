//! \file

#ifndef Y_Ink_Image_Codec_Included
#define Y_Ink_Image_Codec_Included 1

#include "y/ink/pixmap.hpp"
#include "y/color/rgba.hpp"
#include "y/ink/image/format/options.hpp"

namespace Yttrium
{
    namespace Ink
    {
        typedef Color::RGBA<uint8_t> RGBA;

        class Codec : public Object, public Counted, public Identifiable
        {
        public:
            typedef Pixmap<RGBA>     Image;
            typedef Image::RowType   ImageRow;
            virtual ~Codec() noexcept;



        protected:
            template <typename UID> inline
            explicit Codec(const UID &uid) : name(uid) {}

        public:
            const String name;

            virtual const char * callSign() const noexcept;
            const String &       key()      const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Codec);
        };

    }
}

#endif
