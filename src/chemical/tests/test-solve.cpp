
#include "y/chemical/plexus/solver/simplex.hpp"
#include "y/chemical/plexus/solver/applicant.hpp"

#include "y/chemical/plexus/clusters.hpp"

#include "y/chemical/weasel/compiler.hpp"
#include "y/random/park-miller.hpp"
#include "y/utest/run.hpp"

#include "y/chemical/reactive/aftermath.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"
#include "y/orthogonal/family.hpp"


#include "y/mkl/opt/minimize.hpp"
#include "y/stream/libc/output.hpp"

#include "y/mkl/algebra/lu.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        using namespace MKL;
        
       


        class Normalizer : public Quantized, public Counted
        {
        public:
            typedef LittleEndianKey     KeyType;
            typedef LittleEndianAddress KeyType_;

            typedef ArkPtr<KeyType,Normalizer> Ptr;
            typedef SuffixSet<KeyType,Ptr>     Set;

            explicit Normalizer(const Cluster &cl) :
            rcl(cl),
            lek(rcl),
            nsp(cl.species.size),
            afm(),
            ceq(cl.size,nsp),
            aps(cl.size),
            obj(cl.size),
            Cin(nsp,0),
            Cex(nsp,0),
            Cws(nsp,0),
            dof(cl.Nu.rows),
            bnk(),
            apl(bnk),
            qfm(nsp,cl.size),
            sim(cl.size,nsp),
            pro(nsp,nsp),
            psf(1),
            Phi(dof),
            XNu(dof),
            Chi(dof),
            Lhs(dof)
            {
                // building auxiliary matrices
                for(size_t i=1;i<=dof;++i)
                {
                    Phi.grow(i,nsp);
                    XNu.grow(i,nsp);
                    Chi.grow(i,i);
                    Lhs.grow(i);
                }

                // create projection matrix
                setProj();
            }

            virtual ~Normalizer() noexcept
            {
            }

            const KeyType & key() const noexcept { return lek; }

            void run(XWritable       & Ctop,
                     const XReadable & Ktop,
                     XMLog           & xml)
            {
                assert(rcl.size        == ceq.rows);
                assert(rcl.species.size== ceq.cols);
                Y_XML_SECTION_OPT(xml, "Normalizer ", " size='" << rcl.size << "' species='" << rcl.species.size << "'");
                bool    repl = false;
                size_t  nmax = compile(Ctop, Ktop, repl, xml); if(nmax<=0) { Y_XMLOG(xml, "[Jammed!]"); return; }
                xreal_t ham0 = overall(Ctop, repl, xml);
                Y_XMLOG(xml, "found " << real_t(ham0));



                for(size_t iter=0;iter<1;++iter)
                {
                    nmax = compile(Ctop, Ktop, repl, xml); if(nmax<=0) { Y_XMLOG(xml, "[Jammed!]"); return; }
                    const xreal_t ham1= overall(Ctop, repl, xml);
                    Y_XMLOG(xml, "found " << real_t(ham1));
                }

                NDSolve(Ctop, Ktop, xml);

                return;

            }


            xreal_t  operator()(const xreal_t u)
            {
                const xreal_t one = 1;
                const xreal_t v   = one - u;

                for(size_t j=nsp;j>0;--j)
                {
                    const xreal_t c0   = Cin[j];
                    const xreal_t c1   = Cex[j];
                    xreal_t       cmin = c0;
                    xreal_t       cmax = c1;
                    if(cmax<cmin) Swap(cmin,cmax);
                    Cws[j] = Clamp(cmin,v*c0+u*c1,cmax);
                }

                return objectiveFunction(Cws,SubLevel);
            }




            const Cluster &    rcl; //!< reference to cluster
            const KeyType_     lek; //!< key
            const size_t       nsp; //!< number of species
            Aftermath          afm; //!< aftermath
            XMatrix            ceq; //!< storage of phase space
            Applicant::Series  aps; //!< applicant store
            CxxSeries<xreal_t> obj; //!< for objective function
            CxxArray<xreal_t>  Cin; //!< input  phase space
            CxxArray<xreal_t>  Cex; //!< output  phase space
            CxxArray<xreal_t>  Cws; //!< working phase space
            const size_t       dof; //!< primary eqs
            ABank              bnk; //!< pool of applicants
            AList              apl; //!< applicant list
            Orthogonal::Family qfm; //!< orthogonal family
            Simplex            sim; //!< simplex
            const XMatrix      pro; //!< projection matrix
            const xreal_t      psf; //!< projection scaling factor
            CxxSeries<XMatrix> Phi; //!< matrices phi
            CxxSeries<XMatrix> XNu; //!< matrices Nu
            CxxSeries<XMatrix> Chi; //!< matrices chi
            CxxSeries<XArray>  Lhs; //!< arrays

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Normalizer);

            xreal_t objectiveFunction(const XReadable &C, const Level L)
            {
                obj.free();
                for(const ANode *an=apl.head;an;an=an->next)
                    obj << (**an).affinity(afm.xmul,C,L);
                return afm.xadd.normOf(obj);
            }

            //! assuming C0 is ok, fine tune C1
            void project(XWritable        &C1,
                         const Level       L1,
                         const XReadable  &C0,
                         const Level       L0)
            {
                for(const SNode *sn = rcl.species.head;sn;sn=sn->next)
                {
                    const size_t * const indx = (**sn).indx;
                    Cin[ indx[SubLevel] ] = C1[ indx[L1] ] - C0[ indx[L0] ];
                }

                XAdd &       xadd = afm.xadd;
                const size_t m    = nsp;

                xadd.make(m);
                for(size_t j=m;j>0;--j)
                {
                    assert(xadd.isEmpty());
                    for(size_t k=m;k>0;--k)
                    {
                        const xreal_t p = pro[j][k] * Cin[k];
                        xadd << p;
                    }
                    Cex[j] = xadd.sum()/psf;
                }


                for(const SNode *sn = rcl.species.head;sn;sn=sn->next)
                {
                    const size_t * const indx = (**sn).indx;
                    C1[ indx[L1] ] = Cex[indx[SubLevel]] + C0[ indx[L0] ];
                }

            }

            void setProj( )
            {
                const size_t n = rcl.Nu.rows;
                const size_t m = nsp;
                const Matrix<int> &Nu = rcl.Nu;
                Matrix<apq> Nu2(n,n);
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=i;j<=n;++j)
                    {
                        apq sum = 0;
                        for(size_t k=1;k<=m;++k)
                        {
                            sum += Nu[i][k] * Nu[j][k];
                        }
                        Nu2[i][j] = Nu2[j][i] = sum;
                    }
                }

                MKL::LU<apq> lu(n);
                Matrix<apq>  aNu2(Nu2); if(!lu.build(aNu2)) throw Specific::Exception("todo","corrupted topology");
                apz          dNu2 = lu.determinant(aNu2).numer; assert(dNu2!=0);
                lu.adjoint(aNu2,Nu2);
                Matrix<apz> aNu3(n,m);
                {
                    for(size_t i=1;i<=n;++i)
                    {
                        for(size_t j=1;j<=m;++j)
                        {
                            apz sum  = 0;
                            for(size_t k=1;k<=n;++k)
                            {
                                sum += aNu2[i][k].numer * Nu[k][j];
                            }
                            aNu3[i][j] = sum;
                        }
                    }
                }

                Matrix<apz> P(m,m);
                {
                    apn g = 0;
                    for(size_t i=1;i<=m;++i)
                    {
                        for(size_t j=i;j<=m;++j)
                        {
                            apz sum = 0;
                            for(size_t k=1;k<=n;++k)
                            {
                                sum += Nu[k][i] * aNu3[k][j];
                            }
                            P[i][j] = P[j][i] = sum;
                            {
                                const apn gtmp = apn::GCD(sum.n,dNu2.n);
                                if(g<=0)
                                    g = gtmp;
                                else
                                    g = Min(g,gtmp);
                            }
                        }
                    }

                    if(g>1)
                    {
                        Coerce(dNu2.n) /= g;
                        for(size_t i=1;i<=m;++i)
                        {
                            for(size_t j=1;j<=m;++j)
                            {
                                Coerce(P[i][j].n) /= g;
                            }
                        }
                        std::cerr << "P    = " << P << std::endl;
                        std::cerr << "dNu2 = " << dNu2 << std::endl;
                    }
                }

                for(size_t i=1;i<=m;++i)
                    for(size_t j=1;j<=m;++j)
                        Coerce(pro[i][j]) = P[i][j].cast<long>("projection matrix");
                Coerce(psf) = dNu2.cast<long>("scaling factor");
                std::cerr << "psf=" << psf << std::endl;
                std::cerr << "pro=" << pro << std::endl;

            }

            //! after a successful compilation
            xreal_t overall(XWritable       & Ctop,
                            const bool        repl,
                            XMLog           & xml)
            {
                assert(aps.size()>0);
                const size_t dims = extract(xml);
                switch(dims)
                {
                    case 0:  // shouldn't happen
                        throw Specific::Exception("Normalizer", "no equilbrium");

                    case 1: // use single winner
                        return objectiveFunction(rcl.transfer(Ctop,TopLevel,(**apl.head).cc,SubLevel),TopLevel);

                    default: // will improve with simplex
                        break;
                }

                return improve(Ctop,repl,xml);
            }

            bool NDSolve(XWritable       &Ctop,
                         const XReadable &Ktop,
                         XMLog           &xml)
            {
                Y_XML_SECTION(xml, "NDSolve");

                bool          repl = false;
                const size_t  nmax = compile(Ctop, Ktop, repl, xml);  
                if( nmax <=0) { Y_XMLOG(xml, "[Jammed!]"); return true; }

                if(xml.verbose)
                {
                    for(size_t i=1;i<=nmax;++i)
                    {
                        const Equilibrium &eq = aps[i].eq;
                        rcl.display(std::cerr, eq, Ktop) << std::endl;
                    }
                }

                const size_t  n = extract(xml);
                const size_t  m = nsp;

                // act on n!

                XMatrix &Nu  = XNu[n];
                XMatrix &phi = Phi[n];
                XMatrix &chi = Chi[n];
                XArray  &lhs = Lhs[n];

                // fill jacobian and lhs
                {
                    size_t ii=0;
                    for(const ANode *an=apl.head;an;an=an->next)
                    {
                        ++ii;
                        const Applicant   &app = **an;
                        const Equilibrium &eq  =  app.eq;
                        const xreal_t      eK  =  app.eK;
                        lhs[ii] = -eq.massAction(eK, afm.xmul, Ctop, TopLevel);
                        eq.topology(Nu[ii], SubLevel);
                        eq.drvsMassAction(eK, phi[ii], SubLevel, Ctop, TopLevel, afm.xmul);
                    }
                }


                // compute phi*Nu'
                XAdd &xadd = afm.xadd;
                xadd.make(n);
                for(size_t i=n;i>0;--i)
                {
                    for(size_t j=n;j>0;--j)
                    {
                        assert(xadd.isEmpty());
                        for(size_t k=m;k>0;--k)
                        {
                            const xreal_t p = phi[i][k] * Nu[j][k];
                            xadd << p;
                        }
                        chi[i][j] = xadd.sum();
                    }
                }

                MKL::LU<xreal_t> lu(n);


                std::cerr << "phi = " << phi << std::endl;
                std::cerr << "Nu  = " << Nu << std::endl;
                std::cerr << "lhs = " << lhs << std::endl;
                std::cerr << "chi = " << chi << std::endl;

                if(!lu.build(chi))
                {
                    Y_XMLOG(xml, "singular");
                    return false;
                }


                // compute extent in lhs
                lu.solve(chi,lhs);
                std::cerr << "xi=" << lhs << std::endl;
                rcl.transfer(Cin, SubLevel, Ctop, TopLevel);

                assert(xadd.isEmpty());
                bool            cut = false;
                xreal_t         fac = 1;
                const   xreal_t zero;

                for(size_t j=m;j>0;--j)
                {
                    for(size_t k=n;k>0;--k)
                    {
                        const xreal_t p = Nu[k][j] * lhs[k];
                        xadd << p;
                    }
                    const xreal_t dC = Cws[j] = xadd.sum();
                    if(dC<zero)
                    {
                        const xreal_t scale = Cin[j]/(-dC);
                        if(scale<fac)
                        {
                            cut = true;
                            fac = scale;
                        }
                    }
                }

                std::cerr << "C0  = " << Cin << std::endl;
                std::cerr << "dC  = " << Cws << std::endl;
                std::cerr << "cut = " << cut << std::endl;
                std::cerr << "fac = " << fac << std::endl;


                if(cut) {
                    fac /= 2;
                    for(size_t j=m;j>0;--j)
                    {
                        Cex[j] = Cin[j] + fac * Cws[j];
                    }
                }
                else
                {
                    for(size_t j=m;j>0;--j)
                    {
                        Cex[j] = Cin[j] + Cws[j];
                    }
                }


                std::cerr << "C1  = " << Cex << std::endl;
                for(const ANode *an=apl.head;an;an=an->next)
                {
                    const Equilibrium &eq = (**an).eq;
                    if(eq.reac.deficient(Cex, SubLevel)) throw Specific::Exception(eq.name.c_str(),"deficient in reactant(s)");
                    if(eq.prod.deficient(Cex, SubLevel)) throw Specific::Exception(eq.name.c_str(),"deficient in product(s)");
                }

                Triplet<xreal_t> xx = { 0, -1, 1 };
                Triplet<xreal_t> ff = { (*this)(xx.a), -1, (*this)(xx.c) };
                const xreal_t    f0 = ff.a;

                std::cerr << "xx=" << xx << ", ff=" << ff << std::endl;


                {
                    OutputFile fp("ndsolve.dat");
                    const size_t np(1000);
                    for(size_t j=0;j<=np;++j)
                    {
                        const real_t u = real_t(j)/np;
                        fp("%.15g %.15g\n", u,  (real_t( (*this)(u) ) ) );
                    }
                    fp << "\n";
                }

                const xreal_t u_opt = Minimize<xreal_t>::Locate(Minimizing::Inside, *this, xx, ff);
                const xreal_t cost  = (*this)(u_opt);
                Y_XMLOG(xml, "affinity= " << real_t(f0) << " -> " << real_t(cost) );
                rcl.transfer(Ctop, TopLevel, Cws, SubLevel);

                return f0 < cost;
            }

            //! improve by simplex lookUp
            xreal_t  improve(XWritable       & Ctop,
                             const bool        repl,
                             XMLog           & xml)
            {
                Y_XML_SECTION(xml, "Improve");
                assert(apl.size>0);

                {
                    Y_XML_SECTION(xml, "Simplex");
                    // load simplex
                    sim.free();
                    if(!repl)
                    {
                        const xreal_t val = sim.load( objectiveFunction(Ctop, TopLevel), rcl.species, Ctop, TopLevel).cost;
                        Y_XMLOG(xml, std::setw(15) << real_t(val) << " @Ctop");
                    }
                    else
                    {
                        Y_XMLOG(xml, "[replaced Ctop]");
                    }

                    for(const ANode *an=apl.head;an;an=an->next)
                    {
                        const Applicant &app = **an;
                        const xreal_t    val = sim.load( objectiveFunction(app.cc, SubLevel), rcl.species, app.cc, SubLevel).cost;
                        Y_XMLOG(xml,std::setw(15) << real_t(val) << " @" << app.eq);
                    }

                }

                {
                    Y_XML_SECTION(xml, "Search");
                    unsigned long cycle = 0;
                    const size_t  m     = nsp;
                    OutputFile fp("objective.dat");

                    while(sim.size>1)
                    {
                        ++cycle;
                        Y_XMLOG(xml, "#cycle = " << cycle);
                        Triplet<xreal_t> xx = {  0, -1,  1 };
                        Triplet<xreal_t> ff = { -1, -1, -1 };

                        // extract upper/lower and load Cin/Cex
                        {
                            AutoPtr<Vertex> upper = sim.query();
                            AutoPtr<Vertex> lower = sim.query();

                            ff.a = upper->cost;
                            ff.c = lower->cost;

                            for(size_t j=m;j>0;--j)
                            {
                                Cin[j] = (*upper)[j];
                                Cex[j] = (*lower)[j];
                            }
                            sim.free( upper.yield() );
                            sim.free( lower.yield() );
                        }

                        //Y_XMLOG(xml, "upper = " << std::setw(15) << real_t(ff.a) );
                        //Y_XMLOG(xml, "lower = " << std::setw(15) << real_t(ff.c) );


                        {
                            const real_t offset = cycle-1;
                            const size_t np(1000);
                            for(size_t j=0;j<=np;++j)
                            {
                                const real_t u = real_t(j)/np;
                                fp("%.15g %.15g\n", offset+u,  (real_t( (*this)(u) ) ) );
                            }
                            fp << "\n";
                        }

                        const xreal_t u_opt = Minimize<xreal_t>::Locate(Minimizing::Inside, *this, xx, ff);
                        const xreal_t cost  = (*this)(u_opt);
                        sim.load(cost,rcl.species,Cws,SubLevel);
                        Y_XMLOG(xml, "optim = " << std::setw(15) << real_t(cost)  <<  " @" << real_t(u_opt) );
                    }

                }



                rcl.transfer(Ctop, TopLevel, Cws, SubLevel);

                return sim.head->cost;
            }

            static SignType CompareBasis(const ANode * const lhs, const ANode * const rhs) noexcept
            {
                return Sign::Of( (**lhs).eq.indx[TopLevel], (**rhs).eq.indx[TopLevel] );
            }

            size_t   extract(XMLog &xml)
            {
                Y_XML_SECTION(xml, "Extract");

                //--------------------------------------------------------------
                //
                // reset applicant/ortho family
                //
                //--------------------------------------------------------------
                apl.free();
                qfm.free();

                //--------------------------------------------------------------
                //
                // loop over applicants
                //
                //--------------------------------------------------------------
                const size_t nap = aps.size();
                for(size_t i=1;i<=nap;++i)
                {
                    const Applicant &app = aps[i];
                    if( qfm.wouldAccept( rcl.topology[ app.isub() ] ) )
                    {
                        apl << app;
                        qfm.expand();
                        if( apl.size >= dof) break; // base is filled
                    }
                }

                //--------------------------------------------------------------
                //
                // found the base: order by equilibria top-level
                // to build local topology
                //
                //--------------------------------------------------------------
                MergeSort::Call(apl,CompareBasis);




                //--------------------------------------------------------------
                //
                // summary
                //
                //--------------------------------------------------------------
                assert(apl.size <= Min(dof,nap) );
                Y_XMLOG(xml, "#applicant  = " << aps.size());
                Y_XMLOG(xml, "#primary    = " << dof);
                Y_XMLOG(xml, "#family     = " << apl.size);

#if 0
                {
                    XMatrix &Nu = XNu[apl.size];
                    size_t   ii = 0;
                    for(const ANode *an=apl.head;an;an=an->next)
                    {
                        const Applicant &app = **an;
                        app.eq.topology(Nu[++ii],SubLevel);
                        if(xml.verbose)
                        {
                            app.display( xml() << "| ", cl.uuid, false) << std::endl;
                        }
                    }
                    Y_XMLOG(xml, "Nu=" << Nu);
                }
#endif

                //--------------------------------------------------------------
                //
                // return apl.size <= min(dof,nap)
                //
                //--------------------------------------------------------------
                return apl.size;
            }



            size_t compile(XWritable       & Ctop,
                           const XReadable & Ktop,
                           bool            & repl,
                           XMLog           &  xml)
            {
                Y_XML_SECTION(xml, "Compile");

                // examine all equilibria
                repl        = false;
                size_t iter = 0;
            EXAMINE:
                ++iter;
                Y_XMLOG(xml, "[Examine] [#iter=" << iter << "]");
                aps.free();
                for(const ENode *en=rcl.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                    const size_t       ii = aps.size()+1;
                    XWritable         &cc = rcl.transfer(ceq[ii], SubLevel, Ctop, TopLevel);
                    const Situation    st = afm.seek(cc, SubLevel, eq, eK);

                    switch(st)
                    {
                        case Blocked:
                            Y_XMLOG(xml, "[Blocked] " << eq.name);
                            continue;

                        case Crucial:
                            Y_XMLOG(xml, "[Crucial] " << eq.name);
                            rcl.transfer(Ctop, TopLevel, cc, SubLevel);
                            repl = true;
                            goto EXAMINE;

                        case Running:
                            Y_XMLOG(xml, "[Running] " << eq.name);
                            break;
                    }

                    assert(Running==st);
                    const Applicant app(eq,eK,cc, afm.eval(cc, SubLevel, Ctop, TopLevel, eq) );
                    aps << app;
                }

                // sorting and cross-checking
                HeapSort::Call(aps, Applicant::Compare);
                const size_t nmax = aps.size();
                for(size_t i=1;i<=nmax;++i)
                {
                    const Applicant   &lhs = aps[i]; if(xml.verbose)  lhs.display( xml() << "| ", rcl.uuid, true) << std::endl;
                    const Equilibrium &lEq = lhs.eq;
                    const char * const lid = lEq.name.c_str();

                    if( !lEq.reac.accounted(Ctop, TopLevel) ) throw Specific::Exception(lid, "missing reactant(s) on top-level");
                    if( !lEq.prod.accounted(Ctop, TopLevel) ) throw Specific::Exception(lid, "missing product(s) on top-level");

                    for(size_t j=1;j<=nmax;++j)
                    {
                        const Applicant   &rhs = aps[j];
                        const char * const rid = rhs.eq.name.c_str();
                        if(!lEq.reac.accounted(rhs.cc,SubLevel)) throw Specific::Exception(lid, "missing reactant(s) in '%s'", rid);
                        if(!lEq.prod.accounted(rhs.cc,SubLevel)) throw Specific::Exception(lid, "missing reactant(s) in '%s'", rid);
                    }
                }

                return nmax;
            }

        };


        class Solver : public Proxy<const Normalizer::Set>
        {
        public:
            static const char * const CallSign;
            typedef Normalizer::Set::Iterator      Iterator;
            typedef Normalizer::Set::ConstIterator ConstIterator;

            explicit Solver(const SharedClusters &usr) :
            cls(usr),
            ndb()
            {
                for(const Cluster *cl=(*cls)->head;cl;cl=cl->next)
                {
                    const Normalizer::Ptr ptr = new Normalizer(*cl);
                    if(!ndb.insert(ptr)) throw Specific::Exception(CallSign, "corrupted cluster address");
                }
            }

            virtual ~Solver() noexcept { }

            void run(XWritable       & Ctop,
                     const XReadable & Ktop,
                     XMLog           &  xml)
            {
                size_t n = ndb.size();
                for(Iterator it=ndb.begin();n>0;--n,++it)
                {
                    Normalizer &normalizer = **it; assert( (*cls)->owns( & normalizer.rcl ) );
                    normalizer.run(Ctop,Ktop,xml);
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            SharedClusters  cls;
            Normalizer::Set ndb;

            virtual ConstInterface & surrogate() const noexcept
            {
                return ndb;
            }
        };


        const char * const Solver:: CallSign = "Chemical::Solver";
    }
}



using namespace Yttrium;
using namespace Chemical;





Y_UTEST(solve)
{

    Weasel::Compiler  &weasel = Weasel::Compiler::Instance();
    Library            lib;
    LuaEquilibria      eqs;
    Random::ParkMiller ran;

    for(int i=1;i<argc;++i)
    {
        weasel(lib,eqs,Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool           verbose = true;
    XMLog          xml(verbose);
    SharedClusters   cls = new Clusters(eqs,xml);
    const XReadable &K   = cls->K(0);


    XVector C0(lib->size(),0);

    Solver  solver(cls);


    for(size_t iter=0;iter<1;++iter)
    {
        Species::Conc(C0,ran,0.3);

        //C0.ld(0);

        lib(std::cerr << "C0=","\t[",C0,"]");
        solver.run(C0,K, xml);
        lib(std::cerr << "C1=","\t[",C0,"]");



    }

    Y_SIZEOF(Normalizer);
    Y_SIZEOF(Clusters);
    
}
Y_UDONE()
