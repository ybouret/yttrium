

//! \file

#ifndef Y_WOVEn_Subspaces_Included
#define Y_WOVEn_Subspaces_Included 1

#include "y/woven/subspace.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        //______________________________________________________________________
        //
        //
        //
        //! list of distinct subspaces
        //
        //
        //______________________________________________________________________
        class Subspaces : public Subspace::List
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from rows of vectors, pack multiple values
            template <typename T> inline
            Subspaces(const Matrix<T> &mu, QSurvey *survey) :
            Subspace::List()
            {
                // create all starting vector
                for(size_t ir=mu.rows;ir>0;--ir)
                    pushHead( new Subspace(mu,ir) );

                // remove multiple same vector
                pack();


                // initial survey if any
                if(survey)
                    conductInitial(*survey);


            }

            //! cleanup
            virtual ~Subspaces() noexcept;

            //! display
            Y_OSTREAM_PROTO(Subspaces);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! try to pack
            void pack();

            template <typename T>
            bool generate(const Matrix<T> &mu, QSurvey *survey)
            {
                Subspace::List newGen;
                for(Subspace *sub=head;sub;sub=sub->next)
                {
                    std::cerr << "Expanding " << *sub << std::endl;
                    sub->expand(newGen, mu, survey);
                }
                swapWith(newGen);
                std::cerr << "-- direct" << std::endl;
                std::cerr << *this << std::endl;
                pack();
                std::cerr << "-- compact" << std::endl;
                std::cerr << *this << std::endl;
                return size>0;
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Subspaces);
            void conductInitial(QSurvey &survey) const;            
        };
        

    }

}

#endif
