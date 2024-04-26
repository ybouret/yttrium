
#include "y/ink/image/codecs.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/system/exception.hpp"
#include "y/vfs/vfs.hpp"


namespace Yttrium
{
    namespace Ink
    {
        const char * const Codecs:: CallSign = "Ink::Image::Codecs";



        class Codecs:: Code : public SuffixSet<String,Format::Handle>
        {
        public:

            inline explicit Code() : SuffixSet<String,Format::Handle>() {}
            inline virtual ~Code() noexcept {}

            inline void record(Format * const fmt)
            {
                assert(0!=fmt);
                const Format::Handle handle(fmt);
                if(!insert(handle)) throw Specific::Exception(CallSign,"multiple format '%s'", handle->key().c_str());
            }

            const Format & findFor(const String &path)
            {

                for(Iterator it=begin();it!=end();++it)
                {
                    Format &fmt = **it;
                    if(fmt.matches(path)) return fmt;
                }

                throw Specific::Exception(CallSign,"unknown file type '%s'",VFS::BaseName(path));
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        static void *         code_[ Y_WORDS_FOR(Codecs::Code) ];
        static Codecs::Code * code = 0;


        Codecs:: Codecs() : Codec(CallSign)
        {
            try {
                code = new ( Y_STATIC_ZARR(code_) ) Code();
                //std::cerr << "sizeof(Code)=" << sizeof(Code) << " / " << sizeof(code_) << std::endl;
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

        void Codecs:: operator()(Format * const fmt)
        {
            assert(0!=fmt);
            assert(0!=code);
            code->record(fmt);
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


