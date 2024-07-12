
//! \file

#ifndef Y_Chemical_Injector_Included
#define Y_Chemical_Injector_Included 1

#include "y/chemical/plexus/clusters.hpp"
#include "y/container/cxx/series.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Fix broken conservation(s) by injecting minimal concentration
        //
        //
        //______________________________________________________________________
        class Injector
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //
            //! Broken law with context
            //
            //__________________________________________________________________
            class Broken
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                Broken(const Conservation::Law &, XWritable &) noexcept; //!< init
                Broken(const Broken &b)                        noexcept; //!< copy
                ~Broken()                                      noexcept; //!< cleanup
                Y_OSTREAM_PROTO(Broken);                                 //!< display

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________

                //! compare by gain
                static int Compare(const Broken &, const Broken &) noexcept;

                //! compute for law
                bool       still(const XReadable &Ctop, XAdd &xadd);

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                xreal_t                  gain; //!< current gain
                const Conservation::Law &claw; //!< studied conservation
                XWritable               &Csub; //!< persistent local fixed

            private:
                Y_DISABLE_ASSIGN(Broken);
            };
          
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Injector(const Clusters &cls); //!< setup memory for clusters
            virtual ~Injector() noexcept;           //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! fix Ctop from all clusters, store delta(>0) in dTop
            /**
             \param cls  clusters
             \param Ctop TopLevel concentrations
             \param dTop TopLevel increases in conc
             \param xml  verbosity
             */
            void operator()(const Clusters  &cls,
                            XWritable       &Ctop,
                            XWritable       &dTop,
                            XMLog           &xml);





        private:
            Y_DISABLE_COPY_AND_ASSIGN(Injector);

            void process(const Cluster  &cl,
                         XWritable      &Ctop,
                         XWritable      &dTop,
                         XMLog          &xml);

            void process(const Conservation::Laws::Group &grp,
                         XWritable                       &Ctop,
                         XMLog                           &xml);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const bool        used; //!< if clusters have som group
            const size_t      rows; //!< max group size
            const size_t      cols; //!< max species in sub-level
            CxxSeries<Broken> jail; //!< local array of broken laws
            XMatrix           Cnew; //!< workspace(rows,cols) to store Csub
            CxxArray<XAdd>    Cinj; //!< workspace, store incremental increases
            XAdd              xadd; //!< for internal additions
            const xreal_t     zero; //!< alias

        };
    }

}

#endif
