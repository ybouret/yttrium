
//! \file

#ifndef Y_Jive_Syntax_Repeat_Included
#define Y_Jive_Syntax_Repeat_Included 1

#include "y/jive/syntax/wildcard.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {
            //__________________________________________________________________
            //
            //
            //
            //! Repeat a rule
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
                static const uint32_t UUID = Y_FOURCC('R','E','P',':'); //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Repeat(const Rule &, const unsigned nmin); //!< setup from rule
                virtual ~Repeat() noexcept;                       //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! always return true
                virtual bool accepts(Y_JIVE_SYNTAX_RULE_API) const;

                //! GraphViz
                virtual void vizCore(OutputStream &) const;
                
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const unsigned atLeast;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Repeat);



            };
        }

    }

}

#endif
