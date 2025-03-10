
#include "y/lingo/pattern/logic.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Logic:: ~Logic() noexcept
        {
        }


        Logic:: Logic(const uint32_t t) noexcept :
        Pattern(t),
        Pattern::List()
        {
        }

        Logic:: Logic(const Logic &logic) :
        Pattern(logic),
        Pattern::List(logic)
        {
        }

        


        size_t Logic:: serialize(OutputStream &fp) const
        {
            size_t ans = emitUUID(fp);
            ans += fp.emitVBR(size);
            for(const Pattern *p=head;p;p=p->next)
                ans += p->serialize(fp);
            return ans;
        }

        Logic & Logic:: operator<<(Pattern * const p) noexcept
        {
            assert(0!=p);
            pushTail(p);
            return *this;
        }

        void Logic:: vizLink(OutputStream &fp) const
        {
            for(const Pattern *node=head;node;node=node->next)
            {
                Endl(Arrow(fp,this,node));
            }
        }

        bool Logic:: HaveSameContent(const Logic &lhs, const Logic &rhs)
        {
            if(lhs.size != rhs.size) return false;
            for(const Pattern *l=lhs.head, *r=rhs.head;l;l=l->next,r=r->next)
            {
                assert(0!=l);
                assert(0!=r);
                if( !Equality(*l,*r) )
                    return false;
            }
            return true;
        }

        void Logic:: optimizing()
        {
            Patterns store;
            while(size>0)
            {
                assert(0!=head);
                store.pushTail( Pattern::Optimize( popHead() ) );
            }
            swapWith(store);
        }

        void Logic:: ignoreCase()
        {
            Patterns store;
            while( size> 0 )
                store.pushTail( IgnoreCase( popHead() ) );
            swapWith(store);
        }

        void Logic:: noMultiple()
        {
            Patterns store;
            while(size>0)
            {
                AutoPtr<Pattern> lhs   = popHead();
                bool             found = false;
                for(const Pattern *rhs = store.head; rhs; rhs=rhs->next)
                {
                    if( *lhs == *rhs ) {
                        found = true;
                        break;
                    }
                }
                if(found) continue;
                store.pushTail( lhs.yield() );
            }
            swapWith(store);
        }

        static const char msg[] = "empty for ";
        
        void Logic:: query(CharDB &firstChars)  const
        {
            if(size<=0) throw Specific::Exception(callSign(), "%squery()",msg);
            _query(firstChars);
        }

        bool Logic::  strong()  const
        {
            if(size<=0) throw Specific::Exception(callSign(), "%sstrong()",msg);
            return _strong();
        }

        String Logic:: regularExpression() const
        {
            if(size<=0) throw Specific::Exception(callSign(), "%sregularExpression()",msg);
            return _regexp();
        }

        bool Logic:: univocal() const
        {
            if(size<=0) throw Specific::Exception(callSign(), "%sunivocal()",msg);
            return _univocal();
        }

        bool Logic:: takes(Y_Lingo_Pattern_Args) const
        {
            if(size<=0) throw Specific::Exception(callSign(), "%sunivocal()",msg);
            return _takes(token,source,0);
        }

    }

}


#include "y/lingo/pattern/basic/byte.hpp"
#include "y/lingo/pattern/basic/lump.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Logic & Logic:: add(const uint8_t single)
        {
            pushTail( new Byte(single) );
            return *this;
        }

        Logic & Logic:: add(const uint8_t lower, const uint8_t upper)
        {
            pushTail( new Lump(lower,upper) );
            return *this;
        }
    }

}


