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
            class Family : public Quantized, public Metrics, public Proxy<const Vector::List>
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Family(const Metrics &, const Cache &) noexcept; //!< setup
                virtual ~Family() noexcept;                               //!< cleanup
                Y_OSTREAM_PROTO(Family);                                  //!< display

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

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



                void expand() noexcept; //!< expand with latest workspace
                void free()   noexcept; //!< reset
                void trim()   noexcept; //!< clear workspace

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
                Cache         cache; //!< shared cache

            public:
                Family *next; //!< for list/pool
                Family *prev; //!< for list/pool
            };
        }
    }
}

#endif

