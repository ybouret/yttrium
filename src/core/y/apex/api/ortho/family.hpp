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
            class Family :  public Metrics, public Proxy<const Vector::List>
            {
            public:

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
                void recreate(const Family &F);
                bool includes(const Family &sub);
                void generate(Random::Bits &ran, const size_t dim, const size_t bits);
                void increase();        //!< increase with latest workspace
                void clear()  noexcept; //!< free vectors
                void prune()  noexcept; //!< clear workspace
                void reset()  noexcept; //!< free/trim






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


                void fetchInteger(Writable<Integer> &target) const
                {
                    assert(0!=qwork);
                    assert(target.size()==dimensions);
                    target.ld(*qwork);
                }

                void fetchVector(Vector &target) const
                {
                    assert(0!=qwork);
                    assert(target.size()==dimensions);
                    target.ld(*qwork);
                }

                static const char * const VectorCoefficient;

                template <typename T>
                void fetchPrimitive(Writable<T> &target) const
                {
                    assert(0!=qwork);
                    assert(target.size()==dimensions);
                    for(size_t i=dimensions;i>0;--i) target[i] = (*qwork)[i].cast<T>(VectorCoefficient);
                }

                template <typename OUTPUT> inline
                void fetch(OUTPUT &output) const
                {
                    typedef typename OUTPUT::Type      OutputType;
                    typedef TL2(Integer,const Integer) ListType;
                    static const int Indx = TL::IndexOf<ListType,OutputType>::Value;
                    std::cerr << "(indx=" << Indx << ")" << std::endl;
                }




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
                
            };


            
        }
    }
}

#endif

