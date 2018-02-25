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

TH1F* histo; // we need to make the data global.



double myGaus( double* x, double* par ){

    double temp = (x[0]-par[1]) * (x[0]-par[1]) / par[2] / par[2];

    return par[0] * exp( -0.5 * temp  );

}



// it has fixed format for the argument list.
// and the return value has to be assigned to fval
void chi2( Int_t & /*nPar*/, Double_t * /*grad*/ , Double_t &fval, Double_t *par, Int_t /*iflag */  ){

    // calculated chi-square value.

    int nbin = histo->GetNbinsX();

    double chi2 = 0;
    double y;
    double x[1]; // serve as x[0]
    // the x (our defined array) and par (from argument)
    // will be passed through the myGaus function.

    // looping through all data points.
    // to calculated the chi-square value.
    for( int ibin=1; ibin <=nbin; ibin++) 
    {

        y = histo->GetBinContent( ibin );

        x[0] = histo->GetBinCenter ( ibin );

        double tmp = ( y - myGaus(x,par) );
         
        chi2 += tmp*tmp;
    }

    // finally, we let fval = chi2
    fval = chi2;
}




void _05_Minuit_part1() {


   
    // prepare data
    histo = new TH1F("histo", "histo", 100, -5, 5 );
    histo->FillRandom( "gaus", 1000 );





     
    // we use default
    // TVirtualFitter::SetDefaultFitter("Minuit");  
    
    TVirtualFitter* minuit = TVirtualFitter::Fitter( 0, 3 );

    /* synatx note:
        TVirtualFitter * TVirtualFitter::Fitter (
            TObject* obj,
            Int_t    maxpar = 25
        )
        Static function returning a pointer to the current fitter.
    */



    // setting the parameters.
    minuit->SetParameter(0, "a", 30, 0.01, 0, 0 );
    minuit->SetParameter(1, "c", 0,  0.01, 0, 0 );
    minuit->SetParameter(2, "s", 1,  0.01, 0, 0 );

    /*  syntax note:
        virtual Int_t TVirtualFitter::SetParameter(
		Int_t		   ipar,
		const char *   parname,
		Double_t  	   value,
		Double_t  	   verr,
		Double_t  	   vlow,
		Double_t  	   vhigh             )
	
        'value' is the initial value.
	   'verr' is the step size.
	   if vlow = vhigh = 0, then they are no limit.
    */

    // set our own chi-square function.
    minuit->SetFCN( chi2 );




    // set print level
    double arglist[100];
    arglist[0] = 0;
    minuit->ExecuteCommand("SET PRINT",arglist, 1 );
    // it is similar to type in "SET PRINT 1" as the argument for minuit.



    // start fiting
    arglist[0] = 5000; // number of function calls
    arglist[1] = 0.01; // tolerance
    minuit->ExecuteCommand("MIGRAD",arglist,2);
    // it is similar to type in "MIGRAD 5000 0.01" as the argument for minuit.

    /*  syntax note:
        Int_t TFitter::ExecuteCommand      (
	    const char *    command,
	    Double_t *      args,
	    Int_t	        nargs      ) 
    */





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



    TF1* f1 = new TF1("f1", myGaus, -3, 3, 3 );
    f1->SetParameters( minParams );

    histo->Draw();
    f1->Draw( "same" );
}
