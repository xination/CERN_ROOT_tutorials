
// this function will be used to set up our TF1 object 
double myfunction( double* x, double* par ) {

    // it takes in a pointer 'x'
    // for example: double x[2] = { 0, 0);
    // or double* x = new double [2]
    // x[0] = x[1] = 0

    
    
    double deg = x[0];

    double c1  = par[0];
    double m   = par[1];
    
    double ret = c1 + TMath::Sin( m * deg / 180 *  TMath::Pi() );
    return ret;
}



// we will assign data for x and y here.
// the x range is from xMin to xMax.
// and we will have dataN points of (x,y)
void generate_data( double* x, double* y, int dataN,	
                    double xMin = 0., 
                    double xMax = 180. ) 
{
    
    // the answer.
    // and we want the fitting to show c1 and m are close to 
    // these values.
    double c1 = 1.5;
    double m  = 0.7;

    TF1* ftemp = new TF1( "myfunc", myfunction, xMin, xMax, 2 ); 
    
    ftemp->SetParameters( c1 , m );
    
    for( int i = 0; i < dataN; i++ ) 
    {
    
        x[i] = gRandom->Uniform( xMin, xMax );

        y[i] = ftemp->Eval( x[i] ) + gRandom->Uniform(-0.01, 0.01 );	
        
        // we use TF1::Eval method to get the y value from 
        // a given x, with our c1 and m parameters.
        // we also add some noise signals.

   }

   delete ftemp;
}





void _01_basic_fit( ) 
{


    // STEP1:
    // build our TF1 ( formula ) object
    //
    double xMin = 0;   
    double xMax = 180;
    int    parN = 2;   // the parameter numbers in the TF1.

    TF1* f1 = new TF1( "myfunc", myfunction, xMin, xMax, parN ); 
    /*
        this is one of the constructor of TF1 class.
        it takes in a function with  (Double_t*, Double_t*) arguments
        and return a Double_t.
         
    */

  
    // set the names for the two parameters. 
    f1->SetParNames( "c1", "m") ;

    
   


    //===============================================================

    // STEP2:
    // assign some data.
    const int dataN = 1000;
    double x[dataN] = {0.0};
    double y[dataN] = {0.0};

    generate_data( x, y, dataN, xMin, xMax ); 
    

    TGraph* gr = new TGraph( dataN, x, y );
    
    // just for fun, let see what data look like.
    TCanvas* c1 = new TCanvas("c1");
    gr->Draw("AP*");






    //===============================================================
    // STEP3:
    // do fitting and get results.

    // set initial value, important for the fit.
    f1->SetParameters( 0.0 , 1.0 );



    // some options
    //f1->SetParLimits(0, -1, 1 );  
    // set 0th par to be bounded between -1 and 1.

    // f1->FixParameter( 1, 0.8 );
    // set the 1th par to be fixed at 0.8

    gr->Fit( f1 , "Q" );
    // Q = quiet
    // N = not draw the fitted curve.
    // R = using the range defined in TF1, not those from TGraph (or TH1)
    // M = improve algorithm
    // more options. goo.gl/odmhGU 



    cout << "c1 = " << f1->GetParameter(0) << endl; 
    cout << "m  = " << f1->GetParameter(1) << endl; 
    cout << "Ndf =" << f1->GetNDF() << endl;
    cout << "chisqr =" << f1->GetChisquare() << endl;
}
