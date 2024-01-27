

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
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Proxy<const Variable::DB> ProxyType;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                Variables();
                Variables(const Variables &other);
                virtual ~Variables() noexcept;
                Variables & operator=(const Variables &other);

                //______________________________________________________________
                //
                //
                // Create Primarty Variables
                //
                //______________________________________________________________
                const Variable & link(const String &name, const size_t indx);
                const Variable & link(const char   *name, const size_t indx);
                const Variable & link(const char    name, const size_t indx);
                size_t           span() const noexcept; //!< highest index

                template <typename ID> inline
                Variables & operator<<(const ID &id)
                {
                    (void) link(id, (**this).size()+1 );
                    return *this;
                }

                const Variable & link(const String &name, const Variable &v); //!< replica
                const Variable & link(const char   *name, const Variable &v); //!< replica
                const Variable & link(const char    name, const Variable &v); //!< replica
                const Variable & link(const Variable &v);                     //!< auto-replica


                //______________________________________________________________
                //
                //
                // access
                //
                //______________________________________________________________
                const Variable & get(const String &) const; //!< get by name
                const Variable & get(const char   *) const; //!< get by name
                const Variable & get(const char    ) const; //!< get by name

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
