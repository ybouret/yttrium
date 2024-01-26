

#ifndef Y_Fit_Variables_Included
#define Y_Fit_Variables_Included 1

#include "y/mkl/fit/variable/interface.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {

            class Variables : public  Proxy<const Variable::DB>
            {
            public:
                typedef Proxy<const Variable::DB> ProxyType;

                Variables();
                Variables(const Variables &other);
                virtual ~Variables() noexcept;
                Variables & operator=(const Variables &other);

                Variables & link(const String &name, const size_t indx);
                Variables & link(const char   *name, const size_t indx);
                Variables & link(const char    name, const size_t indx);
                size_t      span() const noexcept; //!< highest index

                template <typename ID> inline
                Variables & operator<<(const ID &id)
                {
                    return link(id, (**this).size()+1 );
                }

                const Variable & get(const String &) const;
                const Variable & get(const char   *) const;
                const Variable & get(const char    ) const;

                template <typename ID> inline
                const Variable & operator[](const ID &id) const { return get(id); }

                
            private:
                virtual ConstInterface & surrogate() const noexcept;
                class Code;
                Code *code;
            };

        }

    }

}

#endif
