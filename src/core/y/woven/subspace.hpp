//! \file

#ifndef Y_WOVEn_Subspace_Included
#define Y_WOVEn_Subspace_Included 1

#include "y/woven/indices.hpp"
#include "y/container/matrix.hpp"
#include "y/woven/types.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        typedef Apex::Ortho::Vector                QVector; //!< alias
        typedef Apex::Ortho::Family                QFamily; //!< alias
        typedef Functor<void,TL1(const QVector &)> QSurvey; //!< alias


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
             */
            template <typename T>
            explicit Subspace(const Matrix<T> &mu,
                              const size_t     ir) :
            qfamily( CheckDims(mu) ),
            indices( CheckDOFs(mu) ),
            staying( mu.rows       ),
            next(0),
            prev(0)
            {
                if( !qfamily.wouldAccept(mu[ir]) ) SingularFirstRowException();
                initializeWith(ir,mu.rows);
            }

            //! copy
            Subspace(const Subspace &);

            //! cleanup
            virtual ~Subspace() noexcept;

            //! display
            Y_OSTREAM_PROTO(Subspace);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! check if source was merged with this
            /**
             - indices can contain source indices
             - qfamilu can contain source qfamily
             */
            bool merged( AutoPtr<Subspace> &source );

            //! try to expand current subspace dimension
            /**
             a new subspace is added for all possible
             staying indices that would increase the dimension
             */
            template <typename T>
            inline void expand(List &            L,
                               const Matrix<T> & mu,
                               QSurvey         * survey)
            {
                for(size_t i=staying.size();i>0;--i)
                {
                    const size_t ir = staying[i];
                    if(qfamily.wouldAccept(mu[ir]))
                        expand(L,ir,survey);
                }
            }




            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            QFamily   qfamily; //!< family of orthogonal vectors
            Indices   indices; //!< indices of vectors composing the family
            Indices   staying; //!< indices of available vectores
            Subspace *next;    //!< for list
            Subspace *prev;    //!< for lust


        private:
            Y_DISABLE_ASSIGN(Subspace);
            static size_t CheckDims(const MatrixMetrics &mu);
            static size_t CheckDOFs(const MatrixMetrics &mu);
            static void   SingularFirstRowException();
            void          initializeWith(const size_t ir, const size_t nr);
            void          expand(List &L, const size_t ir, QSurvey *survey);

        };

    }

}

#endif

