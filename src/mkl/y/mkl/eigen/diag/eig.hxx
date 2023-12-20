
inline const Values<T> * eig(Matrix<T> &a)
{
    assert(a.isSquare());
    assert(a.rows>0);
    //--------------------------------------------------------------------------
    //
    // preparing
    //
    //--------------------------------------------------------------------------
    const size_t n = a.rows;
    wr.adjust(n,zero);
    wi.adjust(n,zero);
    wc.free();

    //--------------------------------------------------------------------------
    //
    // balance/reduce to Hessenber form
    //
    //--------------------------------------------------------------------------
    balance(a);
    reduce(a);
    size_t nr = 0;
    try {

        if( QR(a, wr, wi, nr ) )
        {
            assert(nr<=n);

            //------------------------------------------------------------------
            // extract complex values
            //------------------------------------------------------------------
            wc.ensure(n-nr);
            for(size_t j=1+nr;j<=n;++j)
            {
                const Cplx z(wr[j],wi[j]);
                wc << z;
            }

            //------------------------------------------------------------------
            // trim real part of complex values
            //------------------------------------------------------------------
            while( wr.size() > nr ) wr.popTail();
            assert(values.wr.size() + values.wc.size() == n);
            return &values;
        }
        else
        {
            return 0;
        }
    }
    catch(...)
    {
        wr.free();
        wi.free();
        wc.free();
        throw;
    }

}
