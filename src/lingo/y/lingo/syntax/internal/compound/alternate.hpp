
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
            //__________________________________________________________________
            //
            //
            //
            //! Alternate of existing rules
            //
            //
            //__________________________________________________________________
            class Alternate : public Compound
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const uint32_t UUID = Y_FOURCC('X', 'A', 'L', 'T'); //!< identifier

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with name
                template <typename NAME> inline
                explicit Alternate(const NAME & _name) :
                Compound(_name,UUID)
                {
                    Y_Lingo_Syntax_Rule(Alternate);
                }

                //! cleanup
                virtual ~Alternate() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                Alternate & operator |= (const Rule &); //!< alias to add()

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
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

