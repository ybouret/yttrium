
//! \file

#ifndef Y_Chemical_Mixes_Included
#define Y_Chemical_Mixes_Included 1

#include "y/chemical/plexus/mix.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/plexus/conservation/laws.hpp"

#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {



        //______________________________________________________________________
        //
        //
        //
        //! list of indepedent mixes of dependents equilibria
        //
        //
        //______________________________________________________________________
        class Mixes : public Proxy<const Mix::List>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Chemical::Mixes"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup/update from equilibria
            explicit Mixes(XMLog &      xml,
                           Equilibria & eqs);
            virtual ~Mixes() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Mixes);    //!< display


            //! guard each Mix
            void guard(XMLog &xml, XWritable &C, const Level L, XWritable &injected);


            //! solve each Mix
            void solve(XMLog &xml, XWritable &C, const Level L, const XReadable &K);

            //! write graphViz code for each order[which]
            void viz(OutputStream &fp, const size_t which) const;

            //! write standalone graphViz code for order[which]
            void graphViz(OutputStream &fp, const size_t which) const;

            //! render in "root[1:maxOrder].png"
            void renderGraphViz(XMLog &xml, const String &root) const;

        private:
            Mix::List my;

            Y_DISABLE_COPY_AND_ASSIGN(Mixes);
            Y_PROXY_DECL();
            void checkFusion();

        public:
            XBanks        xbanks;   //!< shared banks for associated algorithms
            const size_t  maxOrder; //!< max order of hybrid equilibrium
            const SDuplex involved; //!< all involved species
        };

    }

}

#endif

