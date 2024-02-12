//! \file

//______________________________________________________________
//
//! compute least squares of S w.r.t  F
/**
 \param F    a function (OOO,Seq)
 \param S    multiple samples
 \param L    list of least-squares, one for each sample
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


//______________________________________________________________
//
//! compute least squares of S w.r.t F, plus metrics with F
/**
 \param F    a function (OOO,Seq)
 \param S    multiple samples
 \param L    list of least-squares, one for each sample
 \param aorg parameters
 \param used used parameters
 \param G    gradient of F
 */
//______________________________________________________________
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
    // Pass 1: compute all nodes beta/curv and store result
    //--------------------------------------------------------------------------
    {
        typename SamplesType::Iterator curr = S.begin();
        LeastSquares                  *node = L.head;
        for(size_t i=ns;i>0;--i,++curr,node=node->next)
        {
            assert(0!=node);
            SampleType    &sm = **curr;                  // sample
            LeastSquares  &ls =  *node;                  // least squares
            const ABSCISSA D2 = ls.Of(F,sm,aorg,used,G,false); // value + metrics
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


    const ABSCISSA den = static_cast<const ABSCISSA>(npts);
    const ABSCISSA res = xadd.sum() / den;
  
    //--------------------------------------------------------------------------
    // Pass 2: compute  beta
    //--------------------------------------------------------------------------
    for(size_t i=nv;i>0;--i)
    {
        if(!used[i]) continue;
        xadd.free();
        for(const LeastSquares *node=L.head;node;node=node->next)
            xadd << (node->beta[i] * static_cast<ABSCISSA>(node->npts));
        beta[i] = xadd.sum()/den;
    }


    //--------------------------------------------------------------------------
    // Pass 2: compute  curv
    //--------------------------------------------------------------------------
    for(size_t i=nv;i>0;--i)
    {
        if(!used[i]) continue;
        for(size_t j=i;j>0;--j)
        {
            if(!used[j]) continue;
            xadd.free();
            for(const LeastSquares *node=L.head;node;node=node->next)
                xadd << (node->curv[i][j] * static_cast<ABSCISSA>(node->npts));
            curv[i][j] = curv[j][i] = xadd.sum() / den;
        }
    }

    //--------------------------------------------------------------------------
    // post regularize
    //--------------------------------------------------------------------------
    {
        typename SamplesType::Iterator curr = S.begin();
        LeastSquares                  *node = L.head;
        for(size_t i=ns;i>0;--i,++curr,node=node->next)
            node->regularizeWith(**curr,used);
    }
    return res;
}


