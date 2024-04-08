
#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/utest/run.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/concurrent/frame/1d.hpp"
#include "y/concurrent/frame/2d.hpp"
#include "y/system/wtime.hpp"
#include "y/random/bits.hpp"

#include "y/sequence/vector.hpp"


using namespace Yttrium;


namespace
{

    class XNode : public Object
    {
    public:
        typedef CxxListOf<XNode> List;

        inline explicit XNode() noexcept : Object(),   next(0), prev(0), tmx(), ran() {}
        inline virtual ~XNode() noexcept {}

        XNode *      next;
        XNode *      prev;
        WallTime     tmx;
        Random::Rand ran;

        inline void wait() {
            tmx.wait( 0.1 * ran.to<double>() );
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XNode);
    };

    class XList : public XNode::List
    {
    public:
        explicit XList() noexcept {}
        virtual ~XList() noexcept {}

        void make(const size_t n)
        {
            while(size>n) delete popTail();
            while(size<n) pushTail( new XNode() );
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XList);
    };


    class X1D : public Concurrent::Frame1D<size_t>
    {
    public:

        inline explicit X1D(const ThreadContext &ctx) noexcept : 
        Concurrent::Frame1D<size_t>(ctx), node(0)
        {
        }

        inline virtual ~X1D() noexcept
        {
        }

        inline void link(XNode &xn)
        {
            Coerce(node) = &xn;
        }

        inline void unlink() noexcept
        {
            Coerce(node) = 0;
        }


        XNode * const node;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(X1D);
    };

    inline void DoSomething0(X1D &range)
    {
        {
            Y_LOCK(range.sync);
            (std::cerr << "\tDoSomething0(" << X1D::TrekPtr(range.trek) << ")" << std::endl).flush();
        }
    }

    inline void DoSomething1(X1D               &range,
                             const Vector<int> &v)
    {
        {
            Y_LOCK(range.sync);
            (std::cerr << "\tDoSomething1(" << X1D::TrekPtr(range.trek) << ")" << std::endl).flush();
        }
        const Concurrent::Trek<size_t> * const loop = range.trek;
        int sum = 0;
        if(loop)
        {
            for(size_t i=loop->offset,j=loop->length;j>0;--j,i+=loop->update)
            {
                sum += v[i];
                range.node->wait();
            }
        }
        {
            {
                Y_LOCK(range.sync);
                (std::cerr << "@" << range.name << ": sum = " << sum << std::endl).flush();
            }
        }
    }

}


namespace
{
    struct Strip
    {
        size_t icol; //!< initial column
        size_t irow; //!< initial row
        size_t ncol; //!< number of columns
        size_t cend; //!< end column


        inline friend std::ostream & operator<<(std::ostream &os, const Strip &S)
        {
            os << "(icol=" << S.icol << ",irow=" << S.irow <<",ncol=" << S.ncol << ",cend=" << S.cend << ")";
            return os;
        }
    };

    class X2D : public Concurrent::Frame2D<size_t>
    {
    public:
        inline explicit X2D(const ThreadContext &ctx) noexcept : 
        Concurrent::Frame2D<size_t>(ctx), 
        node(0),
        strip( *(const Strip **) Memory::OutOfReach::Addr(&segment) )
        {}

        inline virtual ~X2D() noexcept {}

        inline void link(XNode &xn)
        {
            Coerce(node) = &xn;
        }

        inline void unlink() noexcept
        {
            Coerce(node) = 0;
        }


        XNode *       const   node;
        const Strip * const & strip;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(X2D);
    };

    inline void In2D0(X2D &range)
    {
        {
            Y_LOCK(range.sync);
            (std::cerr << "\tIn2D0 @" << range.name << " (" << X2D::TilePtr(range.tile) << ")" << std::endl).flush();
            if(range.strip)
            {
                assert(0!=range.segment);
                (Core::Display(std::cerr, range.strip+1, range.tile->size) << std::endl).flush();
                //(std::cerr << "\t\t#strip=" << range.tile->size << " @" << range.segment << "/" << range.strip << std::endl).flush();
            }
        }
    }

}




Y_UTEST(concurrent_simtx)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");

    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);

    XList                 xls;

    {
        Concurrent::SIMT<X1D> seq( seqLoop );
        Concurrent::SIMT<X1D> par( parLoop );

        std::cerr << "-- Testing" << std::endl;
        seq();
        par();
        std::cerr << std::endl;

        seq.assign(1,10,2);
        par.assign(1,10,2);
        std::cerr << "-- Testing 0-arg" << std::endl;
        seq(DoSomething0);
        par(DoSomething0);
        std::cerr << std::endl;

        std::cerr << "-- Testing 1-arg" << std::endl;
        Vector<int> v(10,0);
        for(int i=10;i>0;--i) v[i] = i;

        {
            xls.make(seq.size());
            const Concurrent::SIMT<X1D>::AutoUnlink keep(seq);
            seq.link(xls.head);
            seq(DoSomething1,v);
        }

        {
            xls.make(par.size());
            const Concurrent::SIMT<X1D>::AutoUnlink keep(par);
            par.link(xls.head);
            par(DoSomething1,v);
        }
        std::cerr << std::endl;
    }


    {
        std::cerr << "In 2D" << std::endl;
        Concurrent::SIMT<X2D> seq( seqLoop );
        Concurrent::SIMT<X2D> par( parLoop );

        std::cerr << "-- Testing" << std::endl;
        seq();
        par();
        std::cerr << std::endl;

        const V2D<size_t> lower(1,1);
        const V2D<size_t> upper(3,4);

        seq.assign(lower,upper);
        par.assign(lower,upper);

        xls.make(par.size());
        par.unlink();
        par.link(xls.head);
        
        std::cerr << "-- Testing 0-arg" << std::endl;
        seq(In2D0);
        par(In2D0);
        std::cerr << std::endl;

    }

}
Y_UDONE()
