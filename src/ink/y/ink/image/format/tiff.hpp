

//! \file

#ifndef Y_Ink_Format_TIFF_Included
#define Y_Ink_Format_TIFF_Included 1

#include "y/ink/image/format.hpp"

namespace Yttrium
{
    namespace Ink
    {
        
        //______________________________________________________________________
        //
        //
        //
        //! TIFF
        //
        //
        //______________________________________________________________________
        class FormatTIFF : public Format
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "TIFF"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit FormatTIFF();           //!< setup
            virtual ~FormatTIFF() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! saving
            virtual void  save(const Image   &image,
                               const String  &fileName,
                               const Options *options) const;

            //! loading
            virtual Image load(const String  &fileName,
                               const Options *options) const;

            //! TIFF count_directories
            static size_t CountDirectories(const String &filename);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormatTIFF);
        };
    }

}

#endif

