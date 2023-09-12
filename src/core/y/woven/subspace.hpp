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

        typedef  Apex::Ortho::Vector          QVector; //!< alias
        typedef  Apex::Ortho::Family          QFamily; //!< alias
        typedef  Small::CoopHeavyList<size_t> IList;   //!< alias
        typedef  IList::ProxyType             IProxy;  //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! a subspace is a qfamily with its indices and remaining indices
        //
        //
        //______________________________________________________________________
        class Subspace : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const   CallSign; //!< Subspace
            typedef CxxListOf<Subspace> List;     //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            /**
             \param mu matrices : cols=space dimensions, rows= degrees of freedom
             \param ir row index to start with
             \param px shared proxy for staying indices
             */
            template <typename T>
            explicit Subspace(const Matrix<T> &mu,
                              const size_t     ir,
                              const IProxy    &px) :
            qfamily( CheckDims(mu) ),
            indices( CheckDOFs(mu) ),
            staying( px ),
            next(0),
            prev(0)
            {
                if( !qfamily.wouldAccept(mu[ir]) ) SingularFirstRowException();
                setup(ir,mu.rows);
            }

            //! cleanup
            virtual ~Subspace() noexcept;

            //! display
            Y_OSTREAM_PROTO(Subspace);

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //
            // Memberss
            //
            //__________________________________________________________________
            QFamily   qfamily; //!< family of orthogonal vectors
            Indices   indices; //!< indices of vectors composing the family
            IList     staying; //!< indices of available vectores
            Subspace *next;    //!< for list
            Subspace *prev;    //!< for lust


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

