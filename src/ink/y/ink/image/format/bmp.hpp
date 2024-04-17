
//! \file

#ifndef Y_Ink_Format_BMP_Included
#define Y_Ink_Format_BMP_Included 1

#include "y/ink/image/format.hpp"

namespace Yttrium
{
    namespace Ink
    {
        class FormatBMP : public Format
        {
        public:
            static const char * const CallSign;

            explicit FormatBMP();
            virtual ~FormatBMP() noexcept;


            virtual void  save(const Image         &image,
                               const String        &fileName,
                               const FormatOptions *options) const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormatBMP);
        };
    }

}

#endif

