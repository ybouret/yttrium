//! \file

template <
typename FUNCTION,
typename GRADIENT,
typename SAMPLE>
ABSCISSA err(FUNCTION           &F,
             SAMPLE             &S,
             Writable<ABSCISSA> &aorg,
             const DomainType   &adom,
             const Booleans     &used,
             GRADIENT           &G,
             XMLog              &xml)
{

    static const ABSCISSA zero(0);


    return zero;

}

