

#include "y/ink/image/format/jpeg.hpp"
#include "y/jpeg/jpeglib.h"

#include <cstring>
#include <setjmp.h>

namespace Yttrium
{
    namespace Ink
    {
        FormatJPEG::  FormatJPEG() : Format(CallSign,"(jpe?g)&") {}
        FormatJPEG:: ~FormatJPEG() noexcept {}

        const char * const FormatJPEG:: CallSign = "JPEG";

    }
}

#include "y/stream/libc/output.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Ink
    {



        /*
         * ERROR HANDLING:
         *
         * The JPEG library's standard error handler (jerror.c) is divided into
         * several "methods" which you can override individually.  This lets you
         * adjust the behavior without duplicating a lot of code, which you might
         * have to update with each future release.
         *
         * Our example here shows how to override the "error_exit" method so that
         * control is returned to the library's caller when a fatal error occurs,
         * rather than calling exit() as the standard error_exit method does.
         *
         * We use C's setjmp/longjmp facility to return control.  This means that the
         * routine which calls the JPEG library must first execute a setjmp() call to
         * establish the return point.  We want the replacement error_exit to do a
         * longjmp().  But we need to make the setjmp buffer accessible to the
         * error_exit routine.  To do this, we make a private extension of the
         * standard JPEG error handler object.  (If we were using C++, we'd say we
         * were making a subclass of the regular error handler.)
         *
         * Here's the extended error handler struct:
         */

        struct my_error_mgr {
            struct jpeg_error_mgr pub;    /* "public" fields */

            jmp_buf setjmp_buffer;    /* for return to caller */
        };

        typedef struct my_error_mgr * my_error_ptr;

        /*
         * Here's the routine that will replace the standard error_exit method:
         */

        METHODDEF(void)
        my_error_exit (j_common_ptr cinfo)
        {
            /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
            my_error_ptr myerr = (my_error_ptr) cinfo->err;

            /* Always display the message. */
            /* We could postpone this until after returning, if we chose. */
            (*cinfo->err->output_message) (cinfo);

            /* Return control to the setjmp point */
            longjmp(myerr->setjmp_buffer, 1);
        }


        typedef CxxArray<unsigned char,Memory::Dyadic> RGB_Line;

        class JPEG_RGB_Line : public RGB_Line
        {
        public:
            inline explicit JPEG_RGB_Line(const size_t width) : RGB_Line(3*width) {}
            inline virtual ~JPEG_RGB_Line() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(JPEG_RGB_Line);
        };


        static inline int getQuality(const FormatOptions *opt)
        {
            static const char quality_info[] = "quality";

            // options
            int           quality = 75;
            const String *ps = FormatOptions::Query(opt,quality_info);
            if(ps)
            {
                //quality = clamp<int>(10,ios::ascii::convert::to<int>( *ps,quality_info),100);
            }
            return quality;
        }

        void FormatJPEG:: save(const Image         &img,
                               const String        &fileName,
                               const FormatOptions *options) const
        {


            OutputFile      fp(fileName);
            JPEG_RGB_Line   line(img.w);

            /* This struct contains the JPEG compression parameters and pointers to
             * working space (which is allocated as needed by the JPEG library).
             * It is possible to have several such structures, representing multiple
             * compression/decompression processes, in existence at once.  We refer
             * to any one struct (and its associated working data) as a "JPEG object".
             */
            struct jpeg_compress_struct cinfo;
            /* This struct represents a JPEG error handler.  It is declared separately
             * because applications often want to supply a specialized error handler
             * (see the second half of this file for an example).  But here we just
             * take the easy way out and use the standard error handler, which will
             * print a message on stderr and call exit() if compression fails.
             * Note that this struct must live as long as the main JPEG parameter
             * struct, to avoid dangling-pointer problems.
             */
            struct my_error_mgr jerr;
            
            /* More stuff */
            JSAMPROW  row_pointer[1] = { &line[1] };    /* pointer to JSAMPLE row[s] */



            /* Step 1: allocate and initialize JPEG compression object */

            /* We have to set up the error handler first, in case the initialization
             * step fails.  (Unlikely, but it could happen if you are out of memory.)
             * This routine fills in the contents of struct jerr, and returns jerr's
             * address which we place into the link field in cinfo.
             */
            cinfo.err           = jpeg_std_error(&jerr.pub);
            jerr.pub.error_exit = my_error_exit;


            /* Establish the setjmp return context for my_error_exit to use. */
            if (setjmp(jerr.setjmp_buffer)) {
                /* If we get here, the JPEG code has signaled an error.
                 * We need to clean up the JPEG object, close the input file, and return.
                 */
                jpeg_destroy_compress(&cinfo);
                throw Specific::Exception(CallSign,"save error");
            }

            /* Now we can initialize the JPEG compression object. */
            jpeg_create_compress(&cinfo);

            /* Step 2: specify data destination (eg, a file) */
            /* Note: steps 2 and 3 can be done in either order. */

            /* Here we use the library-supplied code to send compressed data to a
             * stdio stream.  You can also write your own code to do something else.
             * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
             * requires it in order to write binary files.
             */
            jpeg_stdio_dest(&cinfo, (FILE *)fp.handle);

            /* Step 3: set parameters for compression */

            /* First we supply a description of the input image.
             * Four fields of the cinfo struct must be filled in:
             */
            cinfo.image_width      = static_cast<JDIMENSION>(img.w);     /* image width and height, in pixels */
            cinfo.image_height     = static_cast<JDIMENSION>(img.h);
            cinfo.input_components = 3;         /* # of color components per pixel */
            cinfo.in_color_space   = JCS_RGB;   /* colorspace of input image */

            /* Now use the library's routine to set default compression parameters.
             * (You must set at least cinfo.in_color_space before calling this,
             * since the defaults depend on the source color space.)
             */
            jpeg_set_defaults(&cinfo);
            /* Now you can set any non-default parameters you wish to.
             * Here we just illustrate the use of quality (quantization table) scaling:
             */
            jpeg_set_quality(&cinfo, getQuality(options), TRUE /* limit to baseline-JPEG values */);

            /* Step 4: Start compressor */

            /* TRUE ensures that we will write a complete interchange-JPEG file.
             * Pass TRUE unless you are very sure of what you're doing.
             */
            jpeg_start_compress(&cinfo, TRUE);

            /* Step 5: while (scan lines remain to be written) */
            /*           jpeg_write_scanlines(...); */

            /* Here we use the library's state variable cinfo.next_scanline as the
             * loop counter, so that we don't have to keep track ourselves.
             * To keep things simple, we pass one scanline per call; you can pass
             * more if you wish, though.
             */
            //row_stride = image_width * 3;    /* JSAMPLEs per row in image_buffer */

            while (cinfo.next_scanline < cinfo.image_height) {
                /* jpeg_write_scanlines expects an array of pointers to scanlines.
                 * Here the array is only one element long, but you could pass
                 * more than one scanline at a time if that's more convenient.
                 */
                const ImageRow     &src = img(cinfo.next_scanline);
                unsigned char      *ptr = &line[1];
                for(unsigned i=0;i<cinfo.image_width;++i)
                {
                    const RGBA &c = src(i);
                    *(ptr++) = c.r;
                    *(ptr++) = c.g;
                    *(ptr++) = c.b;
                }
                //row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
                (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }

            /* Step 6: Finish compression */

            jpeg_finish_compress(&cinfo);
            /* After finish_compress, we can close the output file. */

            /* Step 7: release JPEG compression object */

            /* This is an important step since it will release a good deal of memory. */
            jpeg_destroy_compress(&cinfo);

            /* And we're done! */

        }
    }

}

#include "y/stream/libc/input.hpp"


namespace Yttrium
{
    namespace Ink
    {
        
        Codec::Image FormatJPEG:: load(const String        &fileName,
                                       const FormatOptions *) const
        {
            return Image(1,1);
        }

    }

}


