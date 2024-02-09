
//! summing precomputed LeastSquares
inline ABSCISSA Of(const List &L)
{

    if(L.size<=0) return zero;

    //----------------------------------------------------------
    // prepare memory
    //----------------------------------------------------------
    {
        const LeastSquares &first = *(L.head);
        const size_t nv = first.dFda.size(); assert(nv>0);
        dFda.adjust(nv,zero);
        beta.adjust(nv,zero);
        curv.make(nv,nv);
        xadd.make(L.size);
    }

    //----------------------------------------------------------
    // first pass: npts
    //----------------------------------------------------------
    Coerce(npts) = 0;
    for(const LeastSquares *cls = L.head; cls; cls=cls->next)
    {
        Coerce(npts) += cls->npts;
    }

    //----------------------------------------------------------
    // second pass: beta
    //----------------------------------------------------------
    const size_t   nvar = beta.size();
    const ABSCISSA den  = ABSCISSA(npts);
    for(size_t i=nvar;i>0;--i)
    {
        xadd.free();
        for(const LeastSquares *cls = L.head; cls; cls=cls->next)
        {
            xadd << (cls->beta[i]* static_cast<ABSCISSA>(cls->npts) );
        }
        beta[i] = xadd.sum() / den;
    }

    //----------------------------------------------------------
    // third pass: curv
    //----------------------------------------------------------
    for(size_t i=nvar;i>0;--i)
    {
        for(size_t j=i;j>0;--j)
        {
            xadd.free();
            for(const LeastSquares *cls = L.head; cls; cls=cls->next)
            {
                xadd << (cls->curv[i][j]*static_cast<ABSCISSA>(cls->npts));
            }
            curv[i][j] = curv[j][i] = xadd.sum() / den;
        }
    }

    //----------------------------------------------------------
    // final D2
    //----------------------------------------------------------
    xadd.free();
    for(const LeastSquares *cls = L.head; cls; cls=cls->next)
    {
        xadd << (cls->last * static_cast<ABSCISSA>(cls->npts));
    }
    return ( xadd.sum()/den );

}
