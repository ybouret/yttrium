//! \file

#ifndef Y_Chemical_Algebra_Included
#define Y_Chemical_Algebra_Included 1

#include "y/container/matrix.hpp"
#include "y/container/cxx/array.hpp"
#include "y/apex/integer.hpp"
#include "y/data/list/cxx.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{

    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Algebraic operations on topology matrix
        //
        //
        //______________________________________________________________________
        struct Algebraic
        {

            typedef CxxArray<int> Coefficients; //!< alias

            //__________________________________________________________________
            //
            //
            //!  Weights and resulting stoichiometric coefficients
            //
            //__________________________________________________________________
            class Weight : public Object, public Coefficients
            {
            public:
                //______________________________________________________________
                //
                // Definition
                //______________________________________________________________
                typedef CxxListOf<Weight> List; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup coefficients from W, compute Nu = W' * Nu
                explicit Weight(const Readable<const apz> &W,
                                const Matrix<int>         &Nu);
                
                //! cleanup
                virtual ~Weight() noexcept;

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                const Coefficients stoi; //!< stoichiometric coefficients
                const size_t       nEqs; //!< involved nEqs
                Weight *           next; //!< for list
                Weight *           prev; //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Weight);
            };


            //__________________________________________________________________
            //
            //! computing conservation matrix
            /**
             \param Q   output conservation of unsigned, non-trivial combinations
             \param Nu  input topology matrix
             \param xml tracing operations
             */
            //__________________________________________________________________
            static void Compute(Matrix<unsigned> &Q, const Matrix<int> &Nu, XMLog &xml);

            //__________________________________________________________________
            //
            //! computing non-trivial combination of equilibria
            /**
             \param W   output list of algebraic weights to make new equilibria
             \param Nu  input topology matrix
             \param xml tracing operations
             \return max |nEqs|
             */
            //__________________________________________________________________
            static size_t Compute(Weight::List     &W, const Matrix<int> &Nu, XMLog &xml);
        };

    }

}

#endif

