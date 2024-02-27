namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {

            template <>
            Arc2D<real_t>:: ~Arc2D() noexcept
            {
                assert(0!=code);
                Nullify(code);
            }

            template <>
            Arc2D<real_t>:: Arc2D() :
            Arc2DInfo(),
            ReadableType(),
            code( new Code() )
            {
                
            }


            template <>
            void Arc2D<real_t>:: free() noexcept
            {
                assert(0!=code);
                code->free();
            }

            template <>
            size_t Arc2D<real_t>:: size() const noexcept
            {
                return SIZE;
            }

            template <>
            const char * Arc2D<real_t>:: callSign() const noexcept
            {
                return CallSign;
            }

            template <>
            const V2D<real_t> & Arc2D<real_t>:: operator[](const size_t i) const noexcept
            {
                assert(0!=code);
                return code->cf[i];
            }

            template <>
            void Arc2D<real_t>:: add(const real_t t, const real_t x, const real_t y)
            {
                assert(0!=code);
                assert(code->xf.load() == code->yf.load());

                code->xf.add(t,x);
                try
                {
                    code->yf.add(t,y);
                }
                catch(...)
                {
                    code->xf.rem();
                    assert(code->xf.load() == code->yf.load());
                    throw;
                }

                assert(code->xf.load() == code->yf.load());

            }

            template <>
            void Arc2D<real_t>:: eval(const real_t t0, const size_t xdg, const size_t ydg)
            {
                assert(0!=code);
                code->eval(t0,xdg,ydg);
            }

            template <>
            size_t Arc2D<real_t>:: load() const noexcept
            {
                assert(0!=code);
                assert(code->xf.load() == code->yf.load());
                return code->xf.load();
            }

            template <>
            real_t Arc2D<real_t>:: Curvature() const
            {
                assert(0!=code);
                static const real_t zero(0);
                const ReadableType &self = *this;
                const Vertex       v     = self[2];
                const Vertex       a     = self[3];
                const real_t       velocity = v.norm();
                if( velocity <= zero ) throw Specific::Exception(CallSign,"infinite curvature");
                return (v.x*a.y-v.y*a.x)/(velocity*velocity*velocity);
            }

        }
    }
}
