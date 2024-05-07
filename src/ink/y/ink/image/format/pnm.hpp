

//! \file

#ifndef Y_Ink_Format_PNM_Included
#define Y_Ink_Format_PNM_Included 1

#include "y/ink/image/format.hpp"

namespace Yttrium
{
    namespace Ink
    {


        //______________________________________________________________________
        //
        //
        //
        //! Portable aNyMap Format
        //
        //
        //______________________________________________________________________
        class FormatPNM : public Format
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "BMP"
            enum Kind
            {
                P1,
                P2,
                P3,
                P4,
                P5,
                P6
            };
            static Kind GetKind(const String &lowerCaseExt, const bool binary);
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit FormatPNM();           //!< setup
            virtual ~FormatPNM() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! saving
            virtual void  save(const Image         &image,
                               const String        &fileName,
                               const FormatOptions *options) const;

            //! loading
            virtual Image load(const String        &fileName,
                               const FormatOptions *options) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormatPNM);
        };
    }

}

#endif

