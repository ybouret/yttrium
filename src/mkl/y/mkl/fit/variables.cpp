
#include "y/mkl/fit/variables.hpp"
#include "y/mkl/fit/variable/replica.hpp"
#include "y/type/nullify.hpp"
#include "y/oversized.hpp"
#include "y/system/exception.hpp"
#include "y/associative/hash/map.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {


            class Variables:: Code : public Oversized, public Variable::DB
            {
            public:
                typedef HashMap<size_t,Variable::Handle> IndexMap;
                static const char * const Label;

                inline explicit Code() : 
                Variable::DB(),
                idmap(),
                lower(0),
                upper(0),
                maxNL(0)
                {
                }

                inline explicit Code(const Code &other) :
                Variable::DB(other),
                idmap(other.idmap),
                lower(other.lower),
                upper(other.upper),
                maxNL(other.maxNL)
                {
                }
                

                inline virtual ~Code() noexcept
                {
                }


                inline const Variable &upgradedWith(const Variable::Handle &hvar)
                {
                    const size_t indx = hvar->idx();
                    try {
                        if(!idmap.insert(indx,hvar))
                            throw Specific::Exception(Label, "unexpected failure to insert #%u for '%s'", unsigned(indx), hvar->c_str());
                    }
                    catch(...)
                    {
                        (void) remove( *hvar );
                        throw;
                    }

                    Coerce(lower) = Min(lower,indx);
                    Coerce(upper) = Max(upper,indx);
                    Coerce(maxNL) = Max(maxNL,hvar->size());
                    return *hvar;
                }

                inline const Variable & link(const Variable::Handle &hvar)
                {
                    const String          &name = *hvar;
                    const size_t           indx = hvar->idx();

                    // look for multiple index
                    {
                        const Variable::Handle *ppv = idmap.search(indx);
                        if(ppv)
                        {
                            throw Specific::Exception(Label,"'%s' index #%u already assigned to '%s'", name.c_str(), unsigned(indx), (**ppv).c_str());
                        }
                    }

                    // look for multiple name
                    if(!insert(hvar))
                        throw Specific::Exception(Label,"Multiple variable '%s'", name.c_str());

                    return upgradedWith(hvar);

                }

                //______________________________________________________________
                //
                // create a new primary variable
                //______________________________________________________________
                template <typename ID> inline
                const Variable &link(const ID &id, const size_t indx)
                {
                    assert(indx>0);
                    const Variable::Handle hvar = new PrimaryVariable(id,indx);
                    return link(hvar);
                }

                //______________________________________________________________
                //
                // create a new replica variable
                //______________________________________________________________
                template <typename ID> inline
                const Variable &link(const ID &id, const Variable &primary)
                {
                    if(primary.quantity()<=0) throw Specific::Exception(Label, "impossible replica of static '%s'", primary.c_str());
                    const Variable::Handle hprm = &Coerce(primary);
                    const Variable::Handle hvar = new ReplicaVariable(id,hprm);
                    return link(hvar);
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                IndexMap     idmap;
                const size_t lower;
                const size_t upper;
                const size_t maxNL;

            private:
                Y_DISABLE_ASSIGN(Code);

            };

            const char * const Variables::Code::Label = "Fit::Variables";
        }


        namespace Fit
        {

            Variables:: Variables() : code( new Code() )
            {
            }

            Variables:: Variables(const Variables &other) : code ( new Code( *(other.code) ) )
            {

            }

            Variables & Variables:: operator=(const Variables &other)
            {
                {
                    Variables vars(other);
                    Swap(code,vars.code);
                }
                return *this;
            }

            Variables:: ~Variables() noexcept
            {
                assert(0!=code);
                Nullify(code);
            }

            Variables::ConstInterface & Variables:: surrogate() const noexcept
            {
                assert(0!=code);
                return *code;
            }

            const Variable & Variables:: link(const String &name, const size_t indx)
            {
                assert(0!=code);
                return code->link(name,indx);
            }

            const Variable & Variables:: link(const char *name, const size_t indx)
            {
                assert(0!=code);
                return code->link(name,indx);
            }

            const Variable & Variables:: link(const char name, const size_t indx)
            {
                assert(0!=code);
                return code->link(name,indx);
            }

            size_t Variables:: span() const noexcept
            {
                assert(0!=code);
                return code->upper;
            }
           
            const Variable & Variables:: get(const String &name) const
            {
                assert(0!=code);
                const Variable::Handle *ppv = code->search(name);
                if(!ppv)
                    throw Specific::Exception(Code::Label,"No '%s'",name.c_str());
                return **ppv;
            }


            const Variable & Variables:: get(const char *name) const
            {
                const String _(name);
                return get(_);
            }

            const Variable & Variables:: get(const char name) const
            {
                const String _(name);
                return get(_);
            }

            const Variable & Variables:: link(const String &name, const Variable &v)
            {
                assert(0!=code);
                return code->link(name,v);
            }

            const Variable & Variables:: link(const char *name, const Variable &v)
            {
                assert(0!=code);
                return code->link(name,v);
            }

            const Variable & Variables:: link(const char name, const Variable &v)
            {
                assert(0!=code);
                return code->link(name,v);
            }

            const Variable & Variables:: link(const Variable &v)
            {
                return link(v,v);
            }

            size_t Variables:: maxNameLength() const noexcept
            {
                assert(0!=code);
                return code->maxNL;
            }

            const Variable * Variables:: query(const size_t indx) const noexcept
            {
                assert(0!=code);
                const Variable::Handle *ppv = code->idmap.search(indx);
                if(!ppv) return 0;
                return & (**ppv);
            }

            bool Variables:: found(const size_t indx)  const noexcept
            {
                assert(0!=code);
                return 0 != code->idmap.search(indx);
            }

        }

    }

}
