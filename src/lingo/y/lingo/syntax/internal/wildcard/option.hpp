
//! \file

#ifndef Y_Lingo_Syntax_Option_Included
#define Y_Lingo_Syntax_Option_Included 1

#include "y/lingo/syntax/internal/wildcard.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            class Option : public Wildcard
            {
            public:
                static const uint32_t UUID = Y_FOURCC('X','O','P','T');
                static const char     Suffix = '?';

                explicit Option(const Rule & _rule) :
                Wildcard( NameFor(_rule), UUID, _rule)
                {
                }


                virtual ~Option() noexcept;



                virtual void viz(OutputStream &fp) const;
                virtual bool robust()     const noexcept;
                virtual bool accepts(Y_Lingo_Syntax_Args) const;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Option);
                static Caption NameFor(const Rule &);
            };

        }

    }

}

#endif

