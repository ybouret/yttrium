
//! \file

#ifndef Y_Chemical_Cursors_Included
#define Y_Chemical_Cursors_Included 1

#include "y/chemical/plexus/cursor.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small:: CoopHeavyList<Cursor> CrList;
        typedef CrList:: ProxyType            CrBank;
        typedef CrList:: NodeType             CrNode;

        class Cursors : public Proxy<const CrList>
        {
        public:
            explicit Cursors(const SBank &) noexcept;
            virtual ~Cursors()              noexcept;

            void reset() noexcept;
            void operator()(const Species &s,
                            const xReal   &x);

            bool areOK() const noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cursors);
            Y_PROXY_DECL();

            CrBank cb;
            CrList my;
            SBank  sb;

            void atTail(const Species &, const xReal &);
            void atHead(const Species &, const xReal &);
        };
    }

}

#endif

