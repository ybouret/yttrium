//! \file

#ifndef Y_Ink_Format_PNG_Included
#define Y_Ink_Format_PNG_Included 1

#include "y/ink/image/format.hpp"

namespace Yttrium
{
    namespace Ink
    {

        //______________________________________________________________________
        //
        //
        //
        //! Raw RGB Bitmap
        //
        //
        //______________________________________________________________________
        class FormatPNG : public Format
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "PNG"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit FormatPNG();           //!< setup
            virtual ~FormatPNG() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! saving
            virtual void  save(const Image         &image,
                               const String        &fileName,
                               const FormatOptions *options) const;

            //! loading
            virtual Image load(const String        &fileName,
                               const FormatOptions *options) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormatPNG);
        };
    }

}

#endif

