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

        typedef Apex::Ortho::Vector                QVector;  //!< alias
        typedef Apex::Ortho::Family                QFamily;  //!< alias
        typedef Functor<void,TL1(const QVector &)> QSurvey;  //!< alias
        typedef Apex::Ortho::Metrics               QMetrics; //!< alias
        typedef Apex::Ortho::Quality               Quality;  //!< alias




        //______________________________________________________________________
        //
        //
        //
        //! a SubSpace is a qfamily with its indices and remaining indices
        //
        //
        //______________________________________________________________________
        class SubSpace : public Object, public QFamily
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const   CallSign; //!< Subspace
            typedef CxxListOf<SubSpace> List;     //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            /**
             \param mu matrices : cols=space dimensions, rows= degrees of freedom
             \param ir row index to start with, other are stored in staying
             */
            template <typename T>
            explicit SubSpace(const Matrix<T> &mu,
                              const size_t     ir) :
            Object(),
            QFamily(  CheckDims(mu) ),
            indices(  CheckDOFs(mu) ),
            staying(  mu.rows       ),
            quality( getQuality(1)  ),
            next(0),
            prev(0)
            {
                if( !wouldAccept(mu[ir]) ) SingularFirstRowException();
                initWith(ir,mu.rows);
            }

            //! copy
            SubSpace(const SubSpace &);

            //! cleanup
            virtual ~SubSpace() noexcept;

            //! display
            Y_OSTREAM_PROTO(SubSpace);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! check if source was merged with this
            /**
             - indices can contain source indices
             - qfamily can contain source qfamily
             */
            //__________________________________________________________________
            bool merged( AutoPtr<SubSpace> &source );

            //__________________________________________________________________
            //
            //! try to expand current subspace dimension
            /**
             - a new subspace is built for all possible
             staying indices that would increase the dimension
             - survey is conducted
             - if staying indices, the new subspace is added to list
             */
            //__________________________________________________________________
            template <typename T>
            inline void tryExpand(List &            L,
                                  const Matrix<T> & mu,
                                  QSurvey         * survey)
            {
                switch(quality)
                {
                    case Apex::Ortho::Fragmental:
                        //------------------------------------------------------
                        // need to test all the staying vectors
                        //------------------------------------------------------
                        assert(size<concluding);
                        for(size_t i=staying.size();i>0;--i)
                        {
                            const size_t ir = staying[i];
                            if(wouldAccept(mu[ir]))
                                doExpand(L,ir,survey);
                        }
                        break;

                    case Apex::Ortho::Hyperplane: {
                        //------------------------------------------------------
                        // there is AT MOST ONE new vector
                        //------------------------------------------------------
                        assert(size==concluding);
                        List tmp;
                        for(size_t i=staying.size();i>0;--i)
                        {
                            const size_t ir = staying[i];
                            if(wouldAccept(mu[ir]))
                            {
                                doExpand(tmp,ir,survey);
                                assert(0==tmp.size);
                                return; // stop at first vector, other are the same
                            }
                        }

                    } break;

                    case Apex::Ortho::Generating:
                        //------------------------------------------------------
                        // base is complete, call fullfill for consistency
                        //------------------------------------------------------
                        assert(size==dimensions);
                        fullfill();
                        break;
                }


            }


            //! check dimensions = mu.cols > 0
            static size_t CheckDims(const MatrixMetrics &mu);


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Indices       indices; //!< indices of vectors composing the family
            Indices       staying; //!< indices of available vectors
            const Quality quality; //!< depending on size vs. dims
            SubSpace     *next;    //!< for list
            SubSpace     *prev;    //!< for lust


        private:
            Y_DISABLE_ASSIGN(SubSpace);
            static size_t CheckDOFs(const MatrixMetrics &mu);
            static void   SingularFirstRowException();

            void          initWith(const size_t ir, const size_t nr);
            void          doExpand(List &L, const size_t ir, QSurvey *survey);
            void          fullfill(); //!< staying -> indices

        };

    }

}

#endif

