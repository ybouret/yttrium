
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chemical/reactive/conservation/law.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            //__________________________________________________________________
            //
            //
            //
            //! set of conservation laws for a cluster
            //
            //
            //__________________________________________________________________
            class Laws : public Proxy<const Law::List>
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Laws() noexcept; //!< setup
                virtual ~Laws() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! create the list of conservations
                void writeDown(const Matrix<unsigned> &Qm,
                               const SpSubSet         &table);

                //! output GraphViz code
                void viz(OutputStream &fp) const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
                Law::List cll;

                virtual ConstInterface & surrogate() const noexcept;
            };
        }

    }

}

#endif
