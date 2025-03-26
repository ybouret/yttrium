

#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        ComponentsType::  ComponentsType() : ComponentsDB(), Latchable() {}
        ComponentsType:: ~ComponentsType() noexcept {}

    }

}

namespace Yttrium
{
    namespace Chemical
    {

        const char * SituationText(const Situation st) noexcept
        {
            switch(st)
            {
                case Blocked: return "Blocked";
                case Running: return "Running";
                case Crucial: return "Crucial";
            }
            return Core::Unknown;
        }


        const char * const Components:: Symbol   = "<=>";
        const char         Components:: Separator;

        Y_PROXY_IMPL(Components,db)


        void Components:: latch() noexcept
        {
            db.latch();
            Coerce(reac).latch();
            Coerce(prod).latch();

        }

        Components:: ~Components() noexcept
        {

        }

        Components:: Components(const String * const xname, const size_t i) :
        Indexed(xname,i),
        Proxy<const ComponentsType>(),
        reac(Actor::AsComponentOnly),
        prod(Actor::AsComponentOnly),
        kind(Deserted),
        db()
        {
        }

        void Components:: use(const Role role, const unsigned nu, const Species &sp)
        {

            //------------------------------------------------------------------
            //
            //
            // sanity check
            //
            //
            //------------------------------------------------------------------
            const String &uid = sp.key();
            if( db.latched )     throw Specific::Exception( key().c_str(), "latched while inserting %s '%s'", Component::RoleText(role), uid.c_str());
            if( db.search(uid) ) throw Specific::Exception( key().c_str(), "multiple '%s'", uid.c_str());
            assert(!reac.latched);
            assert(!prod.latched);

            //------------------------------------------------------------------
            //
            //
            // select actors
            //
            //
            //------------------------------------------------------------------
            Actors * pActors = 0;
            switch(role)
            {
                case Reactant: pActors = &Coerce(reac); break;
                case Product:  pActors = &Coerce(prod); break;
            }
            assert(0!=pActors);

            //------------------------------------------------------------------
            //
            //
            // atomic add
            //
            //
            //------------------------------------------------------------------
            Actors &        actors    = *pActors;
            String          savedName = *actors.name;
            const Component component(role, actors(nu,sp) );
            try {
                if(!db.insert(component)) throw Specific::Exception( key().c_str(), "couldn't insert %s '%s'", component.roleText(), uid.c_str() );
            }
            catch(...)
            {
                savedName.swapWith( Coerce(*actors.name) );
                throw;
            }

            //------------------------------------------------------------------
            //
            //
            // update kind
            //
            //
            //------------------------------------------------------------------
            if(reac->size>0)
            {
                if(prod->size>0)
                {
                    Coerce(kind) = Standard;
                }
                else
                {
                    Coerce(kind) = ReacOnly;
                }
            }
            else
            {
                assert(reac->size<=0);
                if(prod->size>0)
                {
                    Coerce(kind) = ProdOnly;
                }
                else
                {
                    Coerce(kind) = Deserted;
                }
            }

        }


        void Components:: use(const Role role, const Species &sp)
        {
            use(role,1,sp);
        }


        std::ostream & operator<<(std::ostream &os, const Components &cm)
        {
            os << cm.name << Components::Separator << cm.reac.name << Components::Symbol << cm.prod.name;
            return os;
        }

        bool Components:: AreConnected(const Components &lhs, const Components &rhs)
        {
            ConstIterator it = lhs->begin();
            for(size_t n=lhs->size();n>0;--n,++it)
            {
                if(rhs->search(it->key)) return true;
            }
            return false;
        }

        void Components:: gatherSpeciesIn(AddressBook &book) const
        {
            for(ConstIterator it=db.begin();it!=db.end();++it)
            {
                book |= (*it)->sp;
            }
        }


        xreal_t Components:: activity(const xreal_t K, XMul &X, const XReadable &C, const Level L) const
        {
            X.free();
            X << K;
            reac.activity(X,C,L);
            const xreal_t lhs = X.product();

            X << 1;
            prod.activity(X,C,L);
            const real_t rhs = X.product();
            return lhs-rhs;
        }


        xreal_t Components:: activityReac(const xreal_t K, XMul &X, const XReadable &C, const Level L, const xreal_t xi) const
        {
            X.free();
            X << K;
            reac.activity(X,C,L,-xi);
            return X.product();
        }

        xreal_t Components:: activityProd(XMul &X, const XReadable &C, const Level L, const xreal_t xi) const
        {
            X.free();
            X << -1.0;
            prod.activity(X,C,L,xi);
            return X.product();
        }


        xreal_t Components:: activity(const xreal_t K, XMul &X, const XReadable &C, const Level L, const xreal_t xi) const
        {
            return activityReac(K,X,C,L,xi)+activityProd(X,C,L,xi);
            X.free();
            X << K;
            reac.activity(X,C,L,-xi);
            const xreal_t lhs = X.product();

            X << 1;
            prod.activity(X,C,L,xi);
            const real_t rhs = X.product();
            return lhs-rhs;
        }

        void Components:: safeMove(XWritable &C, const Level L, const xreal_t xi) const noexcept
        {
            prod.safeMove(C,L,xi);
            reac.safeMove(C,L,-xi);
        }


        xreal_t Components:: affinity(const xreal_t K, XAdd &xadd, const XReadable &C, const Level L) const
        {
            xadd.free();
            xadd << K.log();
            for(const Actor *ac=reac->head;ac;ac=ac->next)
            {
                const xreal_t l = ac->sp(C,L).log();
                xadd.insert(l,ac->nu);
            }
            for(const Actor *ac=prod->head;ac;ac=ac->next)
            {
                const xreal_t l = -ac->sp(C,L).log();
                xadd.insert(l,ac->nu);
            }
            return xadd.sum();
        }



        Situation Components:: situation(const XReadable &C, const Level L) const noexcept
        {

            if( reac.critical(C,L) )
            {
                if(prod.critical(C,L) )
                {
                    // blocked
                    return Blocked;
                }
                else
                {
                    // crucial
                    return Crucial;
                }
            }
            else
            {
                if(prod.critical(C,L) )
                {
                    // crucial
                    return Crucial;
                }
                else
                {
                    // running
                    return Running;
                }
            }

        }


        xreal_t Components:: extent(XAdd &           xadd,
                                    const XReadable &target,
                                    const Level     targetLevel,
                                    const XReadable &source,
                                    const Level      sourceLevel) const
        {
            xadd.free();
            const size_t n = db.size();
            {
                size_t j = n;
                for(Components::ConstIterator it=db.begin();j-- > 0;++it)
                {
                    const Actor   &ac = **it;
                    const Species &sp = ac.sp;
                    const xreal_t  dc = (sp(target,targetLevel) - sp(source,sourceLevel))/ac.xn;
                    xadd << dc;
                }
                assert(xadd.size()==n);
            }

            const xreal_t denom = n;
            const xreal_t xi    = xadd.sum() / denom;
            return xi;
        };


        bool Components:: critical(const XReadable &C, const Level L) const noexcept
        {
            return reac.critical(C,L) || prod.critical(C,L);
        }


        xreal_t Components:: jacobian(XWritable &xjac, const xreal_t K, XAdd &xadd, const XReadable &C, const Level L) const
        {
            assert(!reac.critical(C,L));
            assert(!prod.critical(C,L));
            xadd.free();
            xadd << K.log();
            for(const Actor *ac=reac->head;ac;ac=ac->next)
            {
                const size_t  j = ac->sp.indx[L];
                const xreal_t c = C[j];
                const xreal_t l = c.log();
                xadd.insert(l,ac->nu);
                xjac[j] = ac->xn/c;
            }


            for(const Actor *ac=prod->head;ac;ac=ac->next)
            {
                const size_t  j = ac->sp.indx[L];
                const xreal_t c = C[j];
                const xreal_t l = -c.log();
                xadd.insert(l,ac->nu);
                xjac[j] = -ac->xn/c;
            }
            return xadd.sum();
        }

        static inline
        void decorateArrow(OutputStream &fp,
                           const Actor  * const a,
                           const String * const color)
        {
            fp << '[';
            if(color)
            {
                fp << *color;
            }
            else
            {
                fp << "color=black";
            }
            const unsigned nu = a->nu;
            if(nu>1)
            {
                fp(",label=\"%u\"",nu);
            }
            fp << ']';
        }

        void Components:: viz(OutputStream &fp,
                              const String * const color) const
        {
            Node(fp,this) << '[';
            Label(fp,*name);
            fp << ",shape=box";
            if(color) fp << ',' << *color;
            fp << ",style=bold";
            Endl(fp << ']');

            for(const Actor *a=prod->head;a;a=a->next)
            {
                Arrow(fp,this,&a->sp);
                decorateArrow(fp,a,color);
                Endl(fp);
            }

            for(const Actor *a=reac->head;a;a=a->next)
            {
                Arrow(fp,&a->sp,this);
                decorateArrow(fp,a,color);
                Endl(fp);
            }
        }

    }

}


#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        bool Components:: neutral() const noexcept
        {
            apz sum = 0;

            for(const Actor *a=reac->head;a;a=a->next)
            {
                const apn nu = a->nu;
                const apz z  = a->sp.z;
                sum -= nu * z;
            }

            for(const Actor *a=prod->head;a;a=a->next)
            {
                const apn nu = a->nu;
                const apz z  = a->sp.z;
                sum += nu * z;
            }
            
            return 0 == sum;
        }


    }

}
