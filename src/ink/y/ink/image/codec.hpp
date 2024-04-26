//! \file

#ifndef Y_Ink_Image_Codec_Included
#define Y_Ink_Image_Codec_Included 1

#include "y/ink/types.hpp"
#include "y/ink/pixmap.hpp"
#include "y/ink/image/format/options.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Ink
    {


        //______________________________________________________________________
        //
        //
        //
        //! Load/Save image interface
        //
        //
        //______________________________________________________________________
        class Codec : public Object, public Counted
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Pixmap<RGBA>               Image;     //!< alias
            typedef Image::RowType             ImageRow;  //!< alias
            typedef ArkPtr<String,const Codec> Handle; //!< alias

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
            const String &       key()      const noexcept; //!< name

            //! wrapper for save
            void save(const Image         &image,
                      const char          *fileName,
                      const FormatOptions *options);

            //! wrapper for load
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
