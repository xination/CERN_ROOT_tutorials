/*
    This script is to demo how to use TMinuit to fit.
    
    note: the data has to set in global.

    we have to write our own function to calculate chi-square.

    * Limits on variable parameters should be used only when needed 

    * TMinuit uses MIGRAD algorithm. 
    The MIGRAD algorithm is in general the best minimized for nearly all functions. 
    Its main weakness is that it depends heavily on knowledge of the first derivatives, 
    and fails miserably if they are very inaccurate. 

******************************************************************/

// we need to make the data global.
ROOT::Fit::BinData* GL_Data;
const int   GL_varN = 4;
const int   GL_dataN = 100;
 

// note: I use 'const double*' for t
// since ROOT::Fit::BinData::GetPoint will return const double*
double myfunc( const double* t, double* par ) {

	double x0 = t[0];
	double x1 = t[1];
	double x2 = t[2];
	double x3 = t[3];

	return par[0]*x0 + par[1]*x1*x2*x3 + par[2];
}



void generate_data( ) {


    GL_Data = new ROOT::Fit::BinData( GL_dataN, GL_varN );
    
    // these are our answers
    double myPar[3] = { 0.5, 0.6, 0.7};

    // for processing.
    double coordinates[GL_varN];
    double value;

    for( int i=0; i<GL_dataN; ++i) {

        coordinates[0] = gRandom->Uniform( 0, 10 );
        coordinates[1] = gRandom->Uniform( 0, 10 );
        coordinates[2] = gRandom->Uniform( 0, 10 );
        coordinates[3] = gRandom->Uniform( 0, 10 );

        value = myfunc( coordinates, myPar ) + gRandom->Uniform( -0.01, 0.01);

        GL_Data->Add( coordinates, value, 0 ); 
        // set error = 0, since we don't use it.
    }
}




void chi2( Int_t & /*nPar*/, Double_t * /*grad*/ , Double_t &fval, Double_t *par, Int_t /*iflag */  ){

    // calculated chi-square value.
    double chi2 = 0;
    double v_cal;

    // variable for processing.
    const double* coordinates; // to retrieve the coordinates in GL_Data.
    double value;

    // looping through all data points.
    // to calculated the chi-square value.
    for( int i=0; i<GL_dataN; ++i) {
	 
        coordinates = GL_Data->GetPoint(i, value );
        
        v_cal = myfunc(  coordinates, par );
	
        chi2 += ( v_cal - value )*( v_cal - value );

    }
    
    // finally, we let fval = chi2
    fval = chi2;
}




void _05_Minuit_part2() {


    //==============================
    // prepare data

    generate_data(); 
    



 
    const int parN = 3;
    TVirtualFitter* minuit = TVirtualFitter::Fitter( 0, parN );
 
    // setting the inital values. ( and I don't set the limits)
    minuit->SetParameter(0, "c1", 0.3, 0.001, 0, 0 );
    minuit->SetParameter(1, "c2", 0.3, 0.001, 0, 0 );
    minuit->SetParameter(2, "c3", 0.3, 0.001, 0, 0 );
    
    minuit->SetFCN( chi2 );




    // set print level
    double arglist[100];
    arglist[0] = 0;
    minuit->ExecuteCommand("SET PRINT",arglist, 1 );
     

    // do fit
    arglist[0] = 5000; // number of function calls
    arglist[1] = 1e-6; // tolerance
    minuit->ExecuteCommand("MIGRAD",arglist,2);
    




    //========================================
    // get fit result.

    double minParams[3];
    double parErrors[3];
    for (int i = 0; i < 3; ++i) {
        minParams[i] = minuit->GetParameter(i);
        parErrors[i] = minuit->GetParError(i);

        printf( "%d-th para = %6.3f, err = %6.3f\n", i, minParams[i], parErrors[i] );
    }

    


    double chi2, edm, errdef; // edm = Expected distance from minimum
    int nvpar, nparx;
    minuit->GetStats(chi2,edm,errdef,nvpar,nparx);

    cout << "edm = " << edm << endl;
    cout << "nvpar = " << nvpar << endl;
    cout << "nparx = " << nparx << endl;
    cout << "chi-squre = " << chi2 << endl;

}
