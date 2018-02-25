/*
    since we don't have TGraph3D class yet (root 6.12), 
    to fit v=v(x,y,z) will need different method.
    ( we don have TH3 class )

    one can use TMinuit or
    ROOT::Fit 

    TMinuit has to set the data to be global, and 
    write our own chi-square function.

    ROOT::Fit need to use BinData, WrappedMultiTF1, Fitter,
    and FitResult. 

    here we will demo the method using ROOT::Fit
    to fit v=v(x,y,z), 
*/

 
void _04_fit_3D() {



    //========================================================
    // part 1
    // generate data
    const int dataN = 1000; // for array size
    double x[dataN], y[dataN], z[dataN], v[dataN]; 
    // x,y,z = coordinate, v = value.
    // suppose our all x,y,z are with 0-10.
    // we assign x,y,z randomly between 0 to 10.
    
    TF3* f3 = new TF3( "f3", 
		"[0]*sin(x) + [1]*cos(y) + [2]*z",
		0, 10,
		0, 10,
		0, 10);

    // these are the answers
    // and we test the fit results by comparing them to these values.
    f3->SetParameters( 1.5, 0.7, 0.8 );

    for( int i = 0; i < dataN; ++i ) 
    {
    
    	// uniform distribution between 0 to 10.
    	x[i] = gRandom->Uniform( 0, 10 ); 
    	y[i] = gRandom->Uniform( 0, 10 ); 
    	z[i] = gRandom->Uniform( 0, 10 ); 

    	// we also add some noise. 
    	v[i] = f3->Eval( x[i], y[i], z[i] )+ gRandom->Uniform( -0.1, 0.1); 
    }






    //========================================================
    // part 2
    // we don't have TGraph3D class to hold our data, and so
    // we need to group data in to BinData object.
   
    const int varN = 3; // since we have x,y,z variables.
    ROOT::Fit::BinData data( dataN, varN );
    // create a BinData objects, with n points (1000), and dim = 3.
    // then we take a loop to assign 

    
    double inputs[varN]; 
    for( int i = 0; i < dataN; ++i ) {
    
    	inputs[0] = x[i];
    	inputs[1] = y[i];
    	inputs[2] = z[i];

	   data.Add( inputs, v[i], 0.1 );
    	// syntax note:
    	// void Add(const double* x, double val, double eval)
    	// eval = error for the value.
    }






    //========================================================
    // part 3
    // setup our fitter


    ROOT::Math::WrappedMultiTF1 wf( *f3, varN );
  
    // WrappedMultiTF1 takes TF1 ojbect ( by reference )
    // we have to deference of our pointer.
    // *f3 is to pass the TF1 object.

    /*
        syntax note: 
        WrappedMultiTF1 class constructor
        ROOT::Math::WrappedMultiTF1::WrappedMultiTF1 
        ( TF1 &f, unsigned int dim = 0 )

        we use WrappedMultiTF1 class to prepare a more generic function 
        for ROOT::Math::Fititer numerical algorithm. 
    */


    // create our Fitter object.
    ROOT::Fit::Fitter fitter; 
    fitter.SetFunction( wf );

    // set the initial values ( important for fitting )
    f3->SetParameters( 1, 1., 1. );






    //========================================================
    // part 4
    // get fit results
    bool ret = fitter.Fit( data );
    // put the BinData object 'data' into ROOT::Fit::Fitter::Fit()'
    // if we our fit is good, ret = true.

    if( ret ) 
    {

    	const ROOT::Fit::FitResult res = fitter.Result();
    	// syntax note:
    	// const FitResult & ROOT::Fit::Fitter::Result () const
    	// fitter.Result() will return a FitResult object ( by reference )

    	res.Print( std::cout );
        // if you don't want to see the summary, comment this line.
    	// setting the std::cout as our ostream object.
    	// and print out the results:
    	// *******************************************************
    	// Minimizer is Minuit / Migrad
    	// Chi2                      =      325.995
    	// NDf                       =          997
    	// Edm                       =  2.21939e-16
    	// NCalls                    =           42
    	// p0                        =      1.49994   +/-   0.00463529  
    	// p1                        =     0.701311   +/-   0.00442244  
    	// p2                        =       0.8003   +/-   0.000551579 


    	f3->SetFitResult( res );
    	// copy all fit result info (values, chi2, etc..) in TF3
    	// syntax note:
    	// void TF1::SetFitResult ( const ROOT::Fit::FitResult &result,
    	//                          const Int_t *  	indpar = 0  )
    	

        cout << "par [0] = " << f3->GetParameter( 0 ) << endl;	
        cout << "par [1] = " << f3->GetParameter( 1 ) << endl;	
        cout << "par [2] = " << f3->GetParameter( 2 ) << endl;	
    	

    }


}
