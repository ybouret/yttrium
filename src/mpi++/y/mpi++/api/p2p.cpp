#include "y/mpi++/api.hpp"
#include "y/stream/io/pack64.hpp"
#include "y/stream/data/input.hpp"

namespace Yttrium
{
    void MPI::send(const void *const data,
                   const size_t      count,
                   const DataType &  datatype,
                   const size_t      destination,
                   const int         tag)
    {
        assert( Good(data,count) );
        assert(count<=MaxCount);
        const uint64_t mark = getTicks();
        Y_MPI_CALL(MPI_Send(data, static_cast<int>(count), datatype.type, int(destination), tag, MPI_COMM_WORLD));
        traffic.send.record(count*datatype.size,getTicks()-mark);
    }


    void MPI:: sendSize(const size_t sz,
                        const size_t destination,
                        const int    tag)
    {
        sizeIO.erase();
        // send number of encoded
        const uint8_t  nw   = static_cast<uint8_t>(IO::Pack64::Emit(sizeIO,sz)); assert(sizeIO.measure() == nw);
        const uint64_t mark = getTicks();
        Y_MPI_CALL(MPI_Send(&nw, 1, MPI_BYTE,int(destination), tag, MPI_COMM_WORLD));

        // send encoded
        if(nw>0)
        {
            Y_MPI_CALL(MPI_Send(sizeIO.ro_addr(),nw, MPI_BYTE,int(destination), tag, MPI_COMM_WORLD));
        }
        traffic.send.record(nw+1,getTicks()-mark);

    }

    void  MPI::recv(void *             data,
                    const size_t       count,
                    const DataType    &datatype,
                    const size_t       source,
                    const int          tag)
    {
        assert( Good(data,count) );
        assert(count<=MaxCount);
        MPI_Status     status;
        const uint64_t mark  = getTicks();
        Y_MPI_CALL(MPI_Recv(data, static_cast<int>(count), datatype.type, int(source), tag, MPI_COMM_WORLD, &status));
        traffic.recv.record(count*datatype.size,getTicks()-mark);
    }

    size_t MPI:: recvSize(const size_t source,
                          const int    tag)
    {
        sizeIO.erase();
        MPI_Status status;

        // recv number of encoded
        uint8_t nw    = 0;
        {
            const uint64_t mark  = getTicks();
            Y_MPI_CALL(MPI_Recv(&nw, 1, MPI_BYTE, int(source), tag, MPI_COMM_WORLD, &status));
            if(nw>0)
            {
                sizeIO.adopt(nw); assert( sizeIO.measure() == nw );
                Y_MPI_CALL(MPI_Recv(sizeIO.rw_addr(), nw, MPI_BYTE, int(source), tag, MPI_COMM_WORLD, &status));
            }
            traffic.recv.record(nw+1,getTicks()-mark);
        }

        InputDataStream fp(sizeIO);
        return IO::Pack64::Read(fp, "RecvSize");
    }


    void MPI:: SendOne<String> :: With(MPI &mpi, const String &str, const size_t dst, const int tag)
    {
        const size_t sz = str.size();
        mpi.sendSize(sz,dst,tag);
        if(sz>0)
        {
            mpi.send(str.c_str(),sz,dst,tag);
        }
    }

    String MPI:: RecvOne<String>:: With(MPI &mpi, const size_t src, const int tag)
    {
        const size_t sz = mpi.recvSize(src,tag);
        if(sz>0)
        {
            String res(sz,AsCapacity,true);
            mpi.recv( static_cast<char *>(res.rw_addr()),sz,src,tag);
            return res;
        }
        else
        {
            return String();
        }
    }


    void MPI:: sendrecv(const void * const sendbuf,
                        const size_t       sendcount,
                        const DataType    &sendtype,
                        const size_t       destination,
                        const int          sendtag,
                        void * const       recvbuf,
                        const size_t       recvcount,
                        const DataType    &recvtype,
                        const size_t       source,
                        const int          recvtag)
    {
        assert(Good(sendbuf,sendcount));
        assert(Good(recvbuf,recvcount));
        assert(sendcount<=MaxCount);
        assert(recvcount<=MaxCount);

        MPI_Status status;
        const uint64_t mark = getTicks();
        Y_MPI_CALL(MPI_Sendrecv(sendbuf, static_cast<int>(sendcount), sendtype.type, int(destination), sendtag,
                                recvbuf, static_cast<int>(recvcount), recvtype.type, int(source),       recvtag,
                                MPI_COMM_WORLD, &status));
        const uint64_t ellapsed = getTicks() - mark;
        traffic.recv.record(recvcount*recvtype.size,ellapsed);
        traffic.send.record(sendcount*sendtype.size,ellapsed);


    }

}



