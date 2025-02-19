
#include "y/utest/run.hpp"

#include "y/osprey/tribe.hpp"

#include "y/random/shuffle.hpp"
#include "y/random/park-miller.hpp"

#include "y/functor.hpp"

#include "y/text/ascii/convert.hpp"
#include "y/stream/hash/output.hpp"
#include "y/hashing/sha1.hpp"
#include "y/hashing/md.hpp"
#include "y/memory/digest.hpp"

#include "y/container/matrix.hpp"
#include "y/system/exception.hpp"

#include "y/stream/xmlog.hpp"

using namespace Yttrium;
using namespace Apex;

namespace Yttrium
{
    namespace Osprey
    {

       

        

    

      

        typedef Functor<void,TL1(const QVector &)> Callback;


        class Tribes : public Proxy<const Tribe::List>
        {
        public:
            static Apex::Natural MaxCount(const size_t n)
            {
                Apex::Natural res=0;
                for(size_t i=1;i<=n;++i) res += Apex::Natural::Arrange(n,i);
                return res;
            }

            static void Display(const QVector &v)
            {
                std::cerr << "\t(+) " << v << std::endl;
            }


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
                Y_XML_SECTION_OPT(xml, "Osprey::Tribes","data[" << data.rows << "][" << data.cols << "]");
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
                {
                    Tribe::List ok;
                    while(my.size>0)
                    {
                        Tribe *tr = my.popHead();
                        if(0==tr->lastVec)
                        {
                            const size_t zid = tr->lastIdx;
                            Y_XML_COMMENT(xml, "removing null     data[" << zid << "]");
                            NoNullVec(zid,ok);
                            NoNullVec(zid,my);
                            delete tr;
                            continue;
                        }
                        ok.pushTail(tr);
                    }
                    my.swapWith(ok);
                }

                //--------------------------------------------------------------
                // process and detect trivially similar vectors
                //--------------------------------------------------------------
                {
                    Tribe::List primary;
                    while(my.size>0)
                    {
                        assert(0!=my.head->lastVec);
                        const QVector *vec = tryInsert(*(my.head->lastVec));
                        if(0!=vec)
                        {
                            proc(*vec);
                            primary.pushTail( my.popHead() );
                        }
                        else
                        {
                            const size_t indx = my.head->lastIdx;
                            delete my.popHead();
                            Y_XML_COMMENT(xml, "removing parallel data[" << indx << "]");
                            NoReplica(indx,primary);
                            NoReplica(indx,my);
                        }
                    }
                    my.swapWith(primary);
                }

                Y_XML_COMMENT(xml, "#dim = " << vc->dimensions);
                Y_XML_COMMENT(xml, "#vec = " << my.size);

            }

            virtual ~Tribes() noexcept
            { while(db.size>0) vc->store( Coerce(db).popTail() ); }


            friend std::ostream & operator<<(std::ostream &os, const Tribes &self)
            {
                if(self.my.size<=0) os << "{}";
                else
                {
                    os << "{ #" << self.my.size << std::endl;
                    for(const Tribe *tr=self->head;tr;tr=tr->next)
                    {
                        os << *tr << std::endl;
                    }
                    os << "}";

                }
                return os;
            }


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

                // check something happened
                for(Tribe *tr=my.head;tr;tr=tr->next)
                {
                    const QVector * const qv = tr->lastVec;

                }


            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribes);
            Y_PROXY_DECL();
            Tribe::List   my;
            QVCache       vc;
        public:
            const QVector::List db;

        private:

            //! remove zid from residue of tribes
            static void NoNullVec(const size_t zid, Tribe::List &tribes) noexcept
            {
                for(Tribe *tr=tribes.head;tr;tr=tr->next)
                {
                    tr->posture.residue.removeNull(zid);
                }
            }

            static void NoReplica(const size_t indx, Tribe::List &tribes)
            {
                for(Tribe *tr=tribes.head;tr;tr=tr->next)
                {
                    if(!tr->posture.tryRemove(indx)) throw Specific::Exception("Tribes::NoReplica","missing index %u", unsigned(indx));
                }
            }


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

        Y_PROXY_IMPL(Tribes,my)
    }

}

Y_UTEST(osprey)
{
    Random::ParkMiller ran;

    bool verbose = true;
    XMLog xml(verbose);

    size_t  rows = 5; if(argc>1) rows = ASCII::Convert::To<size_t>(argv[1],"rows");
    size_t  dims = 6; if(argc>2) dims = ASCII::Convert::To<size_t>(argv[2],"dims");


    Matrix<int>      data(rows,dims);
    for (size_t i = 1; i <= data.rows; ++i)
    {
        for (size_t j = 1; j <= data.cols; ++j)
        {
            data[i][j] = ran.in<int>(-5,5);
        }
    }

    //data[2].ld(0);
    data[3].ld(0);
    data[4].ld(data[1]); for(size_t i=data.cols;i>0;--i) data[4][i] *= -3;
    std::cerr << "data=" << data << std::endl;

    Osprey::IBank    bank;
    Osprey::QMetrics metrics(data.cols);
    Osprey::QVCache  vcache = new Apex::Ortho::Vector::Cache(metrics);
    Osprey::QFCache  fcache = new Apex::Ortho::Family::Cache(vcache);
    void (*proc_)(const Osprey::QVector &) = Osprey::Tribes::Display;
    Osprey::Callback proc(proc_);
    Osprey::Tribes   tribes(xml,proc,data,bank,fcache);

    size_t count = 0;
    while(tribes->size)
    {
        count += tribes->size;
        std::cerr << tribes << std::endl;
        tribes.generate(xml,proc,data);
    }
    std::cerr << "count=" << count << "/" << Osprey::Tribes::MaxCount(data.rows) << std::endl;



    for(size_t n=1;n<=16;++n)
    {
        const Apex::Natural nmax = Osprey::Tribes::MaxCount(n);
        std::cerr << n << " -> " << nmax.toDec(3) << std::endl;
    }


}
Y_UDONE()



