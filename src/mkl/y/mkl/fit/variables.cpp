
#include "y/mkl/fit/variables.hpp"
#include "y/mkl/fit/variable/replica.hpp"
#include "y/type/nullify.hpp"
#include "y/oversized.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {


            class Variables:: Code : public Oversized, public Variable::DB
            {
            public:
                static const char * const Label;

                inline explicit Code() : 
                Variable::DB(),
                lower(0),
                upper(0),
                maxNL(0)
                {
                }

                inline explicit Code(const Code &other) :
                Variable::DB(other),
                lower(other.lower),
                upper(other.upper),
                maxNL(other.maxNL)
                {
                }
                

                inline virtual ~Code() noexcept
                {
                }

                //______________________________________________________________
                //
                // create a new primary variable
                //______________________________________________________________
                template <typename ID> inline
                const Variable &link(const ID &id, const size_t indx)
                {
                    const Variable::Handle hVar = new PrimaryVariable(id,indx);

                    // check no multiple var
                    if( search(*hVar) )
                        throw Specific::Exception(Label, "Multiple Variable '%s'", hVar->c_str() );

                    // check no multiple index
                    for(Iterator it=begin(); it != end(); ++it)
                    {
                        const Variable &v = **it;
                        if( indx == v.idx() )
                            throw Specific::Exception(Label,
                                                      "Multiple index #%lu  for '%s' and '%s'",
                                                      (unsigned long)indx,
                                                      hVar->c_str(),
                                                      v.c_str() );
                    }

                    // final insertion
                    if( !insert(hVar) )
                        throw Specific::Exception(Label,"Unexpected failure to insert '%s'", hVar->c_str() );

                    // update
                    Coerce(lower) = Min(lower,indx);
                    Coerce(upper) = Max(upper,indx);
                    Coerce(maxNL) = Max(maxNL,hVar->size());
                    return *hVar;
                }

                //______________________________________________________________
                //
                // create a new replica variable
                //______________________________________________________________
                template <typename ID> inline
                const Variable &link(const ID &id, const Variable &primary)
                {
                    if(primary.quantity()<=0) throw Specific::Exception(Label, "impossible replica of static '%s'", primary.c_str());
                    const Variable::Handle hp = &Coerce(primary);
                    const Variable::Handle hv = new ReplicaVariable(id,hp);
                    if( !insert(hv) )
                        throw Specific::Exception(Label,"Replica of existing variable '%s'", hv->c_str() );

                    const size_t indx = hv->idx();
                    // update
                    Coerce(lower) = Min(lower,indx);
                    Coerce(upper) = Max(upper,indx);
                    Coerce(maxNL) = Max(maxNL,hv->size());
                    return *hv;
                }


                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
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

        }

    }

}
