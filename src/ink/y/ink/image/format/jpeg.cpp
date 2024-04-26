

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
#include "y/text/ascii/convert.hpp"

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

            int           quality = 75;
            const String *ps = FormatOptions::Query(opt,quality_info);
            if(ps)
            {
                quality = Clamp<int>(10,ASCII::Convert::To<int>( *ps,quality_info),100);
                //std::cerr << "[quality=" << quality << "]" << std::endl;
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
        // wrapper for JSAMPARR
        class JPEG_Buffer
        {
        public:

            static inline JSAMPLE * acquire(size_t &n, size_t &b)
            {
                static Memory::Allocator &mgr = Memory::Dyadic::Instance();
                return mgr.allocate<JSAMPLE>(n,b);
            }

            inline explicit JPEG_Buffer(const unsigned W, const unsigned D) :
            width(W),
            depth(D),
            stride(depth*width),
            items(stride),
            bytes(0),
            //samples( memory_allocator::instance().allocate<JSAMPLE>(items,bytes) ),
            samples( acquire(items,bytes) ),
            buffer(),
            jsbuff(buffer)
            {
                buffer[0] = samples;
            }

            inline virtual ~JPEG_Buffer() noexcept
            {
                static Memory::Allocator &mgr = Memory::Dyadic::Location();
                mgr.withdraw(samples,items,bytes);
                items = 0;
                buffer[0] = NULL;
            }

            inline JSAMPLE ** & operator*() noexcept { return jsbuff; }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(JPEG_Buffer);
            const unsigned width;
            const unsigned depth;
            const unsigned stride;
            size_t         items;
            size_t         bytes;
            JSAMPLE       *samples;
            JSAMPLE       *buffer[1];
            JSAMPLE      **jsbuff;
        };



        static inline
        void JPEG_Row_Convert1(PixRow<RGBA>   &row,
                               const JSAMPLE *samples,
                               const unit_t   width) noexcept
        {
            assert(samples);
            assert(row.w() == width);
            for(unit_t i=0;i<width;++i)
            {
                const uint8_t c = *(samples++);
                row[i] = RGBA(c,c,c,0xff);
            }
        }

        static inline
        void JPEG_Row_Convert3(PixRow<RGBA>   &row,
                               const JSAMPLE *samples,
                               const unit_t   width) noexcept
        {
            assert(samples);
            assert(row.w() == width);
            for(unit_t i=0;i<width;++i)
            {
                const uint8_t r = *(samples++);
                const uint8_t g = *(samples++);
                const uint8_t b = *(samples++);
                row[i] = RGBA(r,g,b,0xff);
            }
        }

        typedef void (*JPEG_Row_Convert)(PixRow<RGBA>  &,
                                         const JSAMPLE *,
                                         const unit_t);

        Codec::Image FormatJPEG:: load(const String        &fileName,
                                       const FormatOptions *) const
        {
            /* This struct contains the JPEG decompression parameters and pointers to
             * working space (which is allocated as needed by the JPEG library).
             */
            struct jpeg_decompress_struct cinfo;

            /* We use our private extension JPEG error handler.
             * Note that this struct must live as long as the main JPEG parameter
             * struct, to avoid dangling-pointer problems.
             */
            struct my_error_mgr jerr;


            /* In this example we want to open the input file before doing anything else,
             * so that the setjmp() error recovery below can assume the file is open.
             * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
             * requires it in order to read binary files.
             */
            InputFile fp(fileName);


            /* Step 1: allocate and initialize JPEG decompression object */

            /* We set up the normal JPEG error routines, then override error_exit. */
            cinfo.err = jpeg_std_error(&jerr.pub);
            jerr.pub.error_exit = my_error_exit;

            /* Establish the setjmp return context for my_error_exit to use. */
            if (setjmp(jerr.setjmp_buffer)) {
                /* If we get here, the JPEG code has signaled an error.
                 * We need to clean up the JPEG object, close the input file, and return.
                 */
                jpeg_destroy_decompress(&cinfo);
                throw Exception("jpeg error");
            }
            /* Now we can initialize the JPEG decompression object. */
            jpeg_create_decompress(&cinfo);

            /* Step 2: specify data source (eg, a file) */
            jpeg_stdio_src(&cinfo, (FILE*)fp.handle);

            /* Step 3: read file parameters with jpeg_read_header() */

            (void) jpeg_read_header(&cinfo, TRUE);
            /* We can ignore the return value from jpeg_read_header since
             *   (a) suspension is not possible with the stdio data source, and
             *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
             * See libjpeg.txt for more info.
             */

            /* Step 4: set parameters for decompression */

            /* In this example, we don't need to change any of the defaults set by
             * jpeg_read_header(), so we do nothing here.
             */

            /* Step 5: Start decompressor */
            (void) jpeg_start_decompress(&cinfo);
            /* We can ignore the return value since suspension is not possible
             * with the stdio data source.
             */

            const unit_t width  = cinfo.output_width;
            const unit_t height = cinfo.output_height;
            Image        image(width,height);

            /* We may need to do some setup of our own at this point before reading
             * the data.  After jpeg_start_decompress() we have the correct scaled
             * output image dimensions available, as well as the output colormap
             * if we asked for color quantization.
             * In this example, we need to make an output work buffer of the right size.
             */
            /* JSAMPLEs per row in output buffer */
            //const unit_t row_stride = cinfo.output_width * cinfo.output_components;

            /* Make a one-row-high sample array that will go away when done with image */

            //JSAMPARRAY buffer = NULL;
            //buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
            //JSAMPLE   *bufline = (JSAMPLE *) calloc(row_stride,1);
            //JSAMPLE   *buffer[1] = { bufline };
            const unsigned   depth = cinfo.output_components;
            JPEG_Row_Convert cproc = NULL;
            switch(depth)
            {
                case 1: cproc = JPEG_Row_Convert1; break;
                case 3: cproc = JPEG_Row_Convert3; break;
                default:
                    throw Exception("JPEG_Format: unhandled depth=%u",depth);
            }

            JPEG_Buffer buffer(cinfo.output_width, cinfo.output_components);

            /* Step 6: while (scan lines remain to be read) */
            /*           jpeg_read_scanlines(...); */

            /* Here we use the library's state variable cinfo.output_scanline as the
             * loop counter, so that we don't have to keep track ourselves.
             */
            unit_t j=0;
            while(cinfo.output_scanline < cinfo.output_height) {
                /* jpeg_read_scanlines expects an array of pointers to scanlines.
                 * Here the array is only one element long, but you could ask for
                 * more than one scanline at a time if that's more convenient.
                 */
                (void) jpeg_read_scanlines(&cinfo, *buffer, 1);
                /* Assume put_scanline_someplace wants a pointer and sample count. */
                //put_scanline_someplace(buffer[0], row_stride);
                cproc(image[j++],(*buffer)[0],width);
            }

            /* Step 7: Finish decompression */

            (void) jpeg_finish_decompress(&cinfo);
            /* We can ignore the return value since suspension is not possible
             * with the stdio data source.
             */

            /* Step 8: Release JPEG decompression object */

            /* This is an important step since it will release a good deal of memory. */
            jpeg_destroy_decompress(&cinfo);

            /* After finish_decompress, we can close the input file.
             * Here we postpone it until after no more JPEG errors are possible,
             * so as to simplify the setjmp error logic above.  (Actually, I don't
             * think that jpeg_destroy can do an error exit, but why assume anything...)
             */

            /* At this point you may want to check to see whether any corrupt-data
             * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
             */

            /* all done */
            return image;
        }

    }

}


