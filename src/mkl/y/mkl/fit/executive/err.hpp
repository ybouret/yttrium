//! \file

template <
typename FUNCTION,
typename GRADIENT,
typename SAMPLE>
ABSCISSA err(FUNCTION                 &F,
             SAMPLE                   &S,
             const Readable<ABSCISSA> &aorg,
             const DomainType         &adom,
             const Booleans           &used,
             GRADIENT                 &G,
             XMLog                    &xml)
{

    static const ABSCISSA zero(0);

    Y_XML_SECTION(xml,"LeastSquaresErr");

    const ABSCISSA D2org = D2(F,S,aorg,used,G);

    return zero;

}

