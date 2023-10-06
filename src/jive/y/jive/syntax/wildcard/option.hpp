//! \file

#ifndef Y_Jive_Syntax_Option_Included
#define Y_Jive_Syntax_Option_Included 1

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
            //! Rule is an option
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
                static const uint32_t UUID = Y_FOURCC('O','P','T',':'); //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Option(const Rule &); //!< setup from rule
                virtual ~Option() noexcept;    //!< cleanup
                
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
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Option);

               

            };
        }

    }

}

#endif
