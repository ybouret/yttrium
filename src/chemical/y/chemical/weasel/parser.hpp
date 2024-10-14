

//! \file

#ifndef Y_Chemical_Weasel_Parser_Included
#define Y_Chemical_Weasel_Parser_Included 1

#include "y/jive/parser.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Weasel
        {

            typedef Jive::Syntax::XNode XNode; //!< alias


            //__________________________________________________________________
            //
            //
            //
            //! WEASEL is Equilibria And Species Elementary Language
            //
            //
            //__________________________________________________________________
            class Parser : public Jive::Parser
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Parser(const char * const); //!< setup with name
                virtual ~Parser() noexcept;          //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                XNode *  load(Jive::Module *); //!< parse and post-process module's content

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };

        }

    }

}

#endif
