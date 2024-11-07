

//! \file

#ifndef Y_Lingo_Syntax_Repeat_Included
#define Y_Lingo_Syntax_Repeat_Included 1

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
            //! Repeat of an existing ROBUST rule
            //
            //
            //__________________________________________________________________
            class Repeat : public Wildcard
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const uint32_t UUID = Y_FOURCC('X','R','E','P'); //!< identifier

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with name depending on rule.name + nmin
                explicit Repeat(const Rule & _rule, const size_t  nmin);
                virtual ~Repeat() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual void viz(OutputStream &fp) const;        //!< this + arrow to rule
                virtual bool robust()     const noexcept;        //!< false
                virtual bool accepts(Y_Lingo_Syntax_Args) const; //!< true, rule content or nothing
                
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const size_t atLeast; //!< valid minimal count

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Repeat);
                static Caption NameFor(const Rule &, const size_t);
            };

        }

    }

}

#endif

