

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

            //__________________________________________________________________
            //
            //! setup from rows of vectors, pack multiple values
            //__________________________________________________________________
            template <typename T> inline
            Subspaces(const Matrix<T> &mu,
                      QSurvey         *survey,
                      XMLog           &xmlog) :
            Subspace::List(),
            xml(xmlog)
            {
                //--------------------------------------------------------------
                // create all starting vector
                //--------------------------------------------------------------
                Y_XML_SECTION(xml,"Subspaces::Initialize");
                for(size_t ir=mu.rows;ir>0;--ir)
                    pushHead( new Subspace(mu,ir) );

                //--------------------------------------------------------------
                // remove multiple same vector
                //--------------------------------------------------------------
                pack();


                //--------------------------------------------------------------
                // conduct initial survey if any
                //--------------------------------------------------------------
                Y_XMLOG(xml,"-- initial count = " << size);
                
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

            //! try to pack similar subspaces
            void pack();

            //__________________________________________________________________
            //
            //! expand all subspaces then pack
            //__________________________________________________________________
            template <typename T>
            bool generate(const Matrix<T> &mu, QSurvey *survey)
            {
                

                //--------------------------------------------------------------
                // substitute new generation
                //--------------------------------------------------------------
                {
                    Subspace::List newGen;
                    for(Subspace *sub=head;sub;sub=sub->next)
                        sub->expand(newGen, mu, survey);
                    swapWith(newGen);
                }

                //--------------------------------------------------------------
                // cleanup
                //--------------------------------------------------------------
                if(size>0)
                {
                    pack();
                    assert(size>0);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            XMLog &xml;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Subspaces);
            void conductInitial(QSurvey &survey) const;            
        };


        //______________________________________________________________________
        //
        //
        //! conduct survey for full matrix
        //
        //______________________________________________________________________

        template <typename T> inline
        void Explore(const Matrix<T> &mu,
                     QSurvey         &survey,
                     const bool       useTop,
                     XMLog           &xmlog)
        {
            Subspaces working(mu,useTop ? &survey : 0,xmlog);
        CYCLE:
            if( working.generate(mu,&survey) )
                goto CYCLE;
        }

    }

}

#endif
