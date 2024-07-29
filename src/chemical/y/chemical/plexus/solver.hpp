
//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1

#include "y/chemical/plexus/solver/qbuilder.hpp"
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/container/cxx/series.hpp"


#include "y/data/list/ordered.hpp"
#include "y/data/pool/cxx.hpp"

namespace Yttrium
{
    namespace Chemical
    {

#if 1
        class Vertex : public Object, public XArray
        {
        public:
            typedef CxxPoolOf<Vertex> Pool;
            struct Comparator
            {
                inline SignType operator()(const Vertex * const lhs, const Vertex * const rhs) const noexcept
                {
                    return Comparison::CxxDecreasing(lhs->cost, rhs->cost);
                }
            };


            explicit Vertex(const size_t maxSpecies) :
            XArray(maxSpecies), cost(0), next(0), prev(0)
            {

            }

            virtual ~Vertex() noexcept {}

            void clear() noexcept { ld(cost=0); }
            
            xreal_t cost;
            Vertex *next;
            Vertex *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Vertex);
        };

        typedef OrderedList<Vertex,Vertex::Comparator,OrderedListQueryHead> Vertices;

        class Simplex : public Vertices
        {
        public:
            explicit Simplex(size_t       maxEqs, 
                             const size_t maxSpecies) :
            Vertices(),
            pool(),
            dims(maxSpecies)
            {
                ++maxEqs;
                while(maxEqs-- > 0) pool.store( new Vertex(dims) );
            }

            void free() noexcept {
                while(size>0) pool.store( popTail() )->clear();
            }

            void load(const xreal_t    cost,
                      const SList     &spec,
                      const XReadable &C,
                      const Level      L)
            {
                AutoPtr<Vertex> ptr = pool.size ? pool.query() : new Vertex(dims);
                Vertex         &vtx = *ptr;
                vtx.cost = cost;
                for(const SNode *sn=spec.head;sn;sn=sn->next)
                {
                    const size_t * const indx = (**sn).indx;
                    vtx[ indx[SubLevel] ] = C[ indx[L] ];
                }
                this->store( ptr.yield() );
            }


            Vertex::Pool pool;
            const size_t dims;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Simplex);
        };
#endif

        //______________________________________________________________________
        //
        //
        //
        //! Solver
        //
        //
        //______________________________________________________________________
        class Solver
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Solver(const Clusters &cls);  //!< setup resources to solve clusters
            virtual ~Solver() noexcept;            //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! build objective function from C at level L
            xreal_t objectiveFunction(const XReadable &C,
                                      const Level      L);

            //! use solver as callable function
            xreal_t operator()(const xreal_t u);

            //! run
            void run(const Cluster   &cl,
                     XWritable       &Ctop,
                     const XReadable &Ktop,
                     XMLog           &xml);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Aftermath           afm; //!< aftermath computation
            XMatrix             ceq; //!< storing results
            XArray              phi; //!< temporary gradients
            CxxSeries<xreal_t>  obj; //!< temporary objectice function values
            CxxSeries<Prospect> pps; //!< prospect series
            XArray              Cin; //!< trial input concentrations
            XArray              Cex; //!< trial exit  concentrations
            XArray              Cws; //!< wokspace concentration
            PBank               bnk; //!< shared bank of PNODE
            QBuilders           qdb; //!< QBuilder for different clusters
            Simplex             sim; //!< simplex
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            QBuilder           *ppb; //!< pointer to prospect base
            Cluster            *pcl; //!< pointer to current cluster
            size_t examine(const Cluster   &cl,
                           XWritable       &Ctop,
                           const XReadable &Ktop,
                           bool            &repl,
                           XMLog           &xml);
        };

    }
}

#endif

