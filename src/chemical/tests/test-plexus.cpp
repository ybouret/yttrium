
#include "y/chemical/plexus/reactors.hpp"
#include "y/chemical/plexus/conservation/wardens.hpp"

#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/random/mt19937.hpp"
#include "y/stream/libc/output.hpp"

#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Restartable
        {
        public:
            static const unsigned      Width = 22;
            static const char * const None;

        protected:
            explicit Restartable() noexcept;

        public:
            virtual ~Restartable() noexcept;

            virtual void restart() noexcept = 0;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Restartable);
        };

        const char * const Restartable:: None = "<none>";

        Restartable:: Restartable() noexcept
        {}

        Restartable:: ~Restartable() noexcept
        {}


        class Boundary : public SRepo, public Restartable
        {
        public:
            explicit Boundary(const SBank &sb) noexcept :
            SRepo(sb),
            xi(0)
            {
            }

            virtual ~Boundary() noexcept {}

            Y_OSTREAM_PROTO(Boundary);

            void operator()(const Species &sp, const xreal_t xx)
            {
                if(size<=0)
                {
                    xi = xx;
                    (*this) << sp;
                }
                else
                {
                    switch(Sign::Of(xx,xi))
                    {
                        case Negative: // new winner
                            xi = xx; free(); (*this) << sp;
                            break;
                            
                        case __Zero__: // ex-aequo
                            (*this) << sp;
                            break;

                        case Positive: // discard
                            break;
                    }
                }
            }




            virtual void restart() noexcept { free(); xi=0; }

            xreal_t xi;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boundary);
        };

        std::ostream & operator<<(std::ostream &os, const Boundary &b)
        {
            if(b.size)
            {
                os << std::setw(Restartable::Width) << b.xi.str() << "@" << (const SRepo &)b;
            }
            else
                os << std::setw(Restartable::Width) << Restartable::None;
            return os;
        }

        class Cursor : public SRepo
        {
        public:
            explicit Cursor(const SBank   &sb,
                            const Species &sp,
                            const xreal_t  xx) :
            SRepo(sb),
            xi(xx)
            {
                (*this) << sp;
            }
            Y_OSTREAM_PROTO(Cursor);


            Cursor(const Cursor &_) : SRepo(_), xi(_.xi) {}

            virtual ~Cursor() noexcept {}

            const xreal_t xi;

        private:
            Y_DISABLE_ASSIGN(Cursor);
        };

        std::ostream & operator<<(std::ostream &os, const Cursor &cr)
        {
            os << std::setw(Restartable::Width) << cr.xi.str() << "@" << (const SRepo &)cr;
            return os;
        }

        typedef Small::CoopHeavyList<Cursor> CrList;
        typedef CrList::NodeType             CrNode;
        typedef CrList::ProxyType            CrBank;

        class EqzBanks
        {
        public:
            explicit EqzBanks();
            virtual ~EqzBanks() noexcept;

            SBank  sb;
            CrBank cb;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(EqzBanks);
        };

        EqzBanks:: EqzBanks() : sb(), cb() {}

        EqzBanks:: ~EqzBanks() noexcept {}

        class Cursors : public Proxy<const CrList>, public Restartable
        {
        public:
            explicit Cursors(const EqzBanks &banks) noexcept;
            virtual ~Cursors() noexcept;
            virtual void restart() noexcept;

            void operator()(const Species &sp, const xreal_t xi);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cursors);
            Y_PROXY_DECL();
            CrNode *crNode(const Species &sp, const xreal_t &xi)
            {
                const Cursor cr(sb,sp,xi);
                return my.proxy->produce(cr);
            }
            CrList my;
            SBank  sb;
        };

        Y_PROXY_IMPL(Cursors,my);

        Cursors:: Cursors(const EqzBanks &banks) noexcept :
        Proxy<const CrList>(),
        my(banks.cb),
        sb(banks.sb)
        {

        }

        Cursors:: ~Cursors() noexcept
        {

        }

        void Cursors:: restart() noexcept { my.free(); }


        void Cursors:: operator()(const Species &sp, const xreal_t xi)
        {
            switch(my.size)
            {
                case 0: my.pushTail( crNode(sp,xi) ); return;
                case 1:
                    switch( Sign::Of(xi, (**my.head).xi) )
                    {
                        case __Zero__: (**my.head) << sp;            return;
                        case Negative: my.pushHead( crNode(sp,xi) ); break;
                        case Positive: my.pushTail( crNode(sp,xi) ); break;
                    }
                    assert(2==my.size);
                    return;
            }
        }



        class Extent : public Restartable
        {
        public:
            static const char * const Limiting;
            static const char * const Required;

            explicit Extent(const EqzBanks &banks) noexcept;
            virtual ~Extent() noexcept;

            virtual void restart() noexcept;

            void operator()(const Actors &      A,
                            const XReadable &   C,
                            const Level         L,
                            const AddressBook * const wanders)
            {
                restart();
                for(const Actor *a=A->head;a;a=a->next)
                {
                    const Species &sp = a->sp; if(wanders && wanders->has(sp)) continue;
                    const xreal_t  cc = sp(C,L);
                    if(cc>=0.0)
                    {
                        limiting(sp,cc/a->xn);
                    }
                    else
                    {
                        required(sp,(-cc)/a->xn);
                    }
                }
            }

            Y_OSTREAM_PROTO(Extent);


            Boundary limiting;
            Cursors  required;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Extent);
        };

        const char * const Extent:: Limiting = "limiting";
        const char * const Extent:: Required = "required";


        Extent:: ~Extent() noexcept
        {
        }

        Extent:: Extent(const EqzBanks &banks) noexcept :
        Restartable(),
        limiting(banks.sb),
        required(banks)
        {
        }

        void Extent:: restart() noexcept { limiting.restart(); required.restart(); }

        std::ostream & operator<<(std::ostream &os, const Extent &ext)
        {
            os << std::endl;
            os << "\t" << Extent::Limiting << "=" << ext.limiting << std::endl;
            if(ext.required->size>0)
            {
                for(const CrNode *cn=ext.required->head;cn;cn=cn->next)
                {
                    os << "\t" << Extent::Required << "=" << **cn;
                    if(cn!=ext.required->tail) os << std::endl;
                }
            }
            else
            {
                os << "\t" << Extent::Required << "=" << std::setw(Restartable::Width) << Restartable::None;
            }
            return os;
        }


        class Extents : public Restartable
        {
        public:
            explicit Extents(const EqzBanks &banks) noexcept;
            virtual ~Extents() noexcept;


            virtual void restart() noexcept;

            void operator()(const Components &  E,
                            const XReadable &   C,
                            const Level         L,
                            const AddressBook * const wanders)
            {
                reac(E.reac,C,L,wanders);
                prod(E.prod,C,L,wanders);
            }



            Extent reac;
            Extent prod;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Extents);
        };

        Extents:: Extents(const EqzBanks &banks) noexcept :
        reac(banks),
        prod(banks)
        {
        }

        Extents:: ~Extents() noexcept
        {
        }

        void Extents:: restart() noexcept { reac.restart(); prod.restart(); }




        class Equalizer
        {
        public:
            explicit Equalizer(const Cluster &cls);
            virtual ~Equalizer() noexcept;
            typedef CxxArray<Extents,MemoryModel> Table;


            const Cluster &cluster;
            EqzBanks       banks;
            Table          definite;

            void operator()(XMLog &xml, XWritable &C0);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
        };


        Equalizer:: Equalizer(const Cluster &cls) :
        cluster(cls),
        banks(),
        definite(cluster.definite->size,CopyOf,banks)
        {
            assert(definite.size() == cluster.definite->size );
        }

        Equalizer:: ~Equalizer() noexcept
        {
        }

        void Equalizer:: operator()(XMLog &xml, XWritable &C0)
        {
            Y_XML_SECTION(xml, "Equalizer");

            Y_XML_COMMENT(xml,"definite");
            for(const ENode *en=cluster.definite->head;en;en=en->next)
            {
                const Equilibrium &eq   = **en;
                Y_XML_SECTION(xml,*eq.name);
                Extents           &exts = eq(definite,AuxLevel);
                exts(eq,C0,TopLevel, & cluster.wandering );
                Y_XMLOG(xml, "reactants :" << exts.reac);
                Y_XMLOG(xml, "products  :" << exts.prod);
            }
        }




    }
}

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(plexus)
{

    Random::MT19937 ran;
    Weasel &weasel = Weasel::Instance();
    weasel << "function f(t) return 1.1 end";

    Library    lib;
    Equilibria eqs;

    for(int iarg=1;iarg<argc;++iarg)
    {
        const String data = argv[iarg];
        weasel(lib,eqs,Lingo::Module::Open(data,data) );
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool verbose = true;

    XMLog          xml(verbose);
    Clusters       cls(xml,eqs,0.0);

    std::cerr << "lib=" << lib << std::endl;
    cls.graphViz("cs");

    const size_t m = lib->size();
    XVector      C0(m,0); // concentration
    XVector      I0(m,0); // injection
    XVector      C(m,0);



    Library::Concentrations(C0,ran,0.5,0.5);
    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Equalizer eqz(*cl);
        eqz(xml,C0);
    }


    return 0;

    Conservation::Wardens wardens(cls);
    wardens(xml,C0,I0);

    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;
    lib.show(std::cerr << "I0=", "\t[", I0, "]", xreal_t::ToString ) << std::endl;

    return 0;

    Reactor::MonitorScore = true;
    Reactor::EmitProfiles = true;
    Reactors cs(cls);
    cs(xml,C0);




    
    Y_SIZEOF(Cluster);
    Y_SIZEOF(ClusterContent);

    Y_SIZEOF(Conservation::Law);
    Y_SIZEOF(Conservation::Laws);
    Y_SIZEOF(Conservation::Canon);
    Y_SIZEOF(Conservation::Canons);

    Y_SIZEOF(Reactor);


}
Y_UDONE()

