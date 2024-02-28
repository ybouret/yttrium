

#ifndef Y_Fit_Variables_Included
#define Y_Fit_Variables_Included 1

#include "y/mkl/fit/variable/interface.hpp"
#include "y/type/proxy.hpp"
#include "y/sequence/vector.hpp"
#include "y/text/justify.hpp"

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

                //! erase content
                void erase() noexcept;

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

                //! query by index, NULL means index is free
                const Variable * query(const size_t indx) const noexcept;

                //! check if index is available
                bool             found(const size_t indx) const noexcept;

                //! get max name length from code
                size_t maxNameLength() const noexcept;



                //! display "pfx var.name sep arr[var]"
                template <typename ARRAY>
                void display(const char *pfx, std::ostream &os,  ARRAY &arr, const char *sep = 0) const
                {
                    if(!pfx)     pfx  = "";
                    if(!sep)     sep  = " = ";
                    const size_t lmax = maxNameLength();
                    for(ConstIterator it = (**this).begin(); it!= (**this).end(); ++it)
                    {
                        const Variable &v = **it;
                        const String   &s = v;
                        os << pfx << Justify(s,lmax) << sep << v(arr) << std::endl;
                    }
                }

                //! display "pfx var.name  aorg[var] \pm aerr[var]"
                template <typename ARRAY, typename ERROR>
                void display(const char *pfx, std::ostream &os,  ARRAY &aorg, ERROR &aerr) const
                {
                    assert(aorg.size()==aerr.size());
                    const size_t   num  = aorg.size();
                    if(!pfx)       pfx  = "";
                    Vector<String> vorg(num,AsCapacity);
                    Vector<String> verr(num,AsCapacity);
                    size_t         lorg=0;
                    size_t         lerr=0;
                    for(ConstIterator it = (**this).begin(); it!= (**this).end(); ++it)
                    {
                        const Variable &v = **it;
                        vorg << Formatted::Get("%.15g", double(v(aorg)));
                        verr << Formatted::Get("%.15g", double(v(aerr)));
                        lorg = Max(lorg,vorg.tail().size());
                        lerr = Max(lerr,verr.tail().size());
                    }

                    const size_t   lmax = maxNameLength();
                    size_t         indx = 1;
                    for(ConstIterator it = (**this).begin(); it!= (**this).end(); ++it, ++indx)
                    {
                        const Variable &v = **it;
                        const String   &s = v;
                        os 
                        << pfx      << Justify(s,lmax)
                        << " = "    << Justify(vorg[indx],lorg)
                        << " \\pm " << Justify(verr[indx],lerr)
                        << " () "   << std::endl;
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
