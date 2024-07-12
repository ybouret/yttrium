
//! \file

#ifndef Y_Chemical_Injector_Included
#define Y_Chemical_Injector_Included 1

#include "y/chemical/plexus/clusters.hpp"
#include "y/container/cxx/series.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Injector
        {
        public:
            explicit Injector(const Clusters &cls);
            virtual ~Injector() noexcept;


            class Broken
            {
            public:
                Broken(const Conservation::Law &l, XWritable &c) noexcept;
                Broken(const Broken &b) noexcept;
                ~Broken() noexcept;
                Y_OSTREAM_PROTO(Broken);


                static int Compare(const Broken &lhs, const Broken &rhs) noexcept;
                bool still(const XReadable &Ctop, XAdd &xadd);

                xreal_t                  gain;
                const Conservation::Law &claw;
                XWritable               &Csub;

            private:
                Y_DISABLE_ASSIGN(Broken);
            };

            const bool        used;
            const size_t      rows; //!< max group size
            const size_t      cols; //!< max species in sub-level
            CxxSeries<Broken> jail;
            XMatrix           Cnew;
            CxxArray<XAdd>    Cinj;
            XAdd              xadd;
            const xreal_t     zero;

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
        };
    }

}

#endif
