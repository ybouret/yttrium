
//______________________________________________________________
//
//! compute least squares of S w.r.t an out of order F
/**
 \param F    an out-of-order function
 \param S    a single sample
 \param aorg parameters
 */
//______________________________________________________________
template <typename FIT_FUNCTION>
inline ABSCISSA Of(FIT_FUNCTION             &F,
                   SamplesType              &S,
                   List                     &L,
                   const Readable<ABSCISSA> &aorg)
{

    //--------------------------------------------------------------------------
    // initialize
    //--------------------------------------------------------------------------
    assert(S.size() == L.size);
    const size_t ns = S.size();
    Coerce(npts) = 0;
    xadd.make(ns);
    xlst.flush();

    //--------------------------------------------------------------------------
    // Loop over samples
    //--------------------------------------------------------------------------
    {
        typename SamplesType::Iterator curr = S.begin();
        LeastSquares                  *node = L.head;
        for(size_t i=ns;i>0;--i,++curr,node=node->next)
        {
            assert(0!=node);
            SampleType    &sm = **curr;           // sample
            LeastSquares  &ls =  *node;           // least squares
            const ABSCISSA D2 = ls.Of(F,sm,aorg); // value
            const size_t   np = ls.npts;          // number of points
            const ABSCISSA sw(np);                // weight

            Coerce(npts) += np; // update total number of points
            xadd << (sw * D2);  // update xadd
        }
    }

    //--------------------------------------------------------------------------
    // final result
    //--------------------------------------------------------------------------
    return (npts<=0) ? zero : (xadd.sum() / static_cast<const ABSCISSA>(npts));
}


template <
typename FUNCTION,
typename GRADIENT>
inline ABSCISSA Of(FUNCTION                 &F,
                   SamplesType              &S,
                   List                     &L,
                   const Readable<ABSCISSA> &aorg,
                   const Booleans           &used,
                   GRADIENT                 &G)

{
    //--------------------------------------------------------------------------
    // initialize
    //--------------------------------------------------------------------------
    assert(S.size() == L.size);
    assert(aorg.size()==used.size());
    const size_t ns = S.size();
    const size_t nv = aorg.size();
    Coerce(npts) = 0;
    xadd.make(ns);
    xlst.flush();
    dFda.adjust(nv,zero);
    beta.adjust(nv,zero);
    curv.make(nv,nv);
    for(size_t i=nv;i>0;--i) curv[i][i] = one;

    //--------------------------------------------------------------------------
    // Pass 1: compute all beta/curv and store result
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Loop over samples
    //--------------------------------------------------------------------------
    {
        typename SamplesType::Iterator curr = S.begin();
        LeastSquares                  *node = L.head;
        for(size_t i=ns;i>0;--i,++curr,node=node->next)
        {
            assert(0!=node);
            SampleType    &sm = **curr;                  // sample
            LeastSquares  &ls =  *node;                  // least squares
            const ABSCISSA D2 = ls.Of(F,sm,aorg,used,G); // value + metrics
            const size_t   np = ls.npts;                 // number of points
            const ABSCISSA sw(np);                       // weight

            Coerce(npts) += np; // update total number of points
            xadd << (sw * D2);  // update xadd
        }
    }

    if(npts<=0)
    {
        return zero;
    }

    std::cerr << "---------- compiling..." << std::endl;
    std::cerr << "used: " << used << std::endl;
    const ABSCISSA den = static_cast<const ABSCISSA>(npts);
    const ABSCISSA res = xadd.sum() / den;
    for(const LeastSquares *node=L.head;node;node=node->next)
    {
        std::cerr << "node_beta: " << node->beta << std::endl;
    }
    for(size_t i=nv;i>0;--i)
    {
        if(!used[i]) continue;
        xadd.free();
        for(const LeastSquares *node=L.head;node;node=node->next)
            xadd << (node->beta[i] * static_cast<ABSCISSA>(node->npts));
        beta[i] = xadd.sum()/den;
    }

    for(const LeastSquares *node=L.head;node;node=node->next)
    {
        std::cerr << "node_curv: " << node->curv << std::endl;
    }

    return res;
}



#if 0
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
#endif

