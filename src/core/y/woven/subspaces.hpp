

//! \file

#ifndef Y_WOVEn_Subspaces_Included
#define Y_WOVEn_Subspaces_Included 1

#include "y/woven/subspace.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        //______________________________________________________________________
        //
        //
        //
        //! list of distinct subspaces
        //
        //
        //______________________________________________________________________
        class Subspaces : public Subspace::List
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from rows of vectors, tighten from multiple values
            template <typename T> inline
            Subspaces(const Matrix<T> &mu) :
            Subspace::List()
            {
                for(size_t ir=mu.rows;ir>0;--ir)
                    pushHead( new Subspace(mu,ir) );
                pack();
            }

            //! cleanup
            virtual ~Subspaces() noexcept;

            //! display
            Y_OSTREAM_PROTO(Subspaces);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! try to pack
            void pack();


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Subspaces);
        };
        

    }

}

#endif
