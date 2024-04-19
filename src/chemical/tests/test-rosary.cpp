
#include "y/chemical/rosary.hpp"
#include "y/chemical/reactive/actors.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

namespace Yttrium
{
    namespace Chemical
    {

        class Cluster : public Object, public EList
        {
        public:

            explicit Cluster(const Equilibrium &eq) : EList(), next(0), prev(0)
            {
                (*this) << eq;
            }

            virtual ~Cluster() noexcept {}

            const Cluster & operator*() const noexcept { return *this; }

            friend std::ostream & operator<<(std::ostream &os, const Cluster &cl)
            {
                const EList &el = cl;
                return os << el;
            }

            bool sharesSpeciesWith(const Equilibrium &rhs) const noexcept
            {
                for(const ENode *mine=head;mine;mine=mine->next)
                {
                    const Equilibrium &lhs = **mine;
                    if( lhs.sharesSpeciesWith(rhs) ) return true;
                }
                return false;
            }

            bool sharesSpeciesWith(const Cluster &other) const noexcept
            {
                for(const ENode *mine=head;mine;mine=mine->next)
                {
                    if(other.sharesSpeciesWith(**mine)) return true;
                }
                return false;
            }

            Cluster *next;
            Cluster *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
        };



        class Clusters : public CxxListOf<Cluster>
        {
        public:

            explicit Clusters(const Equilibria &eqs) : CxxListOf<Cluster>()
            {
                size_t                    n = eqs->size();
                Equilibria::ConstIterator i = eqs->begin();
                while(n-- > 0)
                {
                    const Equilibrium &eq = **(i++);
                    for(Cluster *cl=head;cl;cl=cl->next)
                    {
                        if(cl->sharesSpeciesWith(eq))
                        {
                            (*cl) << eq;
                            fusion();
                            goto DONE;
                        }
                    }
                    pushTail( new Cluster(eq) );
                DONE:
                    continue;
                }
            }

            virtual ~Clusters() noexcept
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
            void fusion() noexcept
            {
                CxxListOf<Cluster> store;
                while(size>0)
                {
                    AutoPtr<Cluster> tmp = popHead();
                    for(Cluster *lhs=store.head;lhs;lhs=lhs->next)
                    {
                        Cluster &rhs = *tmp;
                        if(lhs->sharesSpeciesWith(rhs))
                        {
                            lhs->mergeTail(rhs);
                            goto DONE;
                        }
                    }
                    store.pushTail(tmp.yield());
                DONE:
                    continue;
                }
                swapWith(store);
            }
        };


    }
}

Y_UTEST(rosary)
{
    Rosary & rosary = Rosary::Instance();
    Library       lib;
    LuaEquilibria eqs;
    std::cerr << rosary.callSign() << std::endl;

    for(int i=1;i<argc;++i)
    {
        rosary( Jive::Module::OpenData("data",argv[i]), lib, eqs);
    }

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;



    Y_SIZEOF(Chemical::Cluster);

    Chemical::Clusters cls(eqs);
    std::cerr << cls << std::endl;

}
Y_UDONE()

