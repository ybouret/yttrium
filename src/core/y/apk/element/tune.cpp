#include "y/apk/element.hpp"



namespace Yttrium
{
    namespace APK
    {
        InnerState Element:: TuneUp(Element &lhs, Element &rhs) noexcept
        {
            switch( Sign::Of(lhs.state,rhs.state) )
            {
                case Negative: lhs.set(rhs.state); break;
                case __Zero__: break;
                case Positive: rhs.set(lhs.state); break;
            }
            assert(lhs.state==rhs.state);
            return lhs.state;
        }

        void * Element:: Tuned(uint64_t &data, size_t &size, const InnerState &what) noexcept
        {
            void * const addr = &data;
            size              = 0;
            if(data>0)
            {

                switch(what)
                {
                    case AsBytes: {
                        uint64_t w = data;
                        uint8_t *p = (uint8_t *)addr;
                        for(size_t i=0,j=1;i<8;++i,++j)
                        {
                            if( (p[i] = static_cast<uint8_t>(w>>(8*i))) > 0) size = j;
                        }
                        assert(size>0);

                    } break;

                    case AsNum16: {
                        uint64_t  w = data;
                        uint16_t *p = (uint16_t *)addr;
                        for(size_t i=0,j=1;i<4;++i,++j)
                        {
                            if( (p[i] = static_cast<uint16_t>(w>>(16*i))) > 0) size = j;
                        }
                        assert(size>0);
                    } break;

                    case AsNum32: {
                        uint64_t  w = data;
                        uint32_t *p = (uint32_t *)addr;
                        for(size_t i=0,j=1;i<2;++i,++j)
                        {
                            if( (p[i] = static_cast<uint32_t>(w>>(32*i))) > 0) size = j;
                        }
                        assert(size>0);
                    } break;

                    case AsNum64:  size=1; break;

                }
            }

            return addr;
        }

    }

}

