
//! \file

#ifndef Y_Ink_Image_Codecs_Included
#define Y_Ink_Image_Codecs_Included 1

#include "y/ink/image/codec.hpp"


namespace Yttrium
{
    namespace Ink
    {

        class Codecs : public Codec, public Singleton<Codecs>
        {
        public:
            static const char * const      CallSign;
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 20;
            class Code;

            //! save image according to format+options
            virtual void save(const Image         &image,
                              const String        &fileName,
                              const FormatOptions *options) const;

            //! load image according to format+options
            virtual Image load(const String        &fileName,
                               const FormatOptions *options) const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Codecs);
            friend class Singleton<Codecs>;
            explicit Codecs();
            virtual ~Codecs() noexcept;
        };

    }

}

#endif
