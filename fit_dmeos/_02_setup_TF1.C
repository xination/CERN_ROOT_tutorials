class gaus_class {

public:
    
    double simple_gaus( double* t, double* par) { 
        double x = t[0];
        double h = par[0]; // hight
        double c = par[1]; // center
        double w = par[2]; // width
        double temp = -0.5* ( (x-c)/w )*( (x-c)/w );
	return h*exp(temp);
    }
};

TGraph* gr;

void _02_setup_TF1() {

    //====================================================================
    // to use the built-in function ( gaus, expo, polN )
    // (you dont need to specify the parN.)
    if( 1 ) {

        TCanvas* c1 = new TCanvas("c1");
        c1->Divide(1,3, 0, 0 );
        
        TF1* f1a = new TF1( "f1a", "gaus", -3, 3 ); // p0*exp(-0.5*((x-p1)/p2)^2)). 
        TF1* f1b = new TF1( "f1b", "expo", -3, 3 ); // exp(p0+p1*x). 
        TF1* f1c = new TF1( "f1c", "pol2", -3, 3 ); // p0 + p1*x +p2*x**2
       
        f1a->SetParameters( 1, 0, 0.5 );
        f1b->SetParameters( 0, 1 );
        f1c->SetParameters( 1, 1, 1);
        
        c1->cd(1);  f1a->Draw();
        c1->cd(2);  f1b->Draw();
        c1->cd(3);  f1c->Draw();
    
        
 


    //====================================================================
    // inline method:
    // use [0] as 0th par, [1] as 1th par, etc ... 
     

        TCanvas* c2 = new TCanvas("c2");
        c2->Divide( 2,1,0,0);

        TF1* f2 = new TF1( "f2", "[0]*exp( -0.5* ( (x-[1])/[2] )**2 )", -3, 3 );
        

        f2->SetParameters( 1, 0, 0.5 );
        f2->SetLineColor( kBlue );
        c2->cd(1); f2->Draw();
        c2->cd(2); f1a->Draw();
    
   
    


    //====================================================================
    // using a method from a class.
    // advantage: it can access the member varaibles in the class.
     
        
        TCanvas* c3 = new TCanvas("c3");
        c3->Divide( 3,1,0,0);
        gaus_class* myGaus = new gaus_class();
        TF1* f3 = 
            new TF1( "f3", myGaus, &gaus_class::simple_gaus, -3, 3, 3 );
        
        f3->SetParameters( 0.5, 0., 0.5 );
        f3->SetLineColor( kGreen );
        c3->cd(1); f3->Draw();
        c3->cd(2); f2->Draw();
        c3->cd(3); f1a->Draw();
    }
      
    


    //====================================================================
    // use TGraph as a function.
    // we can use a lambda function to access TGraph object.
   
    if( 1 ) 
    {

        TCanvas* c4 = new TCanvas("c4", "c4");
        
        // let make some data for our TGraph object.
        const int dataN = 100;
        double xMin = 1;
        double xMax = 10;
        double x[dataN]; 
        double y[dataN];  

        for( int i =0; i<dataN; i++ )
        {
            x[i] =  xMin + (xMax-xMin)/ (dataN-1) * i ;
            y[i]  = 1/x[i] + 1.0 ; 
        }
        
        gr = new TGraph( dataN, x, y );
     
        auto lambda_func 
            = [&] (double*x, double *par){ return par[0]*gr->Eval(x[0]); };

        TF1 * f4 = new TF1( "f4", lambda_func, xMin, xMax, 1);
    
        // [] is the symbol for the start of lambda function.
        // [&] means this lambda function can access the variables in current scope.
        // so we can access gr in our lambda function.
        // we have 1 parameters in the lambda function
        // which is kind of scaling factor.

        f4->SetParameter(0, 0.95 );

        
        
        // draw the TF1 and TGraph together sometimes tricy 
        // (and may casue memory issue).
        f4->SetLineColor( kRed+1 );
        f4->Draw( "" );
        gr->Draw("same");
    }
     
}
