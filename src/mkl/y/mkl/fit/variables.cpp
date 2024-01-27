
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
                upper(0)
                {
                }

                inline explicit Code(const Code &other) :
                Variable::DB(other),
                lower(other.lower),
                upper(other.upper)
                {
                }
                

                inline virtual ~Code() noexcept
                {
                }
                
                // create a new variable
                template <typename ID> inline
                void link(const ID &id, const size_t indx)
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
                    lower = Min(lower,indx);
                    upper = Max(upper,indx);
                }

                // Members
                size_t lower;
                size_t upper;

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

            Variables & Variables:: link(const String &name, const size_t indx)
            {
                assert(0!=code);
                code->link(name,indx);
                return *this;
            }

            Variables & Variables:: link(const char *name, const size_t indx)
            {
                assert(0!=code);
                code->link(name,indx);
                return *this;
            }

            Variables & Variables:: link(const char name, const size_t indx)
            {
                assert(0!=code);
                code->link(name,indx);
                return *this;
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


        }

    }

}
