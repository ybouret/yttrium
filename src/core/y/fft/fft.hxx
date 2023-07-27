
namespace Yttrium
{
    
    template < >
    size_t FFT:: MakeXBR<Real>(Real data[], const size_t size) noexcept
    {
        const size_t n = (size<<1);
        size_t j=1;
        for(size_t i=1;i<n;i+=2)
        {
            if(j>i)
            {
                Swap(data[j],data[i]);
                Swap(data[j+1],data[i+1]);
            }
            size_t m=size;
            while (m >= 2 && j > m)
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }
        return n;
    }


    template < >
    size_t FFT:: MakeXBR<Real>(Complex<Real>   data[],
                               const size_t size) noexcept
    {
        const size_t half  = size>>1;
        size_t j=0;
        for(size_t i=0;i<size;++i)
        {
            if(j>i)
            {
                Swap(data[j],data[i]);
            }

            size_t m=half;
            while( (m>0) && j >= m)
            {
                j  -= m;
                m >>= 1;
            }
            j += m;
        }
        return size << 1;
    }

    template < >
    size_t FFT:: makeXBR<Real>(Complex<Real>     data[],
                               const size_t      size,
                               const unsigned    shift) noexcept
    {
        assert( ( size_t(1) << shift) == size);
        if(shift<MinShift)
        {
            return size<<1;
        }
        else
        {
            if(shift>MaxShift)
            {
                return MakeXBR(data,size);
            }
            else
            {
                const uint32_t *arr = xbrp[shift]; assert(0!=arr);
                for(size_t k=xbrn[shift];k>0;--k)
                {
                    const XBR xbr = { arr[k] };
                    Swap(data[xbr.i],data[xbr.j]);
                }
                return size<<1;
            }
        }
    }

    static inline void Raw(Real data[], const size_t n, const int isign) noexcept
    {
        typedef typename LongTypeFor<Real>::Type LongReal;
        static  const    LongReal PI = _FFT<LongReal>::PI;
        static  const    LongReal half(0.5);
        LongReal wtemp,wr,wpr,wpi,wi,theta;
        Real     tempr,tempi;

        size_t mmax=2;
        while(n>mmax)
        {
            const size_t istep = (mmax << 1);
            //theta=isign*(6.28318530717959/mmax);
            theta=isign*(PI/istep);
            wtemp= std::sin(half*theta);
            wpr = -2.0*wtemp*wtemp;
            wpi = std::sin(theta);
            wr=1.0;
            wi=0.0;
            for(size_t m=1;m<mmax;m+=2)
            {
                for(size_t i=m;i<=n;i+=istep)
                {
                    const size_t j=i+mmax;
                    tempr=wr*data[j]-wi*data[j+1];
                    tempi=wr*data[j+1]+wi*data[j];
                    data[j]=data[i]-tempr;
                    data[j+1]=data[i+1]-tempi;
                    data[i]   += tempr;
                    data[i+1] += tempi;
                }
                wr=(wtemp=wr)*wpr-wi*wpi+wr;
                wi=wi*wpr+wtemp*wpi+wi;
            }
            mmax=istep;
        }
    }


    template <>
    void FFT:: Forward<Real>(Real data[], const size_t size) noexcept
    {
        Raw(data,MakeXBR(data,size),1);
    }

    template <>
    void FFT:: Reverse<Real>(Real data[], const size_t size) noexcept
    {
        Core::Display(std::cerr << "reverse : ",data+1,2*size) << std::endl;
        Raw(data,MakeXBR(data,size),-1);
        Core::Display(std::cerr << "result  : ",data+1,2*size) << std::endl;

    }


    static inline void Opt(Real         data[],
                           const size_t n,
                           const typename LongTypeFor<Real>::Type Sin[],
                           const typename LongTypeFor<Real>::Type Aux[]) noexcept
    {
        typedef typename LongTypeFor<Real>::Type LongReal;
        typedef Complex<Real> Cplx;
        static const LongReal one(1);
        static const LongReal zero(0);

        size_t   mmax=2;
        size_t   step=4;
        unsigned curr=2;
        unsigned next=3;
        while(n>mmax)
        {
            LongReal     wtemp = Sin[next];
            LongReal     wpr   = Aux[next];
            LongReal     wpi   = Sin[curr];
            LongReal     wr    = one;
            LongReal     wi    = zero;
            for(size_t m=1;m<mmax;m+=2)
            {
                for(size_t i=m;i<n;i+=step)
                {
                    const size_t j=i+mmax;
                    Cplx        &pj = *(Cplx *)&data[j];
                    Cplx        &pi = *(Cplx *)&data[i];
                    const Cplx   cj = pj;
                    const Cplx   z( static_cast<Real>(wr*cj.re-wi*cj.im),static_cast<Real>(wr*cj.im+wi*cj.re) );
                    pj  = pi - z;
                    pi.in_place_add(z);
                }
                wr=(wtemp=wr)*wpr-wi*wpi+wr;
                wi=wi*wpr+wtemp*wpi+wi;
            }
            mmax=step; step <<= 1;
            curr=next; ++next;
        }
    }

    template <>
    void FFT:: forward(Complex<Real>   cplx[],
                       const size_t    size,
                       const unsigned  shift) noexcept
    {
        typedef typename LongTypeFor<Real>::Type LongReal;
        Opt((&(cplx[0].re))-1,
            makeXBR(cplx,size,shift),
            _FFT<LongReal>::Fwd,
            _FFT<LongReal>::Aux);
    }

    template <>
    void FFT:: reverse(Complex<Real>   cplx[],
                       const size_t    size,
                       const unsigned  shift) noexcept
    {
        typedef typename LongTypeFor<Real>::Type LongReal;
        Opt((&(cplx[0].re))-1,
            makeXBR(cplx,size,shift),
            _FFT<LongReal>::Rev,
            _FFT<LongReal>::Aux);
    }


    static inline
    void FFT_Pack(Real         fft1[],
                  const Real   data1[],
                  const Real   data2[],
                  const size_t n) noexcept
    {
        assert(IsPowerOfTwo(n));
        assert(0!=fft1);
        assert(0!=data1);
        assert(0!=data2);
        for(size_t j=1,jj=2;j<=n;j++,jj+=2)
        {
            fft1[jj-1]= data1[j];
            fft1[jj]  = data2[j];
        }
    }

    template < >
    void FFT:: Unpack( Real fft1[], Real fft2[], const size_t n) noexcept
    {
        static const Real half(0.5);
        const size_t n1  = n+1;
        const size_t nn2 = n1 << 1;
        const size_t nn3 = 1+nn2;
        fft2[1]=fft1[2];
        fft1[2]=fft2[2]=0;
        for(size_t j=3;j<=n1;j+=2) {
            const Real rep=half*(fft1[j]+fft1[nn2-j]);
            const Real rem=half*(fft1[j]-fft1[nn2-j]);
            const Real aip=half*(fft1[j+1]+fft1[nn3-j]);
            const Real aim=half*(fft1[j+1]-fft1[nn3-j]);
            fft1[j]=rep;
            fft1[j+1]=aim;
            fft1[nn2-j]=rep;
            fft1[nn3-j] = -aim;
            fft2[j]=aip;
            fft2[j+1] = -rem;
            fft2[nn2-j]=aip;
            fft2[nn3-j]=rem;
        }
    }

    template <>
    void FFT:: Forward(Real         fft1[],
                       Real         fft2[],
                       const Real   data1[],
                       const Real   data2[],
                       const size_t n) noexcept
    {
        assert(IsPowerOfTwo(n));
        assert(0!=fft1);
        assert(0!=fft2);
        assert(0!=data1);
        assert(0!=data2);


        Core::Display(std::cerr << "data1=", data1+1, n) << std::endl;
        Core::Display(std::cerr << "data2=", data2+1, n) << std::endl;


        FFT_Pack(fft1,data1,data2,n);
        Core::Display(std::cerr<<"pack1=",fft1+1,n*2) << std::endl;
        Forward(fft1,n);
        Unpack(fft1,fft2,n);

    }

    template <>
    void FFT:: forward(Complex<Real>     fft1[],
                       Complex<Real>     fft2[],
                       const Real        data1[],
                       const Real        data2[],
                       const size_t      size,
                       const unsigned    shift) noexcept
    {
        assert(size_t(1)<<shift==size);
        assert(0!=fft1);
        assert(0!=fft2);
        assert(0!=data1);
        assert(0!=data2);

        Real       *c1 = &(fft1[0].re) - 1;
        Real       *c2 = &(fft2[0].re) - 1;
        const Real *d1 = data1-1;
        const Real *d2 = data2-1;

        FFT_Pack(c1,d1,d2,size);
        forward(fft1,size,shift);
        Unpack(c1,c2,size);
    }


}
