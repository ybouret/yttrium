
#include "y/chem/algebraic.hpp"
#include "y/mkl/algebra/ortho-space.cpp"
#include "y/system/exception.hpp"
#include "y/woven/subspaces.hpp"
#include "y/woven/survey/natural.hpp"

namespace Yttrium
{

    namespace Chemical
    {


        void Algebraic:: Compute(Matrix<unsigned> &Q, const Matrix<int> &Nu, XMLog &xml)
        {
            Y_XML_SECTION(xml, "Algebraic::ComputeConservations");
            // prepare data
            Q.release();
            WOVEn::NaturalSurvey survey(xml);

            {
                Y_XML_SECTION(xml, "Exploring");
                // make orthospace
                Matrix<apz> O;
                if(!MKL::OrthoSpace::Make(O,Nu))
                    throw Specific::Exception("Chemica::Algebraic::Conservation","Singular Topology");

                // explore all ortho space
                WOVEn::Explore(O,survey,true,xml);
                Y_XMLOG(xml," (*) #conservation = " << survey.size);
            }

            if(survey.size)
            {
                Y_XML_SECTION(xml, "Processing");
                // survey => matrix
                const size_t M  = Nu.cols;
                Q.make(survey.size,M);
                survey.sort();
                {
                    size_t       i = 0;
                    for(const WOVEn::NaturalArray *node=survey.head;node;node=node->next)
                    {
                        const Readable<const apn> &cof = *node;
                        Writable<unsigned>        &Q_i = Q[++i];
                        for(size_t j=M;j>0;--j)
                            Q_i[j] = cof[j].cast<unsigned>("conservation coefficient");
                    }
                }
            }

        }

    }

}
