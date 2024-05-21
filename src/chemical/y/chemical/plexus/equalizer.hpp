
//! \file

#ifndef Y_Chemical_Equalizer_Included
#define Y_Chemical_Equalizer_Included 1

#include "y/chemical/reactive/clusters.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! equalize negative concentrations
        //
        //
        //______________________________________________________________________
        class Equalizer
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxArray<Fence,MemoryModel> Fences; //!< alias

            class Fixed
            {
            public:
                Fixed(const xreal_t     theGain,
                      const XReadable  &theConc,
                      const Controller &theCntl,
                      const Fence      &theWall) noexcept :
                gain(theGain),
                conc(theConc),
                cntl(theCntl),
                wall(theWall)
                {
                }

                ~Fixed() noexcept {}

                Fixed(const Fixed &other) noexcept :
                gain(other.gain),
                conc(other.conc),
                cntl(other.cntl),
                wall(other.wall)
                {
                }

                void to(XMLog &xml, const char *pfx = 0) const
                {
                    if(!pfx) pfx = "";
                    Y_XMLOG(xml, pfx << "gain = " << std::setw(15) << real_t(gain) << " @" << cntl.primary);
                }

                std::ostream &  displayCompact( std::ostream &os ) const
                {
                    cntl.primary.displayCompact(os,conc,SubLevel);
                    return os;
                }

                //! set primary components into TopLevel C
                void set(XWritable &Ctop) const
                {
                    cntl.primary.transfer(Ctop,TopLevel,conc,SubLevel);
                }

                void add(XAddArray &xadds, XWritable &Ctop, AddressBook &rover) const
                {
                    cntl.custom.transfer(Ctop,TopLevel,conc,SubLevel);
                    for(const SNode *node=cntl.roving.head;node;node=node->next)
                    {
                        const Species &sp = **node;
                        rover |= sp;
                        const size_t  si = sp.indx[SubLevel];
                        const size_t  ti = sp.indx[TopLevel];
                        const xreal_t dc = conc[si]-Ctop[ti]; // nu * xi
                        xadds[si] << dc;
                    }
                }



                const xreal_t      gain;
                const XReadable  & conc;
                const Controller & cntl;
                const Fence      & wall;

            private:
                Y_DISABLE_ASSIGN(Fixed);
            };

            typedef Small::CoopHeavyList<Fixed> FList; //!< alias
            typedef FList::NodeType             FNode; //!< alias
            typedef FList::ProxyType            FBank; //!< alias

            static SignType CompareFixed(const FNode * const lhs,
                                         const FNode * const rhs) noexcept
            {
                return Sign::Of( (**rhs).gain, (**lhs).gain );
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! prepare for a list of clusters
            explicit Equalizer(const Clusters &);

            //! cleanup
            virtual ~Equalizer() noexcept;
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! tune
            void tune(XWritable     &C0,
                      const Cluster &cluster,
                      XMLog         &xml);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
            XMatrixType                 Ceqz;
            XAddArray                   xadds;
            Banks                       banks;
            Fences                      fences;
            AddressBook                 negative;
            FBank                       fbank;
            FList                       flist;
            FList                       glist;
            AddressBook                 rover;
            XAdd                        xadd;

            size_t getNegativeCntl(const XReadable &C0, const Cluster &cluster);
            void   tuneControllers(XWritable &C0, const Cluster &cluster, XMLog &xml);

        };
    }

}

#endif
