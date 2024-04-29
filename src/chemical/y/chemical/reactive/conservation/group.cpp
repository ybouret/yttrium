
#include "y/chemical/reactive/conservation/group.hpp"
#include "y/associative/address-book.hpp"
namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Group:: ~Group() noexcept
            {
            }

            Group:: Group(const Law &first) :
            Object(),
            Proxy<const LawList>(),
            species(),
            topIndx(),
            next(0),
            prev(0),
            laws(),
            maxUUID(0)
            {
                laws << first;
            }

            std::ostream & Group:: padLaw(std::ostream &os, const Law &law) const
            {
                for(size_t i=maxUUID;i<=law.uuid.size();++i) os << ' ';
                return os;
            }


            Group:: ConstInterface & Group:: surrogate() const noexcept
            {
                return laws;
            }

            void Group:: append(const Law &law)
            {
                laws << law;
                updateWith(law);
            }

            void Group:: append(Group &group) noexcept
            {
                laws.mergeTail(group.laws);
            }

            bool Group:: accepts(const Law &law) const noexcept
            {
                for(const LawNode *cl=laws.head;cl;cl=cl->next)
                {
                    const Law &mine = **cl;
                    if(mine.sharesSpeciesWith(law)) return true;
                }
                return false;
            }


            bool Group:: accepts(const Group &group) const noexcept
            {
                for(const LawNode *cl=laws.head;cl;cl=cl->next)
                {
                    if(group.accepts(**cl)) return true;
                }
                return false;
            }


            void Group:: compile()
            {
                //--------------------------------------------------------------
                // init
                //--------------------------------------------------------------
                Coerce(maxLength) = 0;
                Coerce(maxUUID)   = 0;
                Table  &T = Coerce(topIndx);
                T.free();
                {
                    SList  &L = Coerce(species);
                    L.free();
                    //----------------------------------------------------------
                    // collect all species
                    //----------------------------------------------------------
                    {
                        AddressBook book;
                        for(const LawNode *cl=laws.head;cl;cl=cl->next)
                        {
                            const Law &law = **cl;
                            Coerce(maxUUID) = Max(maxUUID,law.uuid.size());
                            for(const Actor *a=law->head;a;a=a->next)
                            {
                                updateWith(a->sp);
                                book |= a->sp;
                            }
                        }
                        SendBookTo<SList>(L,book);
                    }
                    LightSort::MakeAuxLevel(L);
                }
                
                //--------------------------------------------------------------
                // create table
                //--------------------------------------------------------------
                assert(species.size>0);
                assert( (**(species.tail)).indx[AuxLevel] == species.size);
                for(const SNode *node=species.head;node;node=node->next)
                {
                    T << (**node).indx[TopLevel];
                }

                //--------------------------------------------------------------
                // make algebraic laws
                //--------------------------------------------------------------
                //const size_t numSpeciesInGroup = species.size;
                for(LawNode *law=laws.head;law;law=law->next)
                {
                    Coerce(**law).makeAlgebraic(species);
                }

            }

        }

    }

}
