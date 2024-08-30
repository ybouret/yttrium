
//! \file

#ifndef Y_Chemical_Warden_Included
#define Y_Chemical_Warden_Included 1

#include "y/chemical/plexus/warden/trims.hpp"
#include "y/chemical/plexus/warden/fixed.hpp"
#include "y/chemical/plexus/warden/trade.hpp"
#include "y/chemical/plexus/cluster.hpp"

#include "y/sort/heap.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Warden: fix laws and equalize
        //
        //
        //______________________________________________________________________
        class Warden : public Quantized
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Conservation::Laws::Group Group;    //!< alias
            typedef Conservation::LNode       LNode;    //!< alias
            typedef Conservation::LRepo       LRepo;    //!< alias
            static const char * const         CallSign; //!< "Chemical::Warden"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Warden(const Cluster &cluster); //!< setup from cluster
            virtual ~Warden() noexcept;              //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void prolog() noexcept; //!< free all injected

            //! sanitize  equilibria
            void sanitize(XWritable &C, const Level L, XMLog &xml);



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Cluster      &mine;    //!< my cluster
            const Group * const head;    //!< first group
            const size_t        rows;    //!< laws max group size
            const size_t        cols;    //!< max species in sub-level
            XAdd                xadd;    //!< for internal computations
            XMatrix             conc;    //!< workspace for fixed concentrations
            Fixed::Series       jail;    //!< fixed
            XSwell              cinj;    //!< injected accumulator
            Fund                fund;    //!< shared memories
            LRepo               lawz;    //!< laws with zero values
            Trims               trms;    //!< trims
            SingleFrontier      best;    //!< best effort to equalize
            SRepo               wobbly;  //!< negative species list
            XMatrix             ctrade;  //!< traded concentrations
            XMatrix             dtrade;  //!< traded changes
            Trade::Series       trades;  //!< trades
            XSwell              xaccum;  //!< accumulators for multiple trades

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Warden);

            //! finalize using ROAMING equilibria for remaining negative C
            void   finalize(XWritable &C, const Level L, XMLog &xml);

            size_t collectRoaming(ERepo       &target,
                                  const EList &source) const;

            //! equalize CONSERVED with LIMITED equilibria
            /**
             enter when sanitized and possibly lawz
             */
            void equalize(XWritable &C, const Level L, XMLog &xml);

            //! enforce law and store delta in injected
            void zeroLaws(XWritable  &C, const Level L, XMLog      &xml);

            //! optimize found trades from getUnbalanced()
            void   optimizeTrade(XWritable &C, const Level L, XMLog &xml);

            //! fill concentrations trades and wobbly species
            size_t getUnbalanced(const XReadable &C, const Level L, XMLog &xml);


            //! construct best effort single frontier
            void buildFrontier(const SingleFrontier &limiting,
                               const Frontiers      &required);

            //! check if concentrations need to be injected to obey laws
            /**
             \param G   group to scan
             \param C   concentrations
             \param L   level
             \param xml display
             \return last injected law, NULL if group is ok
             */
            const Conservation::Law * wasInjected(const Group &G, XWritable &C, const Level L, XMLog &xml);

            //! load C and compute all necessary modified concentrations
            /**
             \return true if must fix some law
             */
            bool broken(const Group &group, XWritable &C, const Level L, XMLog &xml);

            //! find lowest fix and apply it, keeping track of injected
            /**
             \param C   concentrations to fix
             \param L   level
             \param xml display
             \return last fixed law, to be zeroed!
             */
            const Conservation::Law * reduce(XWritable &C, const Level  L, XMLog &xml);

            //! check who still needs to be fixed in jail
            void update(const XReadable &C, const Level L, XMLog &xml);
        };

    }

}

#endif

