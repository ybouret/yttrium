//! \file


#ifndef Y_Chemical_Equalizer_TwoSided_Included
#define Y_Chemical_Equalizer_TwoSided_Included 1


#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/plexus/equalizer/extents.hpp"
#include "y/chemical/plexus/equalizer/two-sided/gain-list.hpp"

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

          

            typedef Small::CoopHeavyList<Gain>  GList_;
            typedef GList_::NodeType            GNode;
            typedef GList_::ProxyType           GBank;

            class GList :  public GainList, public GList_
            {
            public:
                typedef AutoPtr<GList> Pointer;
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
                typedef AutoPtr<KList> Pointer;

                explicit KList() noexcept : ESolo() {}
                virtual ~KList() noexcept {}

                virtual void show(XMLog &xml, const char * const uuid) const
                {
                    Show(xml,uuid,*this);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(KList);
            };


            class TwoSided : public Quantized
            {
            public:
                typedef CxxListOf<TwoSided> List;
                
                explicit TwoSided(const Cluster               &_cluster,
                                  const Conservation::Canon   &_canon,
                                  const Banks                 &_banks) :
                cluster( _cluster ),
                canon( _canon ),
                probe( _banks),
                nrows( canon.anxious->size ),
                ncols( cluster->species->size),
                gbank(),
                zgain( new GList(gbank) ),
                pgain( new GList(gbank) ),
                klist( new KList()      ),
                c_eqz(nrows,ncols),
                xadd(),
                next(0),
                prev(0)
                {
                    gbank->reserve(nrows);
                }

                virtual ~TwoSided() noexcept
                {

                }

                void fix(XMLog             &xml,
                         XWritable         &C0,
                         Summator          &I0,
                         const Level        L0,
                         const AddressBook &vanishing);


                const Cluster &             cluster;
                const Conservation::Canon & canon;
                Extents                     probe;
                const size_t                nrows;
                const size_t                ncols;
                GBank                       gbank;
                GList::Pointer              zgain;
                GList::Pointer              pgain;
                KList::Pointer              klist;
                XMatrix                     c_eqz;
                XAdd                        xadd;
                TwoSided *                  next;
                TwoSided *                  prev;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(TwoSided);
            };


         
        }





    }
}


#endif
