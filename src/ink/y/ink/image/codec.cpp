
#include "y/ink/image/codec.hpp"

namespace Yttrium
{
    namespace Ink
    {

        Codec:: ~Codec() noexcept {}
        const String & Codec:: key()      const noexcept { return name; }


        void Codec::save(const Image   &image,
                         const char    *fileName,
                         const Options *options)
        {
            const String _(fileName);
            save(image,_,options);
        }

        Codec::Image Codec:: load(const char   *fileName,
                                  const Options *options) const
        {
            const String _(fileName);
            return load(_,options);
        }
    }

}
