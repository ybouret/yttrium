
//! \file

#ifndef Y_Lingo_Syntax_Alternate_Included
#define Y_Lingo_Syntax_Alternate_Included 1

#include "y/lingo/syntax/internal/compound.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            class Alternate : public Compound
            {
            public:
                static const uint32_t UUID = Y_FOURCC('X', 'A', 'L', 'T');

                template <typename NAME> inline
                explicit Alternate(const NAME & _name) :
                Compound(_name,UUID)
                {
                    Y_Lingo_Syntax_Rule(Alternate);
                }


                virtual ~Alternate() noexcept;

                Alternate & operator |= (const Rule &);
                virtual void viz(OutputStream &fp)        const; //!< GraphViz code
                virtual bool robust()                     const; //!< false is one is flimsy
                virtual bool accepts(Y_Lingo_Syntax_Args) const; //!< first non trivial

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alternate);
            };


        }

    }

}

#endif
