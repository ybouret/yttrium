#include "y/chemical/plexus/equalizer/proceeding.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {

            Proceeding:: ~Proceeding() noexcept
            {
            }


            Proceeding:: Proceeding(const Cluster & cluster,
                                    const Banks   & banks) :
            watchers(),
            twoSided(),
            oneSided(cluster,banks),
            vanishing(),
            next(0),
            prev(0)
            {
                for(const Conservation::Canon *pc=cluster.canons.head;pc;pc=pc->next)
                {
                    const Conservation::Canon &canon = *pc;
                    watchers.pushTail( new Watcher(cluster,canon)        );
                    twoSided.pushTail( new TwoSided(cluster,canon,banks) );
                }
                assert(watchers.size==twoSided.size);
            }

            void Proceeding:: session(XMLog     &  xml,
                                      XWritable &  C0,
                                      Summator  &  I0)
            {
                I0.forEach( & XAdd::free );
                {
                    Watcher  *cw = watchers.head;
                    TwoSided *ts = twoSided.head;
                    for(size_t i=watchers.size;i>0;--i,cw=cw->next,ts=ts->next)
                    {
                        assert(0!=cw);
                        assert(0!=ts);
                        vanishing.free();
                        cw->fix(xml, C0, I0, TopLevel, vanishing);
                        ts->fix(xml, C0, I0, TopLevel, vanishing);
                    }
                }
                oneSided.fix(xml,C0,TopLevel);

            }


        }

    }

}
