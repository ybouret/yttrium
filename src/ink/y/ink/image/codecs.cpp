
#include "y/ink/image/codecs.hpp"


namespace Yttrium
{
    namespace Ink
    {
        const char * const Codecs:: CallSign = "Ink::Image::Codecs";

        class Codecs:: Code
        {
        public:
            inline explicit Code() {}
            inline virtual ~Code() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        static void *         code_[ Y_WORDS_FOR(Codecs::Code) ];
        static Codecs::Code * code = 0;


        Codecs:: Codecs() : Codec(CallSign)
        {
            try {
                code = new ( Y_STATIC_ZARR(code_) ) Code();
                std::cerr << "sizeof(Code)=" << sizeof(Code) << " / " << sizeof(code_) << std::endl;
            }
            catch(...)
            {
                code = 0;
                throw;
            }

        }

        Codecs:: ~Codecs() noexcept 
        {
            assert(0!=code);
            Memory::OutOfReach::Naught(code);
            code=0;
        }

        void Codecs:: save(const Image         &image,
                           const String        &fileName,
                           const FormatOptions *options) const
        {

        }

        //! load image according to format+options
        Codec::Image Codecs:: load(const String        &fileName,
                                   const FormatOptions *options) const
        {

        }

    }

}


