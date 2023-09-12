
//! \file

#ifndef Y_WOVEn_Subspace_Included
#define Y_WOVEn_Subspace_Included 1

#include "y/woven/indices.hpp"
#include "y/apex/ortho/family.hpp"
#include "y/container/matrix.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        typedef  Apex::Ortho::Vector          QVector;
        typedef  Apex::Ortho::Family          QFamily;
        typedef  Small::CoopHeavyList<size_t> IList;
        typedef  IList::ProxyType             IProxy;
        
        class Subspace
        {
        public:

            static const char * const CallSign;

            template <typename T>
            explicit Subspace(const Matrix<T> &mu,
                              const size_t     ir,
                              const IProxy    &px) :
            qfamily( CheckDims(mu) ),
            indices( CheckDOFs(mu) ),
            staying( px )
            {
                if( !qfamily.wouldAccept(mu[ir]) ) SingularFirstRowException();
                setup(ir,mu.rows);
                std::cerr << qfamily << " @{" << indices << "}" << std::endl;
            }

            virtual ~Subspace() noexcept;
            

            QFamily qfamily;
            Indices indices;
            IList   staying;

        private:
            Y_DISABLE_ASSIGN(Subspace);
            static size_t CheckDims(const MatrixMetrics &mu);
            static size_t CheckDOFs(const MatrixMetrics &mu);
            static void   SingularFirstRowException();
            void setup(const size_t ir, const size_t nr);

        };

    }

}

#endif

