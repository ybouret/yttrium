
//! \file


#ifndef Y_Chemical_Equalizer_Included
#define Y_Chemical_Equalizer_Included 1


#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/plexus/equalizer/extents.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {
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

            class GList : public GList_
            {
            public:
                explicit GList(const GBank &_) noexcept : GList_(_) {}
                virtual ~GList() noexcept {}

                void show(XMLog &xml, const char * const uuid)
                {
                    if(xml.verbose)
                    {
                        String title = uuid; title += " Gain";
                        xml.display(title,*this);
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(GList);
            };

            class KList : public ESolo
            {
            public:
                explicit KList() noexcept : ESolo() {}
                virtual ~KList() noexcept {}

                void show(XMLog &xml, const char * const uuid)
                {
                    if(xml.verbose)
                    {
                        String title = uuid; title += " Gain";
                        xml.display(title,*this);
                    }
                }
            private:
                Y_DISABLE_COPY_AND_ASSIGN(KList);
            };


            class Ordinary
            {
            public:
                explicit Ordinary(const Cluster               &_cluster,
                                  const Conservation::Canon   &_canon,
                                  const Banks                 &banks) :
                cluster( _cluster ),
                canon( _canon ),
                extents(banks),
                nrows( canon.anxious->size ),
                ncols( cluster->species->size),
                gbank(),
                zgain(gbank),
                pgain(gbank),
                c_eqz(nrows,ncols),
                xadd()
                {
                    gbank->reserve(nrows);
                }

                virtual ~Ordinary() noexcept
                {

                }

                void fix(XMLog             &xml,
                         XWritable         &C0,
                         const Level        L0,
                         const AddressBook &vanishing);


                const Cluster &             cluster;
                const Conservation::Canon & canon;
                Extents                     extents;
                const size_t                nrows;
                const size_t                ncols;
                GBank                       gbank;
                GList                       zgain;
                GList                       pgain;
                KList                       klist;
                XMatrix                     c_eqz;
                XAdd                        xadd;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ordinary);
            };


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


            void Ordinary:: fix(XMLog             &xml,
                                XWritable         &C0,
                                const Level        L0,
                                const AddressBook &vanishing)
            {

                Y_XML_SECTION(xml,"Equalizer::Ordinary");
                if(xml.verbose)
                {
                    vanishing.display<Species>(xml() << "vanishing: ") << std::endl;
                }


            CYCLE:
                zgain.free();
                pgain.free();
                klist.free();
                for(const ENode *en=cluster.definite->head;en;en=en->next)
                {
                    const Equilibrium &eq   = **en;
                    const Resultant    res  = extents(xml,eq,C0,L0, & cluster.wandering );


                    switch(res)
                    {
                        case Correct: continue;
                        case BadBoth: klist << eq; continue;
                        case BadReac: break;
                        case BadProd: break;
                    }

                    XWritable &cc = c_eqz[zgain.size+pgain.size+1];
                    cluster.transfer(cc,SubLevel,C0,L0);
                    const Gain G(extents.generate(xml,xadd,cc,eq,C0,L0, &cluster.wandering),eq,cc);
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
                    cluster.transfer(C0,L0,winner.C,SubLevel);
                    goto CYCLE;
                }

                cluster.show(std::cerr << "Ceqz=", L0, "[", C0, "]", xreal_t::ToString) << std::endl;

                if(klist.size||zgain.size) throw Exception("Equalize: not finished");


            }

        }





    }
}


#endif
