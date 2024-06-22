#include "y/mkl/algebra/lu.hpp"
#include "y/utest/run.hpp"
#include "y/calculus/ipower.hpp"
#include "y/calculus/isqrt.hpp"
#include "y/kemp/ops/common-denominator.hpp"

using namespace Yttrium;

class MakeFilter : public Matrix<apq>
{
public:
    static const size_t Params   = 6;
    static const size_t MaxPower = 4;

    MakeFilter() :
    Matrix<apq>(Params,Params)
    {
    }

    template <typename WEIGHT> static inline
    apq get(WEIGHT &weight, const size_t ip, const size_t jp, const int delta)
    {
        apq sum = 0;

        for(int i=-delta;i<=delta;++i)
        {
            const apq I = ipower(i,ip);
            for(int j=-delta;j<=delta;++j)
            {
                const apq J = ipower(j,jp);
                const apq W = weight(i,j); //std::cerr << "W(" << i << "," << j << ") = " << W << std::endl;
                sum += I*J*W;
            }
        }

        return sum;
    }

    template <typename WEIGHT> static inline
    void fill(Writable<apq> &row, WEIGHT &weight, const size_t ip, const size_t jp, const int delta)
    {
        assert(row.size()==size_t(Squared(1+2*delta)));

        size_t k=0;
        for(int i=-delta;i<=delta;++i)
        {
            const apq I = ipower(i,ip);
            for(int j=-delta;j<=delta;++j)
            {
                const apq J = ipower(j,jp);
                const apq W = weight(i,j);
                row[++k] = I*J*W;
            }
        }
    }

    static inline
    void fill(Matrix<apq> &m, const Readable<apq> &row)
    {
        const size_t n = m.rows; assert(n*n==row.size());
        size_t       k = 0;
        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=n;++j)
            {
                m[j][i] = row[++k];
            }
        }
        LightArray<apq> arr = m.asArray();
        Kemp::CommonDenominator::Simplify(arr);
    }


    template <typename WEIGHT> inline
    void build(const size_t width, WEIGHT &weight)
    {
        if(0==(width&1)) throw Exception("width must be odd");
        const int             delta = int( (width-1)/2 );

        Matrix<apq> &mu = *this;

        mu[1][1] =            get(weight,0,0,delta);
        mu[1][2] = mu[2][1] = get(weight,1,0,delta);
        mu[1][3] = mu[3][1] = get(weight,0,1,delta);
        mu[1][4] = mu[4][1] = get(weight,2,0,delta);
        mu[1][5] = mu[5][1] = get(weight,0,2,delta);
        mu[1][6] = mu[6][1] = get(weight,1,1,delta);

        mu[2][2] =            get(weight,2,0,delta);
        mu[2][3] = mu[3][2] = get(weight,1,1,delta);
        mu[2][4] = mu[4][2] = get(weight,3,0,delta);
        mu[2][5] = mu[5][2] = get(weight,1,2,delta);
        mu[2][6] = mu[6][2] = get(weight,2,1,delta);

        mu[3][3] =            get(weight,0,2,delta);
        mu[3][4] = mu[4][3] = get(weight,2,1,delta);
        mu[3][5] = mu[5][3] = get(weight,0,3,delta);
        mu[3][6] = mu[6][3] = get(weight,1,2,delta);

        mu[4][4] =            get(weight,4,0,delta);
        mu[4][5] = mu[5][4] = get(weight,2,2,delta);
        mu[4][6] = mu[6][4] = get(weight,3,1,delta);

        mu[5][5] =            get(weight,0,4,delta);
        mu[5][6] = mu[6][5] = get(weight,1,3,delta);

        mu[6][6] =            get(weight,2,2,delta);

        //std::cerr << "mu=" << mu << std::endl;

        const size_t ncof = width*width;
        Matrix<apq>  rhs(Params,ncof);

        fill(rhs[1],weight,0,0,delta);
        fill(rhs[2],weight,1,0,delta);
        fill(rhs[3],weight,0,1,delta);
        fill(rhs[4],weight,2,0,delta);
        fill(rhs[5],weight,0,2,delta);
        fill(rhs[6],weight,1,1,delta);

        //std::cerr << "rhs=" << rhs << std::endl;

        MKL::LU<apq> lu(Params);
        if(!lu.build(mu))
            throw Exception("singular mu");
        lu.solve(mu,rhs);
        //std::cerr << "cof=" << rhs << std::endl;

        Matrix<apq> u(width,width);
        fill(u,rhs[2]);
        std::cerr << "u=" << u << std::endl;


        Matrix<apq> v(width,width);
        fill(v,rhs[3]);
        std::cerr << "v=" << v << std::endl;


    }

    virtual ~MakeFilter() noexcept
    {
    }



private:
    Y_DISABLE_COPY_AND_ASSIGN(MakeFilter);
};

static int UniformWeight(int,int)
{
    return 1;
}

static apq OneOverRadius(int i, int j)
{
    const int dx = 1+abs(i);
    const int dy = 1+abs(j);
    const int r2 = dx*dx+dy*dy;
    const int r  = IntegerSquareRoot(r2);
    return apq(1,r);
}

static apq OneOverRadiusSquare(int i, int j)
{
    const int dx = 1+abs(i);
    const int dy = 1+abs(j);
    const int r2 = dx*dx+dy*dy;
    //const int r  = IntegerSquareRoot(r2);
    return apq(1,r2);
}




Y_UTEST(makef)
{

    MakeFilter mkf;
    std::cerr << "Uniform 3x3" << std::endl;
    mkf.build(3, UniformWeight);
    std::cerr << std::endl;

    std::cerr << "Uniform 5x5" << std::endl;
    mkf.build(5, UniformWeight);
    std::cerr << std::endl;

    std::cerr << "OverR 3x3" << std::endl;
    mkf.build(3, OneOverRadius);
    std::cerr << std::endl;

    std::cerr << "OverR 5x5" << std::endl;
    mkf.build(5, OneOverRadius);
    std::cerr << std::endl;

    std::cerr << "OverR2 3x3" << std::endl;
    mkf.build(3, OneOverRadiusSquare);
    std::cerr << std::endl;

    std::cerr << "OverR2 5x5" << std::endl;
    mkf.build(5, OneOverRadiusSquare);
    std::cerr << std::endl;

}
Y_UDONE()

