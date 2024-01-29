

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
                typedef Proxy<const Variable::DB>   ProxyType;     //!< alias
                typedef Variable::DB::ConstIterator ConstIterator; //!< alias

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

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! get mutable named value from array
                template <typename WRITABLE, typename ID> inline
                typename WRITABLE::Type & operator()(WRITABLE &arr, const ID &id) const
                {
                    return get(id)(arr);
                }

                //! get constant named value from array
                template <typename READABLE, typename ID> inline
                typename READABLE::ConstType & operator()(const READABLE &arr, const ID &id) const
                {
                    return get(id)(arr);
                }

                const Variable * query(const size_t indx) const noexcept;
                bool             found(const size_t indx) const noexcept;

                //! get max name length from code
                size_t maxNameLength() const noexcept;


                //! display "pfx var.name sep arr[var]"
                template <typename ARRAY>
                void display(const char *pfx, std::ostream &os,  ARRAY &arr, const char *sep = 0) const
                {
                    if(!pfx) pfx="";
                    if(!sep) sep=" = ";
                    const size_t lmax = maxNameLength();
                    for(ConstIterator it = (**this).begin(); it!= (**this).end(); ++it)
                    {
                        const Variable &v = **it;
                        const String   &s = v;
                        const size_t    l = v.size();
                        os << s; for(size_t i=l;i<lmax;++i) os << ' '; os << sep;
                        os << v(arr);
                        os << std::endl;
                    }
                }



            private:
                virtual ConstInterface & surrogate() const noexcept;
                class Code;
                Code *code;
            };

        }

    }

}

#endif
