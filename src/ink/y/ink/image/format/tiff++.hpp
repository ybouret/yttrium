
//! \file

#ifndef Y_Ink_Format_TIFFxx_Included
#define Y_Ink_Format_TIFFxx_Included 1

#include "y/ink/pixmap.hpp"
#include "y/ink/types.hpp"
#include "y/string.hpp"

namespace Yttrium
{
    namespace Ink
    {

        //__________________________________________________________________
        //
        //
        //! wrapper for a TIFF
        //
        //__________________________________________________________________
        class tiffxx
        {
        public:
            virtual ~tiffxx() noexcept; //!< cleanup

        protected:
            explicit tiffxx(const String &, const char *mode); //!< TIFFOpen
            void *handle; //!< TIFF

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tiffxx);
        };

        //__________________________________________________________________
        //
        //
        //! input TIFF
        //
        //__________________________________________________________________
        class itiff: public tiffxx
        {
        public:
            explicit itiff(const String &filename); //!< setup
            virtual ~itiff() noexcept;               //!< cleanup

            unit_t width();                       //!< image width
            unit_t height();                      //!< image height
            size_t count_directories();           //!< count directories
            bool   load(Pixmap<RGBA> &img);       //!< img.w = width, img.h = height
            void   set_directory(const size_t n); //!< set directory


            static unit_t width_of(const String &filename);        //!< helper to get width
            static unit_t height_of(const String &filename);       //!< helper to get height
            static size_t directories_of(const String &filename);  //!< helper to get directories

            static unit_t width_of(const char *filename);        //!< wrapper
            static unit_t height_of(const char *filename);       //!< wrapper
            static size_t directories_of(const char *filename);  //!< wrapper

        private:
            Y_DISABLE_COPY_AND_ASSIGN(itiff);
        };

        //__________________________________________________________________
        //
        //
        //! output TIFF
        //
        //__________________________________________________________________
        class otiff: public tiffxx
        {
        public:
            explicit otiff(const String &filename); //!< setup
            virtual ~otiff() noexcept;               //!< cleanup


        private:
            Y_DISABLE_COPY_AND_ASSIGN(otiff);
        };

        
    }
}

#endif
