//! file

//______________________________________________________________________________
//
//! LeastSquares for a function on a single sample
//______________________________________________________________________________
template <typename FUNCTION> inline
ABSCISSA D2(FUNCTION                 &F,
            SampleType               &S,
            const Readable<ABSCISSA> &aorg)
{
    return mine->Of(F,S,aorg);
}

//______________________________________________________________________________
//
//! LeastSquares for a function on a multiple samples
//______________________________________________________________________________
template <typename FUNCTION> inline
ABSCISSA D2(FUNCTION                 &F,
            SamplesType              &S,
            const Readable<ABSCISSA> &aorg)
{
    return mine->Of(F,S,roll->setup(S.size()),aorg);
}

//______________________________________________________________________________
//
//! LeastSquares+Metrics for a function on a single sample
//______________________________________________________________________________
template <typename FUNCTION, typename GRADIENT> inline
ABSCISSA D2(FUNCTION                 &F,
            SampleType               &S,
            const Readable<ABSCISSA> &aorg,
            const Booleans           &used,
            GRADIENT                 &G)
{
    return mine->Of(F,S,aorg,used,G);
}

//______________________________________________________________________________
//
//! LeastSquares+Metrics for a function on a multiple samples
//______________________________________________________________________________
template <typename FUNCTION, typename GRADIENT> inline
ABSCISSA D2(FUNCTION                 &F,
            SamplesType              &S,
            const Readable<ABSCISSA> &aorg,
            const Booleans           &used,
            GRADIENT                 &G)
{
    return mine->Of(F,S,roll->setup(S.size()),aorg,used,G);
}

