
//! \file

#ifndef Y_Osprey_Tribes_Included
#define Y_Osprey_Tribes_Included 1

#include "y/osprey/tribe.hpp"
#include "y/functor.hpp"



namespace Yttrium
{
    namespace Osprey
    {
        typedef Functor<void,TL1(const QVector &)> Callback;

        class Tribes : public Proxy<const Tribe::List>
        {
        public:
            static Apex::Natural MaxCount(const size_t n);
            static void          Display(const QVector &);


            template <typename MATRIX> inline
            explicit Tribes(XMLog         & xml,
                            Callback      & proc,
                            const MATRIX  & data,
                            const IBank   & bank,
                            const QFCache & qfcc) :
            my(),
            vc(qfcc->vcache),
            db()
            {
                Y_XML_SECTION_OPT(xml, "Osprey::Tribes","data[" << data.rows << "][" << data.cols << "]" );
                //--------------------------------------------------------------
                // initialize all vectors
                //--------------------------------------------------------------
                {
                    const size_t rows = data.rows;
                    for(size_t indx=1;indx<=rows;++indx)
                    {
                        (void) my.pushTail( new Tribe(data,bank,qfcc,indx) )->lastVec;
                    }
                }

                //--------------------------------------------------------------
                // take care of null vector(s)
                //--------------------------------------------------------------
                noNullVec(xml);

                //--------------------------------------------------------------
                // process and detect trivially similar vectors
                //--------------------------------------------------------------
                noReplica(xml,proc);

                Y_XML_COMMENT(xml, "#vec = " << my.size << " in " << vc->dimensions << "d");
            }

            virtual ~Tribes() noexcept;
            Y_OSTREAM_PROTO(Tribes);




            template <typename MATRIX> inline
            void generate(XMLog &        xml,
                          Callback &     proc,
                          const MATRIX & data)
            {
                Y_XML_SECTION_OPT(xml, "Osprey::Generate", "tribes=" << my.size);
                // create new generation
                {
                    Tribe::List ng;
                    for(Tribe *tr=my.head;tr;tr=tr->next)
                    {
                        tr->unfold(xml,ng,data);
                    }
                    my.swapWith(ng);
                }

                // check if something happened
                for(Tribe *tr=my.head;tr;tr=tr->next)
                {
                    const QVector * const qv = tr->lastVec;
                    if(!qv)
                    {

                    }
                    else
                    {
                        const QVector *vec = tryInsert(*qv);
                        if(vec) proc(*vec);

                    }
                }

                Y_XML_COMMENT(xml,"#generated = " << my.size);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribes);
            Y_PROXY_DECL();
            Tribe::List   my;
            QVCache       vc;
        public:
            const QVector::List db;

        private:

            void noNullVec(XMLog &) noexcept;
            void noReplica(XMLog &, Callback &);

            //! remove zid from residue of tribes
            static void NoNullVec(const size_t zid, Tribe::List &tribes) noexcept;

            //! remove replica indx (must be found in content or residue
            static void NoReplica(const size_t indx, Tribe::List &tribes);


            //! try insert vector in database
            const QVector *tryInsert(const QVector &rhs)
            {
                for(const QVector *lhs=db.head;lhs;lhs=lhs->next)
                {
                    if( *lhs == rhs ) return 0;
                }
                return Coerce(db).pushTail( vc->query(rhs) );
            }
        };


    }

}

#endif

