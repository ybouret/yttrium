//! \file

#ifndef Y_Chemical_Conservation_Included
#define Y_Chemical_Conservation_Included 1

#include "y/chem/species/actors.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Conservation law
        //
        //
        //______________________________________________________________________
        class Conservation : public Object, public Proxy<const Actors>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Conservation() noexcept; //!< cleanup
            explicit Conservation() noexcept; //!< setup empty
            Y_OSTREAM_PROTO(Conservation);    //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! add (nu != 0) * [sp] to constraint
            void add(const unsigned nu, const Species &sp);
            


            //! check if linked to another
            bool linkedTo(const Conservation &) const noexcept;

            //! compute excess
            xreal excess(const Readable<xreal> &C0, XAdd &xadd) const;


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Conservation);
            virtual ConstInterface & surrogate() const noexcept;
            Actors   actors;
            unsigned normSq;

        public:
            const xreal   nrm2;  //!< |this|^2
            Conservation *next;  //!< for list
            Conservation *prev;  //!< for list
        };


       

    }

}

#endif

