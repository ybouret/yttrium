//! \file

#ifndef Y_Chemical_Weasel_Parser_Included
#define Y_Chemical_Weasel_Parser_Included 1

#include "y/chemical/weasel.hpp"
#include "y/lingo/parser.hpp"
#include "y/hashing/perfect.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Parser of Weasel
        //
        //
        //______________________________________________________________________
        class Weasel:: Parser : public Lingo::Parser
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Parser(const Lingo::Caption &); //!< setup
            virtual ~Parser() noexcept;              //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! remove '+' from REAC/PROD
            XNode * postProcess(XNode * const) const noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Compound   &           WEASEL;       //!< top level rule
            Compound   &           FORMULA;      //!< formula rule
            Compound   &           EQUILIBRIUM;  //!< equilibrium rule
            const Rule &           POSITIVE;     //!< mark('+') rule
            const Hashing::Perfect actors;       //!< REAC/PROD to postprocess


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }

}

#endif


