
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

    template <>
    void FFT:: Raw(Real data[], const size_t n, const int isign) noexcept
    {
        typedef typename LongTypeFor<Real>::Type LongReal;
        LongReal wtemp,wr,wpr,wpi,wi,theta;
        Real     tempr,tempi;

        size_t mmax=2;
        while(n>mmax)
        {
            const size_t istep = (mmax << 1);
            //theta=isign*(6.28318530717959/mmax);
            theta=isign*(M_PI/istep);
            wtemp=sin(0.5*theta);
            wpr = -2.0*wtemp*wtemp;
            wpi = sin(theta);
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
    void FFT:: Opt(Real data[], const size_t n, const int isign) noexcept
    {
        typedef typename LongTypeFor<Real>::Type LongReal;
        LongReal  wr,wpr,wpi,wi,theta;
        //Real     tempr,tempi;

        size_t mmax=2;
        while(n>mmax)
        {
            const size_t istep = (mmax << 1);
            theta=isign*(M_PI/istep);
            LongReal wtemp=sin(0.5*theta);
            wpr = -2.0*wtemp*wtemp;
            wpi = sin(theta);
            wr=1.0;
            wi=0.0;
            for(size_t m=1;m<mmax;m+=2)
            {
                for(size_t i=m;i<=n;i+=istep)
                {
                    const size_t j=i+mmax;
                    const size_t ip=i+1;
                    const size_t jp=j+1;
                    const Real tempr=static_cast<Real>(wr*data[j]-wi*data[jp]);
                    const Real tempi=static_cast<Real>(wr*data[jp]+wi*data[j]);
                    data[j]   = data[i]  - tempr;
                    data[jp]  = data[ip] - tempi;
                    data[i]  += tempr;
                    data[ip] += tempi;
                }
                wr=(wtemp=wr)*wpr-wi*wpi+wr;
                wi=wi*wpr+wtemp*wpi+wi;
            }
            mmax=istep;
        }
    }


}
