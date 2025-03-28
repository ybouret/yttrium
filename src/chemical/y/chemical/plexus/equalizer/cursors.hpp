
//! \file


#ifndef Y_Chemical_Cursors_Included
#define Y_Chemical_Cursors_Included 1

#include "y/chemical/plexus/equalizer/banks.hpp"
#include "y/chemical/plexus/equalizer/restartable.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Cursors : public Proxy<const CrList>, public Restartable
        {
        public:
            explicit Cursors(const EqzBanks &banks) noexcept;
            virtual ~Cursors() noexcept;
            virtual void restart() noexcept;

            void operator()(const Species &sp, const xreal_t xi);

            static   SignType Compare(const CrNode * const, const CrNode * const) noexcept;
            bool              checked() const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cursors);
            Y_PROXY_DECL();
            CrNode *crNode(const Species &sp, const xreal_t &xi)
            {
                const Cursor cr(sb,sp,xi);
                return my.proxy->produce(cr);
            }
            CrList my;
            SBank  sb;
        };






    }

}

#endif

