#include "y/chemical/plexus/solver.hpp"
#include "y/type/temporary.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"

#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Solver:: ~Solver() noexcept {}

        Solver:: Solver(const Clusters &cls) :
        afm(),
        ceq(cls.maxEPC,cls.maxSPC),
        phi(cls.maxSPC),
        obj(cls.maxEPC),
        pps(cls.maxEPC),
        Cin(cls.maxSPC,0),
        Cex(cls.maxSPC,0),
        Cws(cls.maxSPC,0),
        bnk(),
        qdb(),
        //sim(cls.maxEPC,cls.maxSPC),
        ppb(0),
        pcl(0)
        {
            // prepare database of QFamily
            for(const Cluster *cl=cls->head;cl;cl=cl->next)
            {
                qdb(cl->Nu.rows,cl->species.size,bnk);
            }
        }


        xreal_t Solver:: objectiveFunction(const XReadable &C,
                                           const Level      L)
        {
            assert(0!=ppb);
            obj.free();
            const PList &list = **ppb;
            {
                for(const PNode *pn=list.head;pn;pn=pn->next)
                {
                    obj << (**pn).objectiveFunction(C,L,afm.xmul);
                }
            }
            assert(obj.size() == list.size);
            return afm.xadd.normOf(obj);
        }


        xreal_t Solver::  operator()(const xreal_t u)
        {
            assert(0!=pcl);
            const xreal_t one = 1;
            const xreal_t v   = one - u;

            for(size_t j=pcl->species.size;j>0;--j)
            {
                const xreal_t c0 = Cin[j];
                const xreal_t c1 = Cex[j];
                xreal_t cmin  = c0;
                xreal_t cmax  = c1;
                if(cmax<cmin) Swap(cmin,cmax);
                Cws[j] = Clamp(cmin,v*c0+u*c1,cmax);
            }

            return objectiveFunction(Cws,SubLevel);
        }


        size_t Solver:: examine(const Cluster   & cl,
                                XWritable       & Ctop,
                                const XReadable & Ktop,
                                bool            & repl,
                                XMLog           & xml)
        {
            Y_XML_SECTION(xml, "Examine");
            const xreal_t zero;
            const xreal_t mOne(-1);
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            size_t        iter = 0;
            repl               = false;
        EXAMINE:
            ++iter;
            Y_XMLOG(xml, "(*) [Examine] (#iter=" << iter << ")" );
            pps.free();

            //------------------------------------------------------------------
            //
            // Loop over each equilibrium
            //
            //------------------------------------------------------------------
            for(const ENode *en=cl.head;en;en=en->next)
            {
                //----------------------------------------------------------
                //
                // get equilibrium and its aftermath
                //
                //----------------------------------------------------------
                const Equilibrium &eq = **en;                      // equilibrium
                const xreal_t      eK = Ktop[ eq.indx[TopLevel] ]; // current constant
                const size_t       ii = pps.size() + 1;            // destination indeq
                XWritable         &cc = ceq[ii];                   // local solution filled and computed
                const Situation    id = afm.seek( cl.transfer(cc,SubLevel,Ctop,TopLevel), SubLevel, eq, eK);
                switch(id)
                {
                    case Blocked: Y_XMLOG(xml, "(-) [Blocked] " << eq); continue;
                    case Running: Y_XMLOG(xml, "(+) [Running] " << eq); break;
                    case Crucial:
                        Y_XMLOG(xml, "(!) [Crucial] " << eq << " @" << cc);
                        repl = true;
                        cl.transfer(Ctop, TopLevel, cc, SubLevel); // set crucial
                        goto EXAMINE;                              // abort and restart
                }

                //----------------------------------------------------------
                //
                // store new running prospect with final computation
                //
                //----------------------------------------------------------
                assert(Running==id);
                //eq.displayCompact(std::cerr << eq.name << ": cc  = ", cc, SubLevel) << " / " << cc << std::endl;
                //std::cerr << eq.name << ": ma  = " << eq.massAction(eK, afm.xmul, cc, SubLevel) << std::endl;

                eq.drvsMassAction(eK, phi, SubLevel, cc, SubLevel, afm.xmul);
                const xreal_t  slope = eq.dot(phi, SubLevel, afm.xadd);
                if(slope>=zero) 
                {
                    eq.displayCompact(std::cerr << "\tcc  = ", cc, SubLevel) << " / " << cc <<  std::endl;
                    eq.displayCompact(std::cerr << "\tphi = ", phi, SubLevel) << " / " << phi <<  std::endl;
                    eq.displayCompact(std::cerr << "\tnu  = ", cl.topology[eq.indx[SubLevel]], SubLevel) << std::endl;
                    throw Specific::Exception(eq.name.c_str(),"invalid optimal slope");
                }
                const Prospect pro(eq, eK, cc, afm.eval(cc,SubLevel,Ctop,TopLevel,eq), mOne/slope );
                pps << pro;
            }

            HeapSort::Call(pps, Prospect::Compare);
            return pps.size();
        }

        void Solver:: run(const Cluster   &cl,
                          XWritable       &Ctop,
                          const XReadable &Ktop,
                          XMLog           &xml)
        {

            Y_XML_SECTION_OPT(xml, "Solver::Cluster", " size='" << cl.size << "'");
            const size_t ndof = cl.Nu.rows;                      // degrees of freedom
            bool         repl = false;                           // replacement flag
            size_t       npmx = examine(cl,Ctop,Ktop,repl,xml);  // max prospects

            if( npmx <= 0) { Y_XMLOG(xml, "[Jammed!]"); return; }

            if(xml.verbose)
            {
                for(size_t i=1;i<=npmx;++i)
                {
                    const Prospect &pro = pps[i];
                    cl.uuid.pad(xml() << pro.eq,pro.eq);
                    *xml << " | ax =" << std::setw(15) << real_t(pro.xi.abs());
                    pro.eq.displayCompact(*xml << " @", pro.cc, SubLevel);
                    *xml << " ks=" << real_t(pro.ks);
                    *xml << std::endl;
                }
            }

            QBuilder &                 base = qdb[cl.species.size];
            const Temporary<QBuilder*> ppbT(ppb,&base);
            const Temporary<Cluster *> pclT(pcl,&Coerce(cl));

            {
                Y_XML_SECTION(xml, "SubBasis");
                base.init();
                for(size_t i=1;i<=npmx;++i)
                {
                    const Prospect &pro = pps[i];
                    if(base.grow(pro, cl.topology) && base->size >= ndof) break;
                }

                Y_XMLOG(xml, "#dof = " << std::setw(10) << ndof);
                Y_XMLOG(xml, "#pro = " << std::setw(10) << npmx);
                Y_XMLOG(xml, "#vec = " << std::setw(10) << base->size);
            }

            
        }
    }
}
