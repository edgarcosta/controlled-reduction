// Copyright 2013-2017 Edgar Costa
// See LICENSE file for license details.
//
// the zeta function is the same under a change of variables

#include "hypersurface_nd.h"
#include "timing.h"
#include "tools.h"

#define NTL_RANGE_CHECK
using namespace NTL;

void test_invariance_ND(int64_t p, int64_t n, int64_t d)
{
    Vec<int64_t> N;
    Vec<int64_t> charpoly_prec;
    int64_t precision;
    default_args(precision, N, charpoly_prec, p, n, d);
    {
        zz_pPush push(p);
        ZZ_pPush push2(power_ZZ(p,precision));

        hypersurface_non_degenerate hs;
        hs = hypersurface_non_degenerate(p, precision, n, d, false);
        Mat<ZZ_p> Fp_ND = hs.frob_matrix_ND(N);
        Mat<ZZ>  Frob_ZZ = conv<Mat<ZZ> >(Fp_ND);
        Vec<ZZ> cp = charpoly_frob(Frob_ZZ, charpoly_prec, p, n - 1);

        map< Vec<int64_t>, zz_p , vi64less> random_fbar, new_fbar;
        Mat<zz_p> slmatrix = random_SL_matrix(n+1);
        random_fbar =  change_of_variables((hs.dR)->fbar, slmatrix);

        Mat<zz_p> M = find_change_of_variables( random_fbar, 1000);
        new_fbar = change_of_variables( random_fbar, M);
        slmatrix = slmatrix * M;

        hypersurface_non_degenerate hs2;
        hs2 = hypersurface_non_degenerate(p, precision, new_fbar, false);
        Mat<ZZ_p> Fp_ND2 = hs.frob_matrix_ND(N);
        Mat<ZZ>  Frob_ZZ2 = conv<Mat<ZZ> >(Fp_ND2);
        Vec<ZZ> cp2 = charpoly_frob(Frob_ZZ2, charpoly_prec, p, n - 1);
        if(cp2 != cp)
        {
            cout << "\n\tFAIL test_invariance_ND" <<endl;
            cout << "\tp = " << p << " n = " << n << " d = " << d << " N = " << N << endl;
            cout << "\tslmatrix = " << slmatrix << endl;
            cout << "\tfbar = ";
            cout <<= (hs.dR)->fbar;
            cout << endl;
            cout << "\tnew_fbar = ";
            cout <<= new_fbar;
            cout << endl;
            abort();
        }
    }
}



int main()
{
    timestamp_pair pair;
    timestamp_mark(pair);
    SetSeed(to_ZZ(time(NULL)));
    cout<<"test_invariance_ND...";
    fflush(stdout);
    int64_t n = 2;
    for (int64_t d = n + 1; d <= 5; ++d) {
        int64_t plen = 5;
        int64_t runs = 20;
        if (d == 5 ) {
            plen = 4;
            runs = 1;
        }
        for (int64_t i = 0; i < runs; ++i) {
            int64_t p = GenPrime_long(plen);
            while ( p <= d )
                p = GenPrime_long(plen);
            // cout << p << " " << n << " " << d << endl;
            test_invariance_ND(p, n, d);
        }
    }
/*
    // it takes too long to be a regular test
    n = 3;
    int64_t d = 4;
    int64_t p = GenPrime_long(5);
    while ( p <= 23 )
        p = GenPrime_long(5);
    cout << p << " " << n << " " << d << endl;
    test_invariance_ND(p, n, d);
*/

    cout << "PASS ";
    timestamp_report(pair);
    cout << endl;
    return 0;
}


