
//! \file

#ifndef Y_Chemical_Cursors_Included
#define Y_Chemical_Cursors_Included 1

#include "y/chemical/plexus/type/cursor.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        // Definitions
        //
        //
        //______________________________________________________________________
        typedef Small::  CoopHeavyList<Cursor> CrList; //!< alias
        typedef CrList:: ProxyType             CrBank; //!< alias
        typedef CrList:: NodeType              CrNode; //!< alias


        class XBanks
        {
        public:
            XBanks();
            ~XBanks() noexcept;
            XBanks(const XBanks &) noexcept;

            SBank  species;
            CrBank cursors;

        private:
            Y_DISABLE_ASSIGN(XBanks);
        };

        //______________________________________________________________________
        //
        //
        //
        //! ordered list of cursors
        //
        //
        //______________________________________________________________________
        class Cursors : public Proxy<const CrList>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with banks for dynamic usage
            explicit Cursors(const XBanks &) noexcept;
            virtual ~Cursors()               noexcept; //!< cleanup
            Cursors(const Cursors &);                  //!< duplicate

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void reset() noexcept; //!< reset all

            //! insert species value at its place
            void operator()(const Species &s,
                            const xReal   &x);

            //! mostly for debug
            bool areOK() const noexcept;
            
        private:
            Y_DISABLE_ASSIGN(Cursors);
            Y_PROXY_DECL();

            CrList my;
            SBank  sb;

            void atTail(const Species &, const xReal &);
            void atHead(const Species &, const xReal &);
        };
    }

}

#endif

