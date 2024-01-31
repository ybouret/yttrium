
#include "y/field/idb.hpp"
#include "y/associative/hash/map.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Field
    {

        namespace
        {
            typedef HashMap<unit_t,SharedString> Indices;

            static void    *Indices__[ Y_WORDS_FOR(Indices) ];
            static Indices *Indices_ = 0;


        }

        const char * const IndexDataBase:: CallSign = "Field::IndexDataBase";

        IndexDataBase:: IndexDataBase()
        {
            Y_LOCK(access);
            Indices_ = new( Y_STATIC_ZARR(Indices__) ) Indices();
            std::cerr << "sizeof(Indices)=" << sizeof(Indices) << " / " << sizeof(Indices__) << std::endl;
        }

        IndexDataBase:: ~IndexDataBase() noexcept
        {
            Y_LOCK(access);
            assert(0!=Indices_);
            Memory::OutOfReach::Naught(Indices_);
            Indices_ = 0;
        }
       
        const String &IndexDataBase:: operator[](const unit_t i)
        {
            Y_LOCK(access);
            assert(0!=Indices_);
            Indices &db = *Indices_;
            {
                const SharedString *pps = db.search(i);
                if(pps) return **pps;
            }
            {
                const SharedString tmp = Formatted::New("%c%ld%c",Prefix, static_cast<long>(i), Suffix);
                if( !db.insert(i,tmp) )
                    throw Specific::Exception(CallSign, "couldn't insert '%s'", tmp->c_str());
                return *tmp;
            }

        }


    }

}


