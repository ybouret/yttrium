
//! \file


#ifndef Y_Chemical_Cursors_Included
#define Y_Chemical_Cursors_Included 1

#include "y/chemical/plexus/equalizer/banks.hpp"
#include "y/chemical/plexus/equalizer/restartable.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! ordered cursors
        //
        //
        //______________________________________________________________________
        class Cursors : public Proxy<const CrList>, public Restartable
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Cursors(const EqzBanks &banks) noexcept; //!< setup empty
            virtual ~Cursors() noexcept;                      //!< cleanup

            //__________________________________________________________________
            //
            //
            // Mehthods
            //
            //__________________________________________________________________
            virtual void restart() noexcept; //!< restart all

            //! dispatch species and vanishing extent
            void operator()(const Species &sp, const xreal_t xi);



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cursors);
            Y_PROXY_DECL();
            static   SignType Compare(const CrNode * const, const CrNode * const) noexcept;
            bool              checked() const noexcept;
            CrNode *          crNode(const Species &sp, const xreal_t xi); //!< create new cursor

            CrList my;
            SBank  sb;
        };






    }

}

#endif

