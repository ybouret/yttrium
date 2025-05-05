#include "y/chemical/plexus/equalizing/proceeding.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizing
        {

            Proceeding:: ~Proceeding() noexcept
            {
            }


            Proceeding:: Proceeding(const Cluster & cluster,
                                    const Banks   & banks,
                                    const GBank   & gbank) :
            injected(cluster->species->size),
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
                    watchers.pushTail( new Watcher(cluster,canon)              );
                    twoSided.pushTail( new TwoSided(cluster,canon,banks,gbank) );
                }
                assert(watchers.size==twoSided.size);
            }

            const Cluster & Proceeding:: cluster() const noexcept
            {
                return oneSided.cluster;
            }



            void Proceeding:: fix(XMLog     &  xml,
                                  XWritable &  C0)
            {
                Y_XML_SECTION(xml,"Equalizing::Proceeding");
                injected.forEach( & XAdd::free );
                vanishing.free();

                {
                    Y_XML_SECTION(xml,"ForEachWarden");

                    for(Watcher *cw = watchers.head; cw; cw=cw->next)
                    {
                        cw->fix(xml,C0,TopLevel,injected,vanishing);
                    }

                    if(xml.verbose)
                    {
                        vanishing.display<Species>(xml() << "vanishing: ") << std::endl;
                        cluster().show( xml() << "Ceqz=", TopLevel, "\t\t[", C0, "]", xreal_t::ToString) << std::endl;
                    }
                }

                throw Exception("need to check");



                {
                    Y_XML_SECTION(xml,"ApplyTwoSided");
                    for(TwoSided *ts = twoSided.head;ts;ts=ts->next)
                    {
                        ts->fix(xml, C0, TopLevel, injected, vanishing);
                    }
                }



                {
                    Watcher  *cw = watchers.head;
                    TwoSided *ts = twoSided.head;
                    for(size_t i=watchers.size;i>0;--i,cw=cw->next,ts=ts->next)
                    {
                        assert(0!=cw);
                        assert(0!=ts);
                        vanishing.free();
                        cw->fix(xml, C0, TopLevel, injected, vanishing);
                        ts->fix(xml, C0, TopLevel, injected, vanishing);
                    }
                }
                oneSided.fix(xml,C0,TopLevel);
            }


        }

    }

}
