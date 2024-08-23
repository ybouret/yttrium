
#include "y/chemical/plexus.hpp"

#include "y/sort/heap.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;


namespace Yttrium
{
    namespace Chemical
    {


        class Fixed
        {
        public:
            typedef CxxSeries<Fixed,XMemory> Series;

            //! initialize with no gain
            Fixed(XWritable & _cc, const Conservation::Law &_cl) noexcept :
            gg(),
            cc(_cc),
            cl(_cl)
            {
            }

            //! duplicate
            Fixed(const Fixed &_) noexcept :
            gg(_.gg),
            cc(_.cc),
            cl(_.cl)
            {
            }


            //! cleanup
            ~Fixed() noexcept {}


            bool still(const XReadable &C,
                       const Level      L,
                       XAdd            &xadd)
            {
                return cl.broken(gg,cc,SubLevel,C,L,xadd);
            }

            static int Compare(const Fixed &lhs, const Fixed &rhs) noexcept
            {
                return Comparison::Decreasing(lhs.gg, rhs.gg);
            }

            friend std::ostream & operator<<( std::ostream &os, const Fixed &self)
            {
                os << std::setw(15) << real_t(self.gg) << " @" << self.cl << " -> ";
                self.cl.displayCompact(os,self.cc, SubLevel);
                return os;
            }

            xreal_t                 gg; //!< gain
            XWritable &             cc; //!< persistent fixed concentration
            const Conservation::Law &cl;

        private:
            Y_DISABLE_ASSIGN(Fixed);
        };

        class Warden
        {
        public:

            typedef Conservation::Laws::Group Group;
            typedef Conservation::LNode       LNode;

            explicit Warden(const Cluster &cluster) :
            mine(cluster),
            head( mine.laws.isValid() ? mine.laws->groups.head : 0),
            rows( 0!=head ? mine.laws->maxGroupSize : 0 ),
            cols( 0!=head ? mine.species.size       : 0 ),
            xadd( cols ),
            conc( rows, cols),
            jail( rows )
            {

            }

            void run(XWritable &C, const Level L, XMLog &xml)
            {
                Y_XML_SECTION(xml, "Warden");

                for(const Group *g=head;g;g=g->next)
                    run(*g,C,L,xml);

            }


            virtual ~Warden() noexcept
            {

            }


            const Cluster      &mine;  //!< my cluster
            const Group * const head;  //!< first group
            const size_t        rows;  //!< laws max group size
            const size_t        cols;  //!< max species in sub-level
            XAdd                xadd;  //!< for internal computations
            XMatrix             conc;  //!< workspace for concentrations
            Fixed::Series       jail;  //!< fixed

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Warden);

            void run(const Group &group,
                     XWritable   &C,
                     const Level  L,
                     XMLog       &xml)
            {
                initialize(group,C,L,xml);


                while(jail.size()>0)
                {
                    Y_XML_SECTION_OPT(xml,"Reducing","size='"<<jail.size()<<"'");

                    // sort by decreasing excess
                    HeapSort::Call(jail,Fixed::Compare);
                    if(xml.verbose)
                    {
                        for(size_t i=1;i<jail.size();++i)
                        {
                            Y_XMLOG(xml, "(+) " << jail[i]);
                        }
                        Y_XMLOG(xml, "(*) " << jail.tail());
                    }

                    // optimize (and remove) smallest excess
                    optimize(C,L,jail.tail(),xml);
                    jail.popTail();

                    // check remaining

                    break;
                }

            }

            void optimize(XWritable   &C,
                          const Level  L,
                          const Fixed &F,
                          XMLog       &xml)
            {
                Y_XML_SECTION_OPT(xml,"Optimize","fixed='" << F.cl.name << "'");

            }

            void initialize(const Group &group,
                            XWritable   &C,
                            const Level  L,
                            XMLog       &xml)
            {
                Y_XML_SECTION_OPT(xml, "Initialize", "groupSize='" << group.size << "'");
                jail.free();
                for(const LNode *ln=group.head;ln;ln=ln->next)
                {
                    const Conservation::Law &cl = **ln;
                    const size_t             ii = jail.size()+1;
                    XWritable               &cc = conc[ii];
                    Fixed                    fx(cc,cl);
                    if( fx.still(C,L,xadd) )
                    {
                        jail << fx;
                        Y_XMLOG(xml, "(+) " << fx);
                    }
                }
            }


        };

    }
}

Y_UTEST(plexus)
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
        Warden warden(*cl);
        plexus.conc(C0,0.3,0.5);
        lib(std::cerr << "C0=","\t[",C0,"]");
        warden.run(C0,TopLevel,xml);
        lib(std::cerr << "C1=","\t[",C0,"]");


    }

}
Y_UDONE()

