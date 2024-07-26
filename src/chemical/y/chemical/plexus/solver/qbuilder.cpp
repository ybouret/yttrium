#include "y/chemical/plexus/solver/qbuilder.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        QBuilder:: QBuilder(const size_t primary,
                            const size_t species,
                            const PBank &probank) :
        list(probank),
        qfam(species,primary)
        {
        }

        QBuilder:: ~QBuilder() noexcept {}



        const size_t & QBuilder:: key() const noexcept { return qfam.dimensions; }

        void QBuilder:: init() noexcept
        {
            list.free();
            qfam.free();
        }

        bool QBuilder:: grow(const Prospect &pro, const Matrix<int> &topo)
        {
            if( !qfam.wouldAccept(topo[pro.sub()])) return false;
            list << pro;
            qfam.expand();
            return true;
        }


        void QBuilder:: ensure(const size_t primary)
        {
            qfam.ensure(primary);
        }

        const QFamily & QBuilder:: family() const noexcept
        {
            return qfam;
        }


    }

}

#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        const char * const QBuilders:: CallSign = "Chemical::QBuilders";


        QBuilders:: QBuilders() : QBuilder::Set() {}
        QBuilders:: ~QBuilders() noexcept {}


        void QBuilders:: operator()(const size_t primary,
                        const size_t species,
                        const PBank &probank)
        {
            QBuilder::Ptr * const ppb = search(species);
            if(0!=ppb)
            {
                (**ppb).ensure(primary);
                return;
            }
            else
            {
                const QBuilder::Ptr ptr = new QBuilder(primary,species,probank);
                if(!insert(ptr))
                    throw Specific::Exception(CallSign,"corrupted!!");
            }
        }

        QBuilder & QBuilders:: operator[](const size_t species)
        {
            QBuilder::Ptr * const ppb = search(species);
            if(0==ppb) 
                throw Specific:: Exception(CallSign,"no #species=%u", unsigned(species));
            return **ppb;
        }

    }

}
