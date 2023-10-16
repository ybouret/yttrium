#include "y/chem/plexus/cluster.hpp"
#include "y/chem/algebraic.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace
        {

            static inline String FirstCof(const int nu)
            {
                switch(nu)
                {
                    case -1: return String('-');
                    case  1: return String();
                }
                assert(abs(nu)>1);
                return FormatString("%d*",nu);
            }

            static inline String ExtraCof(const int nu)
            {
                switch(nu)
                {
                    case -1: return String('-');
                    case  1: return String('+');
                }
                assert(abs(nu)>1);
                if(nu < -1)
                    return FormatString("%d*",nu);
                assert(nu>1);
                return FormatString("+%d*",nu);
            }

            static inline String MakeName(const EqArray       & eqs,
                                          const Readable<int> & cof)
            {
                assert(eqs.size()>=cof.size());
                String name;
                bool   first = true;
                for(size_t i=1;i<=cof.size();++i)
                {
                    const int nu = cof[i];
                    if(0==nu) continue;
                    if(first)
                    {
                        name += FirstCof(nu);
                        first = false;
                    }
                    else
                    {
                        name += ExtraCof(nu);
                    }
                    name += eqs[i]->name;
                }

                return name;
            }

            class MixedEquilibrium : public Equilibrium
            {
            public:

                //______________________________________________________________
                //
                //
                //! compile node: index+coeff
                //
                //______________________________________________________________
                class Node : public Object
                {
                public:
                    inline explicit Node(const size_t i, const int n) noexcept : 
                    id(i), nu(n),  next(0), prev(0) { assert(nu!=0); }
                    inline virtual ~Node() noexcept {}

                    const size_t id;
                    const int    nu;
                    Node        *next;
                    Node        *prev;
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Node);
                };

                //______________________________________________________________
                //
                //
                //! list of compiled coefficients
                //
                //______________________________________________________________
                typedef CxxListOf<Node> List;


                //______________________________________________________________
                //
                //! initialize
                //______________________________________________________________
                inline explicit MixedEquilibrium(const String          &eid,
                                                 const Readable<int>   &cof,
                                                 const Readable<xreal> &Ksh) :
                Equilibrium(eid),
                coef(),
                allK(Ksh),
                xmul(),
                xone(1)
                {
                    // compile coefficients
                    const size_t N = cof.size();
                    for(size_t i=1;i<=N;++i)
                    {
                        const int n = cof[i];
                        if(n!=0)
                            Coerce(coef).pushTail( new Node(i,n) );
                    }
                    assert(coef.size>1);
                    
                }

                //______________________________________________________________
                //
                //! cleanup
                //______________________________________________________________
                inline virtual ~MixedEquilibrium() noexcept
                {
                }

                const List              coef;
                const Readable<xreal> & allK;
                XMul                    xmul;
                const xreal             xone;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);

                virtual xreal getK(double)
                {
                    xmul.free();
                    for(const Node *node=coef.head;node;node=node->next)
                    {
                        const xreal k = allK[node->id];
                        const int   n = node->nu;
                        if(n<0)
                        {
                            const xreal ik = xone/k;
                            xmul.insert(ik,-n);
                        }
                        else
                        {
                            assert(n>0);
                            xmul.insert(k,n);
                        }
                    }
                    return xmul.product();
                }
            };
        }

        void Cluster:: connectAll(XMLog &                 xml,
                                  Equilibria            & all,
                                  const Readable<xreal> & Ksh)
        {
            Y_XML_SECTION(xml, "ConnectAll");

            //------------------------------------------------------------------
            //
            //
            // calling Algebraic
            //
            //
            //------------------------------------------------------------------
            Chemical::Algebraic::Weight::List W;
            Chemical::Algebraic::Compute(W,Nu,xml);

            //------------------------------------------------------------------
            //
            //
            // initalize local indices
            //
            //
            //------------------------------------------------------------------
            const EqArray &ea = *edb;
            for(size_t i=edb->size();i>0;--i)
                Coerce(ea[i]->indx[SubLevel]) = i;

            //------------------------------------------------------------------
            //
            //
            // append new algebraic/mixed equilibria
            //
            //
            //------------------------------------------------------------------
            const SpArray &sa  = *sdb;
            for(const Algebraic::Weight *w=W.head;w;w=w->next)
            {
                //--------------------------------------------------------------
                //
                // create equilibrium
                //
                //--------------------------------------------------------------
                const Readable<int>           &cof = *w;
                const String                   eid = MakeName(ea,cof);                         // make the name
                MixedEquilibrium              &eq  = all( new MixedEquilibrium(eid,cof,Ksh) ); // initialize mixed equilibrium
                const Algebraic::Coefficients &st  = w->stoi;                                  // get stoichiometry
                //std::cerr << cof << " #" << w->nEqs << " => " << eid << std::endl;

                //--------------------------------------------------------------
                //
                // populate equilibrium
                //
                //--------------------------------------------------------------
                for(size_t i=1;i<=st.size();++i)
                {
                    const int      nu =  st[i]; if(nu==0) continue;
                    const Species &sp = *sa[i]; assert(i==sp.indx[SubLevel]);
                    eq(nu,sp);
                }

                //--------------------------------------------------------------
                //
                // record and update
                //
                //--------------------------------------------------------------
                assert(all->size()==eq.indx[TopLevel]);
                Coerce(eqs) << eq;
                Coerce(eq.indx[SubLevel]) = eqs.size;
            }

            {
                //const size_t N = ea.size();
                size_t       i = 1;
                for(const EqNode *node=eqs.head;node;node=node->next,++i)
                {
                    const Equilibrium &eq = **node;
                    Y_XMLOG(xml, eq.name << " top=" << eq.indx[TopLevel] << ", sub=" << eq.indx[SubLevel]);
                    if(i!=eq.indx[SubLevel])
                        throw Specific::Exception("Chemical::Cluster", "<%s> bad id!", eq.name.c_str());
                }
            }



        }
    }

}

