
//______________________________________________________________
//
//! compute least squares of S w.r.t an out of order F
/**
 \param F    an out-of-order function
 \param S    a single sample
 \param aorg parameters
 \param vars variables
 */
//______________________________________________________________
inline ABSCISSA Of(OutOfOrderFunc           &F,
                   SamplesType              &S,
                   List                     &L,
                   const Readable<ABSCISSA> &aorg)
{
    assert(S.size() == L.size);
    const size_t ns = S.size();
    Coerce(npts) = 0;

    switch(ns)
    {
        case 0: return zero;
        case 1: return L.head->Of(F,**S.begin(),aorg);
        default:
            break;
    }

    xadd.make(ns);
    {
        typename SamplesType::Iterator curr = S.begin();
        LeastSquares                  *node = L.head;
        for(size_t i=ns;i>0;--i,++curr,++node)
        {
            SampleType    &sm = **curr;           // sample
            LeastSquares  &ls =  *node;           // least squares
            const ABSCISSA D2 = ls.Of(F,sm,aorg); // value
            const size_t   np = ls.npts;          // number of points
            const ABSCISSA sw(np);                // weight

            Coerce(npts) += np; // update total number of pointd
            xadd << (sw * D2);  // update xadd
        }
    }

    return (npts<=0) ? zero : xadd.sum() / static_cast<const ABSCISSA>(npts);


#if 0
    //----------------------------------------------------------
    // initialize
    //----------------------------------------------------------
    const size_t     np = S.numPoints();
    const Abscissae &a  = S.abscissae();
    const Ordinates &b  = S.ordinates();
    xadd.make(np*Dimension);
    xlst.flush();

    //----------------------------------------------------------
    // compute
    //----------------------------------------------------------
    for(size_t j=np;j>0;--j)
    {
        const ORDINATE Fj = F(a[j],aorg,vars);
        pushDSQ(b[j],Fj);
    }

    //----------------------------------------------------------
    // return sum
    //----------------------------------------------------------
    Coerce(npts) = np;
    return (  half * xadd.sum() );
#endif
}


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
