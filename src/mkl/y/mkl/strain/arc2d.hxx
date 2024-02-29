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
            code( new Code() ),
            r( code->cf[1] ),
            v( code->cf[2] ),
            a( code->cf[3] ),
            velocity(0),
            zero(0)
            {
                
            }


            template <>
            void Arc2D<real_t>:: free() noexcept
            {
                assert(0!=code);
                code->free();
                Coerce(velocity) = zero;
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
                Coerce(velocity) = v.norm();
            }

            template <>
            size_t Arc2D<real_t>:: load() const noexcept
            {
                assert(0!=code);
                assert(code->xf.load() == code->yf.load());
                return code->xf.load();
            }

            template <>
            real_t Arc2D<real_t>:: CartesianCurvature() const
            {
                assert(0!=code);
                if( velocity <= zero ) throw Specific::Exception(CallSign,"infinite curvature");
                return (v.x*a.y-v.y*a.x)/(velocity*velocity*velocity);
            }


            template <>
            real_t Arc2D<real_t>:: CylindricCurvature() const
            {
                assert(0!=code);
                if( r.x      <= zero ) throw Specific::Exception(CallSign,"negative radius");
                if( velocity <= zero ) throw Specific::Exception(CallSign,"infinite curvature");
                return (v.x*a.y-v.y*a.x)/(velocity*velocity*velocity)
                + v.y/r.x/velocity;
            }
        }
    }
}
