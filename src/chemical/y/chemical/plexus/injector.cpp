#include "y/chemical/plexus/injector.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Injector:: Broken:: Broken(const Conservation::Law &l,
                                   XWritable               &c) noexcept :
        gain(0),
        claw(l),
        Csub(c)
        {
        }

        Injector:: Broken::  Broken(const Broken &b) noexcept :
        gain(b.gain),
        claw(b.claw),
        Csub(b.Csub)
        {

        }

        Injector::Broken:: ~Broken() noexcept {}
        
        int Injector::Broken:: Compare(const Broken &lhs, const Broken &rhs) noexcept
        {
            return Comparison::Decreasing(lhs.gain, rhs.gain);
            //return Comparison::Increasing(lhs.gain, rhs.gain);
        }

        std::ostream & operator<<(std::ostream &os, const Injector::Broken &self)
        {
            const xreal_t zero;
            os << real_t(self.gain) << " @" << (self.claw);
            if(self.gain>zero) { os << "->"; self.claw.displayCompact(os,self.Csub,SubLevel); }
            return os;
        }


        bool Injector::Broken:: still(const XReadable &Ctop, XAdd &xadd)
        {
            return claw.broken(gain, Csub, SubLevel, Ctop, TopLevel, xadd);
        }
    }
}


namespace Yttrium
{
    namespace Chemical
    {
        Injector:: Injector(const Clusters &cls) :
        used(cls.maxLPG>0),
        rows( used ? cls.maxLPG : 0),
        cols( used ? cls.maxSPC : 0),
        jail( rows ),
        Cnew( rows, cols),
        Cinj( used ? cls.species.size : 0 ),
        xadd(),
        zero()
        {
        }

        Injector:: ~Injector() noexcept
        {
        }

        void Injector:: operator()(const Clusters  &cls,
                                   XWritable       &Ctop,
                                   XWritable       &dTop,
                                   XMLog           &xml)
        {
            dTop.ld(zero);
            for(const Cluster *cl = cls->head;cl;cl=cl->next)
            {
                process(*cl,Ctop,dTop,xml);
            }
        }

        void Injector:: process(const Cluster  &cl,
                                XWritable      &Ctop,
                                XWritable      &dTop,
                                XMLog          &xml)
        {
            Y_XML_SECTION(xml,"Injector");

            //------------------------------------------------------------------
            //
            //
            // clean all injected
            //
            //------------------------------------------------------------------
            for(size_t i=cols;i>0;--i) Cinj[i].free();


            const bool hasLaws = cl.laws.isValid();

            Y_XML_SECTION_OPT(xml, "Cluster", " laws='" << (hasLaws?cl.laws->size:0) << "'");
            if(!hasLaws) return;

            //------------------------------------------------------------------
            //
            // process all laws per group
            //
            //------------------------------------------------------------------
            for(const Conservation::Laws::Group *g=cl.laws->groups.head;g;g=g->next)
                process(*g,Ctop,xml);

            //------------------------------------------------------------------
            //
            // setup increase
            //
            //------------------------------------------------------------------
            for(const SNode *sn=cl.species.head;sn;sn=sn->next)
            {
                const Species &s = **sn;
                const size_t   i = s.indx[TopLevel];
                const size_t   j = s.indx[SubLevel];
                cl.uuid.lj(std::cerr,s.name) << " = " << Cinj[j] << std::endl;
                dTop[i] = Cinj[j].sum();
            }



        }



        void Injector:: process(const Conservation::Laws::Group &grp,
                                XWritable                       &Ctop,
                                XMLog                           &xml)
        {
            Y_XML_SECTION_OPT(xml, "Group", " size='" << grp.size << "'");


            //__________________________________________________________________
            //
            //
            // initialize with all broken laws
            //
            //__________________________________________________________________
            jail.free();
            {
                Y_XML_SECTION(xml, "Initialize");
                for(Conservation::LNode *ln=grp.head;ln;ln=ln->next)
                {
                    const Conservation::Law &claw = **ln;
                    Broken                   broken(claw,Cnew[jail.size()+1]);
                    if(broken.still(Ctop,xadd))
                    {
                        Y_XMLOG(xml," (+) " << broken);
                        jail << broken;
                    }
                }
            }

            //__________________________________________________________________
            //
            //
            // iterative reduction
            //
            //__________________________________________________________________
            while(jail.size()>0)
            {
                {
                    Y_XML_SECTION_OPT(xml,"Reduction"," broken='" << jail.size() << "'");

                    //__________________________________________________________
                    //
                    // order by increasing gain
                    //__________________________________________________________
                    HeapSort::Call(jail,Broken::Compare);
                    for(size_t i=1;i<jail.size();++i) { Y_XMLOG(xml," (+) " << jail[i]); }

                    //__________________________________________________________
                    //
                    // fixed most broken
                    //__________________________________________________________
                    {
                        const Broken & best = jail.tail();
                        Y_XMLOG(xml," (*) " << best);
                        const Conservation::Law &claw = best.claw;
                        const XReadable         &Csub = best.Csub;
                        for(const Actor *a=claw->head;a;a=a->next)
                        {
                            const size_t   isub = a->sp.indx[SubLevel];
                            const size_t   itop = a->sp.indx[TopLevel];
                            const xreal_t  Cnew = Csub[ isub ];
                            xreal_t       &Cold = Ctop[ itop ];
                            assert(Cnew>=Cold);
                            Cinj[isub] << (Cnew-Cold); // store  difference
                            Cold = Cnew;               // update concentration
                        }
                    }

                    //__________________________________________________________
                    //
                    // remove it
                    //__________________________________________________________
                    jail.popTail();
                    if(jail.size()<=0) break;
                }


                //__________________________________________________________________
                //
                //
                // then update and keep/drop remaining broken
                //
                //__________________________________________________________________
                {
                    Y_XML_SECTION_OPT(xml,"Upgrading"," broken='" << jail.size() << "'");
                    for(size_t i=jail.size();i>0;--i)
                    {
                        if( jail[i].still(Ctop,xadd) )
                        {
                            Y_XMLOG(xml, " (+) " << jail[i]);
                        }
                        else
                        {
                            Y_XMLOG(xml, " (-) " << jail[i]);
                            jail.remove(i);
                        }
                    }
                }
            }

        }

    }

}

