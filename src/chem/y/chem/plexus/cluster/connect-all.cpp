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
                                                 const Readable<xreal> &Ksh) :
                Equilibrium(eid),
                coef(),
                allK(Ksh),
                xmul(),
                xone(1)
                {

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
                                  Equilibria            & Eqs,
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
            {
                const size_t nmax = Chemical::Algebraic::Compute(W,Nu,xml);
                Coerce(meg) = new EqGroup(nmax);
            }
            EqGroup &eg = Coerce(*meg);

            //------------------------------------------------------------------
            //
            //
            // initalize local indices
            //
            //
            //------------------------------------------------------------------
            const EqArray &ea = *edb;
            const size_t   N  = ea.size(); assert(N==pre.size);
            for(size_t i=N;i>0;--i)
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
                const String                   eid = MakeName(ea,cof);                     // make the name
                MixedEquilibrium              &eq  = Eqs( new MixedEquilibrium(eid,Ksh) ); // initialize mixed equilibrium
                const Algebraic::Coefficients &st  = w->stoi;                              // get stoichiometry


                //--------------------------------------------------------------
                //
                // populate coefficients wit TopLevel indices to compute K
                //
                //--------------------------------------------------------------
                for(size_t i=1;i<=N;++i)
                {
                    const int nu = cof[i];
                    if(0==nu) continue;;
                    const size_t I = ea[i]->indx[TopLevel];
                    Coerce(eq.coef).pushTail( new MixedEquilibrium::Node(I,nu) );
                }

                assert(eq.coef.size==w->nEqs);

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
                assert(Eqs->size()==eq.indx[TopLevel]);
                assert(w->nEqs<=eg.size());
                Coerce(all)         << eq;
                Coerce(eg[w->nEqs]) << eq;
                Coerce(eq.indx[SubLevel]) = all.size;
            }

            {
                size_t       i = 1;
                for(const EqNode *node=all.head;node;node=node->next,++i)
                {
                    const Equilibrium &eq = **node;
                    if(i!=eq.indx[SubLevel])
                        throw Specific::Exception("Chemical::Cluster", "<%s> bad id!", eq.name.c_str());
                }
            }

            {
                Y_XML_SECTION_OPT(xml,"Hierarchy"," order='" << meg->size() << "'");
                Y_XMLOG(xml,pre);
                for(size_t i=1;i<=meg->size();++i)
                {
                    Y_XMLOG(xml,(*meg)[i]);
                }
            }

        }
    }

}

