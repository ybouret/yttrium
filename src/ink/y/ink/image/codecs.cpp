
#include "y/ink/image/codecs.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/system/exception.hpp"
#include "y/vfs/vfs.hpp"


namespace Yttrium
{
    namespace Ink
    {
        const char * const Codecs:: CallSign = "Ink::Image::Codecs";


        typedef Yttrium::SuffixSet<String,Format::Handle> FormatDB;

        class Codecs:: Code : public FormatDB
        {
        public:

            inline explicit Code(Lockable &master) : FormatDB(), sync(master) {}
            inline virtual ~Code() noexcept {}

            inline void record(Format * const fmt)
            {
                assert(0!=fmt);
                const Format::Handle handle(fmt);
                if(!insert(handle)) throw Specific::Exception(CallSign,"multiple format '%s'", handle->key().c_str());
            }

            const Format & findFor(const String &path)
            {
                Y_LOCK(sync);
                for(Iterator it=begin();it!=end();++it)
                {
                    Format &fmt = **it;
                    if(fmt.matches(path)) return fmt;
                }

                throw Specific::Exception(CallSign,"unknown file type '%s'",VFS::BaseName(path));
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            Lockable &sync;
        };

        static void *         code_[ Y_WORDS_FOR(Codecs::Code) ];
        static Codecs::Code * code = 0;


        Codecs:: Codecs() : Codec(CallSign)
        {
            try
            {
                code = new ( Y_STATIC_ZARR(code_) ) Code(access);
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

        bool Codecs:: has(const String &fid) const noexcept
        {
            assert(0!=code);
            return 0 != code->search(fid);
        }

        bool Codecs:: has(const char * const fid) const
        {
            const String _(fid);
            return has(_);
        }




        void Codecs:: save(const Image         &image,
                           const String        &fileName,
                           const FormatOptions *options) const
        {
            assert(0!=code);
            code->findFor(fileName).save(image,fileName,options);
        }

        Codec::Image Codecs:: load(const String        &fileName,
                                   const FormatOptions *options) const
        {
            assert(0!=code);
            return code->findFor(fileName).load(fileName,options);
        }


        void Codecs:: fetch(Sequence<Format::Handle> &formats) const
        {
            assert(0!=code);
            for(Code::Iterator it=code->begin();it!=code->end();++it)
            {
                const Format::Handle &handle = *it;
                formats << handle;
            }
        }


    }

}


