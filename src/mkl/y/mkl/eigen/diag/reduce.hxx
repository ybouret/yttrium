

//! reduction to a real Hessenberg form
/**
 matrix should be balanced.
 */
inline void reduce( Matrix<T> &a ) throw()
{
    assert(a.isSquare());
    const size_t n = a.rows;

    //----------------------------------------------------------
    // m = r+1
    //----------------------------------------------------------
    for(size_t m=2; m<n; ++m )
    {
        const size_t r   = m-1;
        T            piv = zero;
        size_t       s   = m;
        for( size_t j=m+1;j<=n;++j)
        {
            //--------------------------------------------------
            // find the pivot
            //--------------------------------------------------
            const T tmp = a[j][r];
            if(Fabs<T>::Of(tmp)>Fabs<T>::Of(piv))
            {
                piv = tmp;
                s   = j;
            }
        }
        if( s != m )
        {
            assert(Fabs<T>::Of(piv)>zero);
            //--------------------------------------------------
            // First similarity transform: exchange colums/rows
            //--------------------------------------------------
            a.swapBoth(s,m);

            //--------------------------------------------------
            // Second similarity transform
            //--------------------------------------------------
            assert( Fabs<T>::Of(piv-a[m][m-1]) <= zero );
            for(size_t i=m+1;i<=n;++i)
            {
                const T factor = a[i][r] / piv;

                //----------------------------------------------
                // subtract factor times row r + 1 from row i
                //----------------------------------------------
                for(size_t j=n;j>0;--j) a[i][j] -= factor * a[m][j];

                //----------------------------------------------
                // add factor times column i to column r + 1
                //----------------------------------------------
                for(size_t j=n;j>0;--j)  a[j][m] += factor * a[j][i];
            }
        }
    }

    //==================================================================
    // clean up to the exact Hessenberg form
    //==================================================================
    for(size_t j=n;j>0;--j)
    {
        for(size_t i=j+2;i<=n;++i)
            a[i][j] = zero;
    }
    }
