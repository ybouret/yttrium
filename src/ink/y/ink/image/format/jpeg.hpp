//! \file

#ifndef Y_Ink_Format_JPEG_Included
#define Y_Ink_Format_JPEG_Included 1

#include "y/ink/image/format.hpp"

namespace Yttrium
{
    namespace Ink
    {


        //______________________________________________________________________
        //
        //
        //
        //! JPEG Bitmap
        //
        //
        //______________________________________________________________________
        class FormatJPEG : public Format
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "JPEG"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit FormatJPEG();           //!< setup
            virtual ~FormatJPEG() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! saving
            virtual void  save(const Image  &image,
                               const String &fileName,
                               const Options *options) const;

            //! loading
            virtual Image load(const String &fileName,
                               const Options *options) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormatJPEG);
        };
    }

}

#endif

