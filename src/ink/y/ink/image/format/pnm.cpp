
#include "y/ink/image/format/pnm.hpp"
#include "y/string/boolean.hpp"
#include "y/system/exception.hpp"
#include "y/stream/libc/output.hpp"
#include "y/color/grayscale.hpp"

namespace Yttrium
{
    namespace Ink
    {

        namespace
        {
            static inline
            void  EmitWxH(OutputStream &fp, const Bitmap &bmp)
            {
                fp("%lu %lu\n", static_cast<unsigned long>(bmp.w), static_cast<unsigned long>(bmp.h) );
            }

            // black/white
            static inline void ToPBM(OutputStream       &fp,
                                          const RGBA         &c)
            {
                if (c.r>0||c.g>0||c.b>0) fp << '0'; else fp << '1';
            }

            //! gray scale
            static inline void ToPGM(OutputStream       &fp,
                                     const RGBA         &c)
            {
                const uint8_t gs = Color::GrayScale::Pack<uint8_t>(c);
                fp("%3u",gs);
            }

            //! color
            static inline void ToPPM(OutputStream       &fp,
                                     const RGBA         &c)
            {
                fp("%3u %3u %3u",c.r, c.g, c.b);
            }


            template <typename PROC>
            static inline void WriteRGBA(OutputStream       &fp,
                                         const Codec::Image &image,
                                         PROC               &proc)
            {
                for(unit_t j=0;j<image.h;++j)
                {
                    const PixRow<RGBA> &line = image(j);
                    for(unit_t i=0;i<image.w;++i)
                    {
                        proc(fp,line(i));
                        //if(i<image.xt) fp << ' ';
                        fp << '\n';
                    }

                }
            }

            static inline
            void WriteComments(OutputStream &fp, const Options * const options)
            {
                const String *com = Options:: Query(options, "comment");
                if(0!=com)
                {
                    fp << "# " << *com << '\n';
                }
            }

            

            static inline
            void SaveP1(const Codec::Image &image,
                        const String       &filename,
                        const Options      *options)
            {
                OutputFile fp(filename);
                fp << "P1\n";
                WriteComments(fp,options);
                EmitWxH(fp,image);
                WriteRGBA(fp,image,ToPBM);
            }


            static inline
            void SaveP2(const Codec::Image    &image,
                        const String          &filename,
                        const Options * const  options)
            {
                OutputFile fp(filename);
                fp << "P2\n";
                WriteComments(fp,options);
                EmitWxH(fp,image);
                fp << "255\n";
                WriteRGBA(fp,image,ToPGM);
            }

            static inline
            void SaveP3(const Codec::Image   &image,
                        const String         &filename,
                        const Options * const options)
            {
                OutputFile fp(filename);
                fp << "P3\n";
                WriteComments(fp,options);
                EmitWxH(fp,image);
                fp << "255\n";
                WriteRGBA(fp,image,ToPPM);
            }





        }

        FormatPNM::  FormatPNM() : Format(CallSign,"(p[bgp]m)&") {}
        FormatPNM:: ~FormatPNM() noexcept {}

        const char * const FormatPNM:: CallSign = "PNM";

        static   bool QueryBinary(const Options * const options)
        {
            static const char    name[] = "binary";
            const String * const args = Options::Query(options,"binary");
            if(0==args) return false;
            return StringToBoolean::Get(*args,name);
        }

        FormatPNM:: Kind FormatPNM:: GetKind(const String &lowerCaseExt, const bool binary)
        {
            if("pbm" == lowerCaseExt) return binary ? P4 : P1;
            if("pgm" == lowerCaseExt) return binary ? P5 : P2;
            if("ppm" == lowerCaseExt) return binary ? P6 : P3;
            throw Specific::Exception(CallSign, "invalid extension '%s'", lowerCaseExt.c_str());
        }



#define Y_PNM_SAVE(FMT) case FMT: Save##FMT(image,fileName,options); break

        void  FormatPNM:: save(const Image   &image,
                               const String  &fileName,
                               const Options *options) const
        {
            const bool   binary = QueryBinary(options);
            const String ext    = LowerCaseExt(fileName); std::cerr << "ext=" << ext << " / binary=" << binary << std::endl;
            const Kind   kind   = GetKind(ext,binary);
            switch(kind)
            {
                    Y_PNM_SAVE(P1);
                    Y_PNM_SAVE(P2);
                    Y_PNM_SAVE(P3);
                default: throw Specific::Exception(CallSign,"%s not implemented yet", ext.c_str());
            }
        }

        Codec::Image FormatPNM:: load(const String &fileName,
                                      const Options * ) const
        {
            throw Specific::Exception(CallSign,"load is not implemented yet for '%s'", fileName.c_str());
            return Image(1,1);
        }
    }
}

