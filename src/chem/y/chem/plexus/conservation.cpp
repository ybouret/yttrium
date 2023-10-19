
#include "y/chem/plexus/conservation.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Conservation:: ~Conservation() noexcept {}

        Conservation:: Conservation() noexcept :
        Object(),
        Proxy<const Actors>(),
        actors(),
        normSq(0),
        nrm2(0),
        next(0),
        prev(0)
        {
        }


        String Conservation:: toString() const
        {
            return "d_(" + actors.toString() + ")";
        }

        std::ostream & operator<<(std::ostream &os, const Conservation &cons)
        {
            return os << cons.toString();
        }



        Conservation::ConstInterface & Conservation:: surrogate() const noexcept
        {
            return actors;
        }

        void Conservation:: add(const unsigned nu, const Species &sp)
        {
            assert(nu!=0);
            for(Actor *a=actors.head;a;a=a->next)
            {
                const Species &mine = a->sp;
                if(&mine == &sp)
                    throw Specific::Exception("Conservation","multiple species '%s'",sp.name.c_str());
            }
            actors.pushTail( new Actor(sp,nu) );
            normSq      += nu*nu;
            Coerce(nrm2) = double(normSq);
        }

        bool Conservation:: linkedTo(const Conservation &other) const noexcept
        {
            for(const Actor *ac=other->head;ac;ac=ac->next)
            {
                if( actors.has(ac->sp) ) return true;
            }
            return false;
        }


        xreal Conservation:: excess(const Readable<xreal> &C0, XAdd &xadd) const
        {
            xadd.free();
            for(const Actor *ac=actors.head;ac;ac=ac->next)
            {
                const Species &sp = ac->sp;
                xadd.insert( C0[sp.indx[TopLevel]] * ac->xn );
            }
            return xadd.sum();
        }

        const char * const Conservation:: SchemeId = "set19";

        void Conservation:: lnk(OutputStream &fp, const Actor *h, const Actor *t, const size_t indx) const
        {
            Arrow(fp, &(h->sp), &(t->sp));
            fp << '[';
            fp << "dir=both";
            fp << ", arrowhead=odot";
            fp << ", arrowtail=odot";
            fp << ", style=\"dashed,bold\"";
            Color(fp<< ", color=", SchemeId, indx);
            fp << ']';
            Endl(fp);
        }


        void Conservation:: viz(OutputStream &fp, const size_t indx) const
        {
            const Actor *h = actors.head; if(!h) return;
        LOOP:
            const Actor * const t = h->next;
            if(!t) goto CYCLIC;
            lnk(fp,h,t,indx);
            h = t;
            goto LOOP;

        CYCLIC:
            if(actors.size>2)
            {
                lnk(fp,actors.tail,actors.head,indx);
            }
        }
    }

}

