
//! \file

#ifndef Y_Information_Entropic_Model_Included
#define Y_Information_Entropic_Model_Included 1

#include "y/information/entropic/symbol.hpp"

namespace Yttrium
{


    namespace Information
    {

        namespace Entropic
        {

            //__________________________________________________________________
            //
            //
            //
            //! Model interface
            //
            //
            //__________________________________________________________________
            class Model
            {
            public:
                virtual ~Model() noexcept;

                //! build from used units
                virtual void build(Symbol::List &) = 0;

            protected:
                explicit Model() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Model);
            };

        }

    }

}

#endif

