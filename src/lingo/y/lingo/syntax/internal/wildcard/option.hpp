
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

            //__________________________________________________________________
            //
            //
            //
            //! Rule is an Option
            //
            //
            //__________________________________________________________________
            class Option : public Wildcard
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const uint32_t UUID = Y_FOURCC('X','O','P','T'); //!< identifier
                static const char     Suffix = '?';                     //!< for naming

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                explicit Option(const Rule &); //!< setup with rule.name + suffix
                virtual ~Option() noexcept;    //!< cleanup


                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual bool         robust()                     const; //!< false
                virtual bool         accepts(Y_Lingo_Syntax_Args) const; //!< true, rule content or nothing
                virtual void         vizMark(OutputStream &fp)    const; //!< for GraphViz
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Option);
                static Caption NameFor(const Rule &);
            };

        }

    }

}

#endif

