

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
        class SubSpaces :
        public Identifiable,
        public QMetrics,
        public SubSpace::List
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< WOVEn::SubSpaces

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
            SubSpaces(const Matrix<T> &mu,
                      QSurvey         *survey,
                      XMLog           &xmlog) :
            Identifiable(),
            QMetrics( SubSpace::CheckDims(mu) ),
            SubSpace::List(),
            generation(1),
            xml(xmlog)
            {
                //--------------------------------------------------------------
                // create all starting vector
                //--------------------------------------------------------------
                Y_XML_SECTION(xml,"Subspaces::Initialize");
                for(size_t ir=mu.rows;ir>0;--ir)
                    pushHead( new SubSpace(mu,ir) );

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
            virtual ~SubSpaces() noexcept;

            //! display
            Y_OSTREAM_PROTO(SubSpaces);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            virtual const char * callSign() const noexcept; //!< CallSign
            void                 pack();                    //!< pack similar subspaces

            //__________________________________________________________________
            //
            //! expand all subspaces then pack
            //__________________________________________________________________
            template <typename T>
            bool generate(const Matrix<T> &mu, QSurvey *survey)
            {

                Y_XML_SECTION_OPT(xml,
                                  "Subspaces::Generate",
                                  " generation='" << generation << "'" <<
                                  " dimensions='" << dimensions << "'" <<
                                  " size='" << size << "'");

                //--------------------------------------------------------------
                // substitute new generation
                //--------------------------------------------------------------
                {
                    SubSpace::List newGen;
                    for(SubSpace *sub=head;sub;sub=sub->next)
                        sub->tryExpand(newGen, mu, survey);
                    swapWith(newGen);
                }

                Coerce(generation)++;

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
            const size_t generation; //!< generation in [1..dimensions]
            XMLog       &xml;        //!< output helper

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SubSpaces);
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
            SubSpaces working(mu,useTop ? &survey : 0,xmlog);
        CYCLE:
            if( working.generate(mu,&survey) )
                goto CYCLE;
        }

    }

}

#endif
