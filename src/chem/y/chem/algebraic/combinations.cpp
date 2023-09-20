
#include "y/chem/algebraic.hpp"
#include "y/system/exception.hpp"
#include "y/apex/mylar.hpp"
#include "y/woven/subspaces.hpp"
#include "y/woven/survey/integer.hpp"

namespace Yttrium
{

    namespace Chemical
    {
        void Algebraic:: Compute(Weight::List &wl, const Matrix<int> &Nu, XMLog &xml)
        {
            Y_XML_SECTION(xml, "Algebraic::ComputeCombinations");

            //------------------------------------------------------------------
            // initialize lists
            //------------------------------------------------------------------
            wl.release();
            WOVEn:: IntegerSurvey survey(xml);

            {
                Y_XML_SECTION(xml,"Exploring");
                const Matrix<int> NuT(TransposeOf,Nu);
                const Matrix<int> NuTx;
                Apex::Mylar::Compress(Coerce(NuTx),NuT);
                WOVEn:: Explore(NuTx,survey,false,xml);
                Y_XMLOG(xml," (*) #combination = " << survey.size);
            }

            if(survey.size)
            {
                Y_XML_SECTION(xml,"Processing");
                survey.sort();
                WOVEn::Indices incoming(Nu.cols);
                WOVEn::Indices outgoing(Nu.cols);

                for(const WOVEn::IntegerArray *node=survey.head;node;node=node->next)
                {
                    const WOVEn::IntegerArray & w = *node;              // survey result
                    AutoPtr<Weight>             W = new Weight(w,Nu);   // weight for reactions
                    //std::cerr << W << " -> " << W->stoi << std::endl;

                    //----------------------------------------------------------
                    // checking species
                    //----------------------------------------------------------
                    outgoing.free();
                    incoming.free();

                    //----------------------------------------------------------
                    // outgoing species by new stoichiometry
                    //----------------------------------------------------------
                    outgoing.record(W->stoi);

                    //----------------------------------------------------------
                    // incoming species by presence in original reactions
                    //----------------------------------------------------------
                    for(size_t i=Nu.rows;i>0;--i)
                    {
                        if( (*W)[i]!=0 ) incoming.record( Nu[i] );
                    }
                    //std::cerr << "|_incoming : " << incoming << std::endl;
                    //std::cerr << "|_outgoing : " << outgoing << std::endl;

                    //----------------------------------------------------------
                    // checking consistency
                    //----------------------------------------------------------
                    const size_t numOutgoing = outgoing.size();
                    const size_t numIncoming = incoming.size();

                    switch( Sign::Of(numOutgoing,numIncoming) )
                    {
                        case Negative: assert(numOutgoing<numIncoming); break;
                        case __Zero__:
                            if( outgoing != incoming )
                                throw Specific::Exception("Chemical::Algebraic::Weight","corrupted topology (same components)");
                            goto DONE; // no gain

                        case Positive:
                            throw Specific::Exception("Chemical::Algebraic::Weight","corrupted topology (extra components)");
                    }
                    incoming ^= outgoing;
                    //std::cerr << "|_missing  : " << incoming << std::endl;


                    //----------------------------------------------------------
                    // cheching multiplicity
                    //----------------------------------------------------------
                    for(const Weight *rhs=wl.head;rhs;rhs=rhs->next)
                    {
                        assert(*W != *rhs);
                        if(W->stoi == rhs->stoi)
                        {
                            std::cerr << "*** multiple stoi" << std::endl;
                            goto DONE;
                        }
                    }

                    //----------------------------------------------------------
                    // cheching meaningfull
                    //----------------------------------------------------------


                    Y_XMLOG(xml, " (+) " << W << " -> " << W->stoi <<", missing=" << incoming);

                    wl.pushTail( W.yield() );
                DONE:
                    ;
                }
                Y_XMLOG(xml," (*) #combination = " << wl.size << " / " << survey.size);

            }
        }
    }

}

