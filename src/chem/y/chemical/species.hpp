
//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/lingo/syntax/xnode.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Chemical
    {



        //______________________________________________________________________
        //
        //
        //
        //! Species is a charged entity
        //
        //
        //______________________________________________________________________
        class Species  
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<const String,const Species> Handle; //!< alias
            static const char * const  CallSign; //!< "Chemical::Species"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________


            //! cleanup
            virtual ~Species() noexcept;

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
    }

}

#endif

