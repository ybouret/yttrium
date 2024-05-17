
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
            typedef CxxArray<Fence,MemoryModel> Fences;

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
            Banks                       banks;
            Fences                      fences;
            AddressBook                 negative;
            XAdd                        xadd;
            FBank                       bank;

            void tuneControllers(XWritable     &C0,
                                 const Cluster &cluster,
                                 XMLog         &xml);

        };
    }

}

#endif
