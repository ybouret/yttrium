
#include "y/chemical/plexus.hpp"
#include "y/utest/run.hpp"
#include "y/sort/heap.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //__________________________________________________________________
        //
        //
        //! Broken law with context
        //
        //__________________________________________________________________
        class Broken
        {
        public:
            typedef CxxSeries<Broken,XMemory> Series;
            //______________________________________________________________
            //
            // C++
            //______________________________________________________________
            Broken(const Conservation::Law &, XWritable &) noexcept; //!< init
            Broken(const Broken &broken)                   noexcept; //!< copy
            ~Broken()                                      noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Broken);                                 //!< display

            //______________________________________________________________
            //
            // Methods
            //______________________________________________________________

            //! compare by gain
            static int Compare(const Broken &, const Broken &) noexcept;

            //! compute for law
            bool       still(const XReadable &Ctop, XAdd &xadd);

            //______________________________________________________________
            //
            // Members
            //______________________________________________________________
            xreal_t                  gain; //!< current gain
            const Conservation::Law &claw; //!< studied conservation
            XWritable               &cfix; //!< persistent local fixed

        private:
            Y_DISABLE_ASSIGN(Broken);
        };

        Broken:: Broken(const Conservation::Law &l,
                        XWritable               &c) noexcept :
        gain(0),
        claw(l),
        cfix(c)
        {
        }

        Broken::  Broken(const Broken &b) noexcept :
        gain(b.gain),
        claw(b.claw),
        cfix(b.cfix)
        {

        }

        Broken:: ~Broken() noexcept {}

        int  Broken:: Compare(const Broken &lhs, const Broken &rhs) noexcept
        {
            return Comparison::Decreasing(lhs.gain, rhs.gain);
            //return Comparison::Increasing(lhs.gain, rhs.gain);
        }

        std::ostream & operator<<(std::ostream &os, const  Broken &self)
        {
            const xreal_t zero;
            os << real_t(self.gain) << " @" << (self.claw);
            if(self.gain>zero) { os << "->"; self.claw.displayCompact(os,self.cfix,SubLevel); }
            return os;
        }


        bool  Broken:: still(const XReadable &Ctop, XAdd &xadd)
        {
            return claw.broken(gain, cfix, SubLevel, Ctop, TopLevel, xadd);
        }


        class Janitor
        {
        public:
            typedef Conservation::Laws::Group Group;

            explicit Janitor(const Cluster &cl);
            virtual ~Janitor() noexcept;

            void prolog() noexcept;

            void process(XWritable  &Ctop,
                         XMLog      &xml)
            {
                Y_XML_SECTION(xml, "Janitor");
                for(const Group *g=init;g;g=g->next)
                    process(*g,Ctop,xml);
            }

            void display(std::ostream &os, const Library &lib) const
            {
                os << "{" << std::endl;
                for(const SNode *sn = mine.species.head; sn; sn=sn->next)
                {
                    const Species &sp = **sn;
                    lib.pad(os << "d" << sp,sp) << '=' << Cinj[ sp.indx[SubLevel] ] << std::endl;
                }
                os << "}" << std::endl;
            }


            const Cluster    &     mine; //!< persistent cluster
            const Group * const    init; //!< if mine.laws.isValid()
            const size_t           rows; //!< laws max group size
            const size_t           cols; //!< max species in sub-level
            Broken::Series         jail; //!< local array of broken laws
            XMatrix                Cnew; //!< workspace(rows,cols) to store Csub
            CxxArray<XAdd,XMemory> Cinj; //!< workspace, store incremental increases
            XAdd                   xadd; //!< for internal additions

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Janitor);
            void  process(const Conservation::Laws::Group &grp,
                          XWritable                       &Ctop,
                          XMLog                           &xml);
        };

        Janitor:: Janitor(const Cluster &cl) :
        mine(cl),
        init(mine.laws.isValid() ? mine.laws->groups.head : 0),
        rows(init ? mine.laws->maxGroupSize : 0),
        cols(init ? mine.species.size       : 0),
        jail(rows),
        Cnew(rows,cols),
        Cinj(cols),
        xadd(cols)
        {
        }


        Janitor:: ~Janitor() noexcept
        {
        }


        void Janitor:: prolog() noexcept
        {
            for(size_t i=cols;i>0;--i) Cinj[i].free();
        }


        void  Janitor:: process(const Conservation::Laws::Group &grp,
                                XWritable                       &Ctop,
                                XMLog                           &xml)
        {
            Y_XML_SECTION_OPT(xml, "Group", "size='" << grp.size << "'");

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
                        const XReadable         &cfix = best.cfix;
                        for(const Actor *a=claw->head;a;a=a->next)
                        {
                            const size_t   isub = a->sp.indx[SubLevel];
                            const size_t   itop = a->sp.indx[TopLevel];
                            const xreal_t  Cnew = cfix[ isub ];
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


using namespace Yttrium;
using namespace Chemical;


Y_UTEST(janitor)
{
    Plexus plexus(true);

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << plexus.lib << std::endl;
    std::cerr << "eqs=" << plexus.eqs << std::endl;

    const Library   &lib = plexus.lib;
    Clusters        &cls = plexus.assemble();
    XMLog           &xml = plexus.xml;
    //const XReadable &K   = cls.K(0);


    XVector C0(lib->size(),0);
    XVector dC(C0);



    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        plexus.conc(C0,0.3,0.5);
        Janitor janitor(*cl);
        lib(std::cerr << "C0=","\t[",C0,"]");
        janitor.prolog();
        janitor.process(C0,xml);
        lib(std::cerr << "C1=","\t[",C0,"]");
        janitor.display(std::cerr, lib);

    }

}
Y_UDONE()
