
//! \file


#ifndef Y_Chemical_Reactor_Included
#define Y_Chemical_Reactor_Included 1


#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/plexus/extents.hpp"




namespace Yttrium
{
    namespace Chemical
    {
#if 0
        class Gain
        {
        public:
            Gain(const xreal_t      _g,
                 const Components & _E,
                 const XReadable  & _c) noexcept :
            g(_g), E(_E), C(_c)
            {
            }

            ~Gain() noexcept {}

            Gain(const Gain &_) noexcept :
            g(_.g), E(_.E), C(_.C)
            {
            }

            Y_OSTREAM_PROTO(Gain);

            const xreal_t     g;
            const Components &E;
            const XReadable  &C;

        private:
            Y_DISABLE_ASSIGN(Gain);
        };

        std::ostream & operator<<(std::ostream &os, const Gain &gain)
        {
            os << std::setw(Restartable::Width) << gain.g.str() << " @" << gain.E.name << "=";
            gain.E.displayCompact(os,gain.C,SubLevel);
            return os;
        }

        typedef Small::CoopHeavyList<Gain>  GList_;
        typedef GList_::NodeType            GNode;
        typedef GList_::ProxyType           GBank;

        class KList : public ESolo
        {
        public:
            explicit KList() noexcept : ESolo() {}
            virtual ~KList() noexcept {}

            void show(XMLog &xml, const char * const uuid)
            {
                if(!xml.verbose) return;
                Y_XML_SECTION_OPT(xml,"Gain","is " << uuid);
                for(const ENode *gn=head;gn;gn=gn->next)
                {
                    Y_XMLOG(xml, (**gn).name);
                }
            }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(KList);
        };

        class GList : public GList_
        {
        public:
            explicit GList(const GBank &_) noexcept : GList_(_) {}
            virtual ~GList() noexcept {}

            void show(XMLog &xml, const char * const uuid)
            {
                if(!xml.verbose) return;
                Y_XML_SECTION_OPT(xml,"Gain","is " << uuid);
                for(const GNode *gn=head;gn;gn=gn->next)
                {
                    Y_XMLOG(xml, **gn);
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GList);
        };

        class Equalizer
        {
        public:
            explicit Equalizer(const Cluster &cls);
            virtual ~Equalizer() noexcept;


            const Cluster &cluster;
            EqzBanks       banks;
            const size_t   nrows;
            const size_t   ncols;
            Extents        extents;
            KList          klist;
            GBank          gbank;
            GList          zgain;
            GList          pgain;
            XMatrix        c_eqz;
            XAdd           xadd;

            void operator()(XMLog &xml, XWritable &C0);

            void analyze(XMLog &xml, const Extents &, const XReadable &C0, const AddressBook * wanders);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
        };


        Equalizer:: Equalizer(const Cluster &cls) :
        cluster(cls),
        banks(),
        nrows(cluster.definite->size),
        ncols(cluster->species->size),
        extents(banks),
        klist(),
        gbank(),
        zgain(gbank),
        pgain(gbank),
        c_eqz(nrows,nrows>0?ncols:nrows),
        xadd()
        {
            gbank->reserve(nrows);
        }

        Equalizer:: ~Equalizer() noexcept
        {
        }


        //! decreasing
        static inline
        SignType CompareGains(const GNode * const lhs,
                              const GNode * const rhs) noexcept
        {
            const Gain &L = **lhs;
            const Gain &R = **rhs;
            switch( Sign::Of(L.g,R.g) )
            {
                case Positive: return Negative;
                case Negative: return Positive;
                case __Zero__: break;
            }
            return Sign::Of(L.E.indx[TopLevel],R.E.indx[TopLevel]);
        }

        void Equalizer:: operator()(XMLog     &xml,
                                    XWritable &C0)
        {
            Y_XML_SECTION(xml, "Equalizer");

            Y_XML_COMMENT(xml,"definite");
            do
            {
                klist.free();
                zgain.free();
                pgain.free();

                for(const ENode *en=cluster.definite->head;en;en=en->next)
                {
                    const Equilibrium &eq   = **en;
                    //Y_XML_SECTION(xml,*eq.name);
                    const Resultant    res  = extents(xml,eq,C0,TopLevel, & cluster.wandering );

                    switch(res)
                    {
                        case Correct: continue;
                        case BadBoth: klist << eq; continue;
                        case BadReac: break;
                        case BadProd: break;
                    }

                    XWritable &cc = c_eqz[zgain.size+pgain.size+1];
                    cluster.gather(cc,C0);
                    const Gain G(extents.generate(xml,xadd, cc, eq, C0, TopLevel, &cluster.wandering),eq,cc);
                    assert(G.g.mantissa>=0);
                    if(G.g.mantissa<=0) zgain << G; else pgain << G;
                }

                {
                    Y_XML_SECTION(xml,"Status");
                    MergeSort::Call(pgain,CompareGains);
                    klist.show(xml, Sign::ToText(Negative) );
                    zgain.show(xml, Sign::ToText(__Zero__) );
                    pgain.show(xml, Sign::ToText(Positive) );
                }
                if(pgain.size)
                {
                    const Gain &winner = **pgain.head;
                    cluster.expand(C0,winner.C);
                }
            } while(pgain.size>0);
            cluster.show(std::cerr, TopLevel, "[", C0, "]", xreal_t::ToString) << std::endl;

            if(klist.size||zgain.size) throw Exception("Equalize: not finished");

        }

#endif



    }
}


#endif
