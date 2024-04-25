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

        typedef Color::RGBA<uint8_t> RGBA; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Load/Save image interface
        //
        //
        //______________________________________________________________________
        class Codec : public Object, public Counted, public Identifiable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Pixmap<RGBA>     Image;     //!< alias
            typedef Image::RowType   ImageRow;  //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! cleanup
            virtual ~Codec() noexcept;

        protected:

            //! setup name of the Codec
            template <typename UID> inline
            explicit Codec(const UID &uid) : name(uid) {}

        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign() const noexcept; //! [Identifiable] name.c_str()
            const String &       key()      const noexcept; //!< name
            void save(const Image         &image,
                      const char          *fileName,
                      const FormatOptions *options);

            Image load(const char          *fileName,
                       const FormatOptions *options) const;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! save image according to format+options
            virtual void save(const Image         &image,
                              const String        &fileName,
                              const FormatOptions *options) const = 0;

            //! load image according to format+options
            virtual Image load(const String        &fileName,
                               const FormatOptions *options) const = 0;



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const String name; //!< identifier

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Codec);
        };

    }
}

#endif
