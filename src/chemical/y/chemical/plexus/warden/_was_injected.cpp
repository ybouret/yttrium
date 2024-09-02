#include "y/chemical/plexus/warden.hpp"
#include "y/sort/heap.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        const Conservation::Law * Warden:: wasInjected(const Group &G,
                                                       XWritable   &C,
                                                       const Level  L,
                                                       XMLog       &xml)
        {
            
            //--------------------------------------------------------------
            //
            // initialize all possible fixed in group
            //
            //--------------------------------------------------------------
            if(!broken(G,C,L,xml)) return 0;
            
            //--------------------------------------------------------------
            //
            // iterative reduction
            //
            //--------------------------------------------------------------
            const Conservation::Law *law = 0; assert( jail.size() > 0);
            while(jail.size()>0)
            {
                law = reduce(C,L,xml); assert(0!=law); // record last reduced
                if(jail.size()<=0) return law;         // early return
                update(C,L,xml);
            }
            
            //--------------------------------------------------------------
            //
            // something was modified, return last reduced
            //
            //--------------------------------------------------------------
            assert(0!=law);
            return law;
        }
        
        bool Warden:: broken(const Group &group,
                             XWritable   &C,
                             const Level  L,
                             XMLog       &xml)
        {
            Y_XML_SECTION_OPT(xml, "broken", "group='" << group.size << "'");
            jail.free();
            bool mustFix = false;
            for(const LNode *ln=group.head;ln;ln=ln->next)
            {
                const Conservation::Law &cl = **ln;
                const size_t             ii = jail.size()+1;
                XWritable               &cc = mine.transfer(conc[ii],SubLevel,C,L);
                Fixed                    fx(cc,cl);
                if( fx.still(C,L,xadd) )
                {
                    jail << fx;
                    Y_XMLOG(xml, "(+) " << fx);
                    mustFix = true;
                }
            }
            return mustFix;
        }
        
        
        const Conservation::Law * Warden:: reduce(XWritable   &C,
                                                  const Level  L,
                                                  XMLog       &xml)
        {
            Y_XML_SECTION_OPT(xml,"reduce","size='"<<jail.size()<<"'");
            
            //------------------------------------------------------
            // sort by decreasing excess
            //------------------------------------------------------
            HeapSort::Call(jail,Fixed::Compare);
            if(xml.verbose)
            {
                for(size_t i=1;i<jail.size();++i)
                { Y_XMLOG(xml, "(+) " << jail[i]);     }
                { Y_XMLOG(xml, "(*) " << jail.tail()); }
            }
            
            //------------------------------------------------------
            // optimize (and remove) smallest excess
            //------------------------------------------------------
            const Fixed             &fx = jail.tail();
            const Conservation::Law &cl = fx.cl;
            for(const Actor *a=cl->head;a;a=a->next)
            {
                const Species &      sp = a->sp;
                const size_t * const id = sp.indx;
                const size_t         ii = id[SubLevel];
                const size_t         II = id[L];
                const xreal_t        c0 = C[ II ];
                const xreal_t        c1 = fx.cc[ ii ]; assert(c1>=c0);
                cinj[ii] << (c1-c0);
                C[II] = c1;
            }
            jail.popTail();
            return &cl;
        }

        void Warden:: update(const XReadable &C, const Level L, XMLog &xml)
        {
            Y_XML_SECTION_OPT(xml,"update","size='"<<jail.size()<<"'");

            //------------------------------------------------------
            // check remaining with new concentration
            //------------------------------------------------------
            for(size_t i=jail.size();i>0;--i)
            {
                Fixed &fx = jail[i];
                if( fx.still(C,L,xadd) )
                {
                    Y_XMLOG(xml, "(>) " << fx);
                }
                else
                {
                    Y_XMLOG(xml, "(<) " << fx);
                    jail.remove(i);
                }
            }
        }

    }
    
}


