
#include "y/chemical/plexus.hpp"
#include "y/utest/run.hpp"


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
        class BrokenLaw
        {
        public:
            //______________________________________________________________
            //
            // C++
            //______________________________________________________________
            BrokenLaw(const Conservation::Law &, XWritable &) noexcept; //!< init
            BrokenLaw(const BrokenLaw &broken)                noexcept; //!< copy
            ~BrokenLaw()                                      noexcept; //!< cleanup
            Y_OSTREAM_PROTO(BrokenLaw);                                 //!< display

            //______________________________________________________________
            //
            // Methods
            //______________________________________________________________

            //! compare by gain
            static int Compare(const BrokenLaw &, const BrokenLaw &) noexcept;

            //! compute for law
            bool       still(const XReadable &Ctop, XAdd &xadd);

            //______________________________________________________________
            //
            // Members
            //______________________________________________________________
            xreal_t                  gain; //!< current gain
            const Conservation::Law &claw; //!< studied conservation
            XWritable               &Csub; //!< persistent local fixed

        private:
            Y_DISABLE_ASSIGN(BrokenLaw);
        };

        BrokenLaw:: BrokenLaw(const Conservation::Law &l,
                              XWritable               &c) noexcept :
        gain(0),
        claw(l),
        Csub(c)
        {
        }

        BrokenLaw::  BrokenLaw(const BrokenLaw &b) noexcept :
        gain(b.gain),
        claw(b.claw),
        Csub(b.Csub)
        {

        }

        BrokenLaw:: ~BrokenLaw() noexcept {}

        int  BrokenLaw:: Compare(const BrokenLaw &lhs, const BrokenLaw &rhs) noexcept
        {
            return Comparison::Decreasing(lhs.gain, rhs.gain);
            //return Comparison::Increasing(lhs.gain, rhs.gain);
        }

        std::ostream & operator<<(std::ostream &os, const  BrokenLaw &self)
        {
            const xreal_t zero;
            os << real_t(self.gain) << " @" << (self.claw);
            if(self.gain>zero) { os << "->"; self.claw.displayCompact(os,self.Csub,SubLevel); }
            return os;
        }


        bool  BrokenLaw:: still(const XReadable &Ctop, XAdd &xadd)
        {
            return claw.broken(gain, Csub, SubLevel, Ctop, TopLevel, xadd);
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
        lib(std::cerr << "C0=","\t[",C0,"]");



    }

}
Y_UDONE()
