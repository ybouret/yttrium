
#include "y/chemical/reactive/equilibrium.hpp"
#include "y/apex/natural.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const String * Equilibrium:: Compile:: Name(const XNode * const eNode)
        {
            assert(0!=eNode);
            assert( eNode->defines<Equilibrium>() );
            const XList &       xlist = eNode->branch(); assert(xlist.size==4);
            const XNode * const label = xlist.head;      assert(label->is(Equilibrium::Label));
            const String        xname = label->lexeme().toString(1,0);
            //std::cerr << "returning '" << xname << "'" << std::endl;
            return new String(xname);
        }


        const String *  Equilibrium:: Compile:: Data(const XNode * const eNode)
        {
            assert(0!=eNode);
            assert( eNode->defines<Equilibrium>() );
            const XList &       xlist = eNode->branch(); assert(xlist.size==4);
            const XNode * const kdata = xlist.tail;      assert(kdata->is("String"));
            const String        sdata = kdata->lexeme().toString();
            return new String(sdata);
        }

        static inline
        void fillActors(Components &  eq,
                        const Role    role,
                        XNode * const aNode,
                        Library &     lib)
        {
            assert( (Reactant==role && aNode->is(Equilibrium::Reac) ) || (Product==role && aNode->is(Equilibrium::Prod)) );

            for(XNode *node=aNode->branch().head;node;node=node->next)
            {
                assert( node->defines<Actor>() );
                XList        &actor =  node->branch();    assert(1==actor.size||2==actor.size);
                const Formula formula( actor.popTail() ); // extract node
                const String  uuid  = formula.uuid();     // make uuid
                const Species &sp   = lib(uuid);          // guess species
                unsigned       nu   = 1;                  // default coefficient
                assert(lib.owns(sp));
                if(actor.size>0)
                {
                    assert(1==actor.size);
                    const apn Nu = actor.head->lexeme().toNatural();
                    if(!Nu.tryCast(nu))
                        throw Specific::Exception(Equilibrium::CallSign, "coefficient overflow for %s '%s'", Component::RoleText(role), uuid.c_str());
                    assert(nu>0);
                }
                assert(nu>0);
                eq.use(role,nu,sp);
            }
        }


        void Equilibrium:: Compile:: Fill(Components &eq, Library &lib, XNode * const eNode)
        {
            assert(0!=eNode);
            assert( eNode->defines<Equilibrium>() );
            XList & xlist = eNode->branch(); assert(xlist.size==4);
            XNode * node  = xlist.head;
            assert(0!=node);
            assert( node->is(Equilibrium::Label) );

            {
                node=node->next;
                assert(0!=node);
                assert( node->is(Equilibrium::Reac)  );
                fillActors(eq,Reactant,node,lib);
            }

            {
                node=node->next;
                assert(0!=node);
                assert( node->is(Equilibrium::Prod)  );
                fillActors(eq,Product,node,lib);
            }

            if(eq->size()<=0)
            {
                throw Specific::Exception(eq.name->c_str(), "no component!");
            }

            eq.latch();
            

        }


    }
}

