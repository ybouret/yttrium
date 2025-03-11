
//! \file


#ifndef Y_Chemical_Equilibrium_Const_Included
#define Y_Chemical_Equilibrium_Const_Included 1


#include "y/chemical/reactive/equilibrium/raw.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Default constant equilibirum
        //
        //
        //______________________________________________________________________
        class ConstEquilibrium : public RawEquilibrium
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from parsed node with numerical string
            explicit ConstEquilibrium(Library &lib,  XNode * const eNode);

            //! cleanup
            virtual ~ConstEquilibrium() noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const xreal_t               Konst; //!< numerical value


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            virtual xreal_t getK(xreal_t);
            static  xreal_t Eval(const String &);
        };

    }

}

#endif

