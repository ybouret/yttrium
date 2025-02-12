//! \file

#ifndef Y_Apex_Ortho_Family_Included
#define Y_Apex_Ortho_Family_Included 1

#include "y/apex/api/ortho/vector.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {
            //__________________________________________________________________
            //
            //
            //
            //! Family of orthogonal vectors
            //
            //
            //__________________________________________________________________
            class Family : public Object, public Metrics, public Proxy<const Vector::List>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxPoolOf<Family> Pool;

                class Cache : public Object, public Counted, public Metrics, public Proxy<const Pool>
                {
                public:
                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    explicit Cache(const Metrics &, const VCache &) noexcept; //!< setup
                    virtual ~Cache()                                noexcept; //!< cleanup

                    //__________________________________________________________
                    //
                    // Methods
                    //__________________________________________________________
                    Family *query();                         //!< query/create
                    void    store(Family * const) noexcept;  //!< store and reset
                    Family *query(const Family &);           //!< duplicate

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Cache);
                    Y_PROXY_DECL();
                    Pool   my; //!< my pool
                    VCache vc; //!< shared vectors cache
                };


                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Family(const Metrics &, const VCache &) noexcept; //!< setup
                virtual ~Family() noexcept;                                //!< cleanup
                Y_OSTREAM_PROTO(Family);                                   //!< display

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                static bool AreEqual(const Family &lhs, const Family &rhs) noexcept;
                friend bool operator==(const Family &lhs, const Family &rhs) noexcept;
                friend bool operator!=(const Family &lhs, const Family &rhs) noexcept;


                //______________________________________________________________
                //
                //! compute if remaining orthogonal component is not zero
                //______________________________________________________________
                template <typename T> inline
                bool wouldAccept(const Readable<T> &a)
                {
                    assert(a.size()==dimensions);

                    // check dimension
                    if(qlist.size>=dimensions)
                        return false;

                    // check workspace
                    if(!qwork) qwork = cache->query();

                    // initialize
                    if(!qwork->set(a))
                        return false; // 0 vector

                    // remove projection
                    for(const Vector *basis=qlist.head;basis;basis=basis->next)
                    {
                        if(!qwork->keepOrtho(*basis)) return false;
                    }

                    return true;
                }
                
                void expand();          //!< expand with latest workspace
                void clear()  noexcept; //!< free vectors
                void prune()  noexcept; //!< clear workspace
                void reset()  noexcept; //!< free/trim


                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Quality quality; //!< current quality
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Family);
                Y_PROXY_DECL();
                Vector::List  qlist; //!< current list
                Vector *      qwork; //!< current workspace
                VCache        cache; //!< shared cache

            public:
                Family *next; //!< for list/pool
                Family *prev; //!< for list/pool
            };


            typedef ArcPtr<Family::Cache> FCache;

        }
    }
}

#endif

