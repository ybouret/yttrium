
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

            //__________________________________________________________________
            //
            //! Fixed phase space
            //__________________________________________________________________
            class Fixed
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
             
                //! setup with all parameters
                Fixed(const xreal_t     theGain,
                      const XReadable  &theConc,
                      const Controller &theCntl,
                      const Fence      &theWall) noexcept;

                //! cleanup
                ~Fixed() noexcept;

                //! copy
                Fixed(const Fixed &) noexcept;

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________

                //! display
                void to(XMLog &xml, const char *pfx = 0) const;

                //! forward to cntl.primary
                std::ostream &  displayCompact( std::ostream &os ) const;

                //! set cntl.primary components into TopLevel C
                void set(XWritable &Ctop) const;

                //! set custom components and store roving components
                void add(XAddArray &xadds, XWritable &Ctop, AddressBook &rover) const;
                
                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                const xreal_t      gain; //!< gain from custom
                const XReadable  & conc; //!< improved phase space
                const Controller & cntl; //!< matching controller
                const Fence      & wall; //!< matching fence

            private:
                Y_DISABLE_ASSIGN(Fixed);
            };

            typedef Small::CoopHeavyList<Fixed> FList; //!< alias
            typedef FList::NodeType             FNode; //!< alias
            typedef FList::ProxyType            FBank; //!< alias

            //! compare by decreasing gain
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
