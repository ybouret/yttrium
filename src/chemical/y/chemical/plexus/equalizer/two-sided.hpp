//! \file


#ifndef Y_Chemical_Equalizer_TwoSided_Included
#define Y_Chemical_Equalizer_TwoSided_Included 1


#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/plexus/equalizer/extents.hpp"
#include "y/chemical/plexus/equalizer/gain-list.hpp"

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

            class GList : public GainList, public GList_
            {
            public:
                explicit GList(const GBank &_) noexcept : GList_(_) {}
                virtual ~GList() noexcept {}

                virtual void show(XMLog &xml, const char * const uuid) const
                {
                    Show(xml,uuid,*this);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(GList);
            };

            class KList : public GainList, public ESolo
            {
            public:
                explicit KList() noexcept : ESolo() {}
                virtual ~KList() noexcept {}

                virtual void show(XMLog &xml, const char * const uuid) const
                {
                    Show(xml,uuid,*this);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(KList);
            };


            class TwoSided
            {
            public:
                explicit TwoSided(const Cluster               &_cluster,
                                  const Conservation::Canon   &_canon,
                                  const Banks                 &_banks) :
                cluster( _cluster ),
                canon( _canon ),
                probe( _banks),
                nrows( canon.anxious->size ),
                ncols( cluster->species->size),
                gbank(),
                zgain(gbank),
                pgain(gbank),
                klist(),
                c_eqz(nrows,ncols),
                xadd()
                {
                    gbank->reserve(nrows);
                }

                virtual ~TwoSided() noexcept
                {

                }

                void fix(XMLog             &xml,
                         XWritable         &C0,
                         const Level        L0,
                         const AddressBook &vanishing);


                const Cluster &             cluster;
                const Conservation::Canon & canon;
                Extents                     probe;
                const size_t                nrows;
                const size_t                ncols;
                GBank                       gbank;
                GList                       zgain;
                GList                       pgain;
                KList                       klist;
                XMatrix                     c_eqz;
                XAdd                        xadd;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(TwoSided);
            };


         
        }





    }
}


#endif
