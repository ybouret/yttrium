

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
            //__________________________________________________________________
            //
            //
            //
            //! set of unique [Primary|Replica]Variable(s)
            //
            //
            //__________________________________________________________________
            class Variables : public  Proxy<const Variable::DB>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Proxy<const Variable::DB> ProxyType; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                Variables();                              //!< setup
                Variables(const Variables &other);        //!< copy
                virtual ~Variables() noexcept;            //!< cleanup
                Variables & operator=(const Variables &); //!< assign

                //______________________________________________________________
                //
                //
                // Create Primary Variables
                //
                //______________________________________________________________
                const Variable & link(const String &name, const size_t indx); //!< new primary
                const Variable & link(const char   *name, const size_t indx); //!< new primary
                const Variable & link(const char    name, const size_t indx); //!< new primary
                size_t           span() const noexcept;                       //!< highest index

                //! helper to declare primary variables
                template <typename ID> inline
                Variables & operator<<(const ID &id)
                {
                    (void) link(id, (**this).size()+1 );
                    return *this;
                }

                //______________________________________________________________
                //
                //
                // Create Replica Variables
                //
                //______________________________________________________________
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

                //! any access
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
