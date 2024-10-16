//! \file

#ifndef Y_CSV_Parser_Included
#define Y_CSV_Parser_Included 1

#include "y/csv/document.hpp"
#include "y/jive/module.hpp"

namespace Yttrium
{

    namespace CSV
    {

        //______________________________________________________________________
        //
        //
        //
        //! a Parser for CSV
        //
        //
        //______________________________________________________________________
        class Parser
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Parser();          //!< setup
            virtual ~Parser() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Document * operator()(Jive::Module *); //!< convert module to document

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
            class Code;
            Code *code;
        };
    }

}

#endif

