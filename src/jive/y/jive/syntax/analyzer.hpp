
//! \file

#ifndef Y_Jive_Syntax_Analyzer_Included
#define Y_Jive_Syntax_Analyzer_Included 1

#include "y/jive/syntax/xnode.hpp"

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
            //! Syntax Analyzer to walk down and process AST
            //
            //
            //__________________________________________________________________
            class Analyzer
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~Analyzer() noexcept; //!< cleanup
                explicit Analyzer() noexcept; //!< setup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! what to do when a  terminal is encountered
                virtual void onTerminal(const String &name,
                                        const Token  &data);

                //! what to do when an internal is encoutered
                virtual void onInternal(const String &name,
                                        const size_t  size);

                //! reset depth, walk from root
                void           run(const XNode &root);

                //! helper to indent std::cerr using depth
                std::ostream & indent() const;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const unsigned depth; //!< current depth
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
                void walk(const XNode *root);

            };

        }
    }

}

#endif
