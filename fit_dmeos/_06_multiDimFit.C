/*!
    this script follow the multidimfit.C in the tutorial folder.

    we have d = f( x1, x2, x3, x4)

    we don't know the exact f( x1, x2, x3, x4 ) funciton.
    but we want to assume it is:

    c1 * x1^0 * x2^0 * x3^0 * x4^0 +          // term1 = c1 * F1
    c2 * x1^1 * x2^0 * x3^0 * x4^0 +          // term2 = c2 * F2
    c3 * x1^0 * x2^1 * x3^0 * x4^0 +          // term3 = c3 * F3
    ...
    c5 * x1^2 * x2^1 * x3^0 * x4^0 +
    ...
    cL * x1^pL1 * x2^PL2 * x3^PL3 * x4^PL4.   // termL = c_L * F_L


    we can have many terms.
    in this script, we set the max number of F_L is 1000.
    by SetMaxFunctions(1000) and SetMaxStudy(1000)

    But we won't use all the 1000 F_L, some of them don't affect the
    fitting results, and we also need to avoid over fit
    i.e. use too many terms.
    ex. if we just have two points in (x1,y1) and (x2,y2) in a x,y plane.
    we can fit it by a line. y = c0 + c1* x
    or y = c0 + c1* x + c2* x^2
    or y = c0 + c1* x + c2* x^2 + c3* x^3.
    since we have just two data, the latter two are the over fit.

    We have 500 data points in our sample.
    we only chose the 30 significant F_L functions into our model.
    fit->SetMaxTerms(30); // Lmax = 30.
    ( Gram-Schmidt Orthogonalisation )
    the selection is done by algorithem in the TMultiDimFit class.
    and we just need to input some cretira.


    1) powers.
    Int_t mPowers[]   = { 6 , 6, 6, 6 };
    fit->SetMaxPowers(mPowers);
    i.e. for x1^p_L1, we require p_L1 < p_1Max.
         for x2^p_L2, we require p_L2 < p_2Max.
    here p_1Max = 6, so:
    x1^6 is not allowed in any F_L.
    x1^0, x1^1, ..., x1^5 are allowed.

    2) power control limit Q.
    fit->SetPowerLimit(1); // Q = 1
    by just condition 1, there are still too many terms.
    we further require:

    define Q_L for a given F_L =
    ( p_L1/p_1Max + p_L2/p_2Max + ... ) < Q
    i.e.
    x1^1 * x2^2 * x3^3 * x4^4 ==> Q = ( 1/6 + 2/6 + 3/6 + 4/6 ) > 1, we don't select it.
    x1^5 * x2^0 * x3^0 * x4^0 ==> Q = ( 5/6 + 0/6 + 0/6 + 0/6 ) < 1, allowed F_L.

    3) there are still some issues for the last term, F_Lmax.
    to test whether we should include the F_Lmax,
    we have two tests.
    i)  fit->SetMinAngle(10);
    ii) fit->SetMaxAngle(10);

    finally, we define good fitting as
    fit->SetMinRelativeError(.01); // esplion = 0.01
    S   = sum over all ( data - simulated value )
    D^2 = sum over all (data^2)
    esplion = S / D^2


    math note:
    legendre polynomial
    p0(x) = 1
    p1(x) = x
    p2(x) = 1/2 * ( 3 x^2 - 1) // == power = 2 term.
    p3(x) = 1/2 * ( 5 x^3 -3x) // == power = 3 term.


*/

//____________________________________________________________________
void makeData(Double_t* x, Double_t& d, Double_t& e)
{
    // Make data points

    // ( x1, x2, x3, x4, e )
    Double_t upp[5] = { 10, 10, 10, 10,  1 };

    Double_t low[5] = {  0,  0,  0,  0, .1 };


    // let x[i] from a gaussian distribution form low[i] to upp[i]
    for (int i = 0; i < 4; i++)
    {

        x[i] = (upp[i] - low[i]) * gRandom->Rndm() + low[i];
    }


    d = x[0] * TMath::Sqrt(x[1] * x[1] + x[2] * x[2] + x[3] * x[3]);

    e = gRandom->Gaus( upp[4],low[4] );
}



void _06_multiDimFit() {



    //===============================================
    // part 1 setting.

    // data parameters
    Int_t nVars   = 4;
    Int_t nData   = 500;

    TMultiDimFit* fit = new TMultiDimFit(nVars, TMultiDimFit::kLegendre,"v");
    // we have base function as:
    // TMultiDimFit::kMonomials, kChebyshev, kLegendre
    // "v" for verbose.

    // set up the max power for each variable.
    // i.e. x1^p_L1, and p_L1 < p_1Max.
    Int_t mPowers[]   = { 6 , 6, 6, 6 };
    fit->SetMaxPowers(mPowers);


    // set up the max number for F_L
    fit->SetMaxFunctions(1000);
    fit->SetMaxStudy(1000);

    // set up the max selected F_L
    fit->SetMaxTerms(30);


    // set the selection condition for a given F_L, we want:
    // Q_L = ( p_L1/p_1Max + p_L2/p_2Max + ... ) < Q
    fit->SetPowerLimit(1); // the power control limit Q.

    // test1 and test2 to whether we include the Lmax term.
    fit->SetMinAngle(10);
    fit->SetMaxAngle(10);

    // def a good fit as:
    // esplion = S / D^2
    // S   = sum over all ( data - simulated value )
    // D^2 = sum over all (data^2)
    fit->SetMinRelativeError(.01); // esplion = 0.01

    cout << "#==========Let's print Parameters==========#" << endl;
    fit->Print("p");







    //================================================
    // part 2
    // adding data:
    Double_t d;
    Double_t e;
    Double_t x[4];
    for(int i = 0; i < nData ; i++)
    {

        // Make some data
        makeData(x,d,e);

        // Add the row to the fit object by TMultiDimFit::AddRow()
        fit->AddRow(x,d,e);
    }



    //================================================
    // part 3
    // start the selection and fitting.

    // Book histograms (optional)
    // we can write histograms to root files and examine the fitting.
    // fit->MakeHistograms();

    // Find the parameterization -- doing the selection and fit.
    fit->FindParameterization();






    //================================================
    // part 4
    // print the fitting results.
    // P Parameters
    // S Statistics
    // C Coefficients
    // R Result of parameterisation
    // F Result of fit
    // K Correlation Matrix
    // M Pretty print formula

    cout << "#==========Let's print coefficeints========#" << endl;
    fit->Print("c");



    cout << "#==========Let's print formula   ==========#" << endl;
    fit->Print("M");


    // Write code to file, so that we can reused our parameterization.
    fit->MakeCode("MDF.C");

    delete fit;
}
