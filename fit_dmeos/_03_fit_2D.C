
// gaus2d is for 2d gaussian function.
Double_t gaus2d( Double_t* t, Double_t* par ) {

    double x = t[0];
    double y = t[1];

    double a_x = par[0];
    double c_x = par[1];
    double s_x = par[2];

    double a_y = par[3];
    double c_y = par[4];
    double s_y = par[5];


    double termx = a_x * TMath::Exp( -0.5*(x-c_x)*(x-c_x) /s_x /s_x  );
    double termy = a_y * TMath::Exp( -0.5*(y-c_y)*(y-c_y) /s_y /s_y  );
    return termx * termy;

}





void _03_fit_2D() {

    //======================================================
    // Step 1
    // prepare a TH2F histo for source data.
    //
    TH2F* histo2D = new TH2F( "histo2D", "histo2D",
                               200, 0, 200,
                               200, 0, 200 );
    double width_x[3], width_y[3];
    double center_x[3], center_y[3];

    for( int i=0; i<10000; i++) {

            histo2D->Fill(  gRandom->Gaus( 100, 5),
                            gRandom->Gaus( 150, 3) );
    }

    // let's visualize the data
    TCanvas* c1 = new TCanvas("c1","c1", 500, 375);
    histo2D->Draw( "colz" );





    //======================================================
    // Step 2
    // prepare a TF2 for the fitting.
    

    
    TF2* f2 = new TF2( "f2", gaus2d, 50, 150, 50, 200, 6 );
    // the syntax is very similar
    // xMin, xMax, yMin, yMax, parN

    // it is important to set inital values for fitting.
    f2->SetParameters( 1000, 80, 8, 1000, 120, 3 ); 
    f2->SetParLimits( 2, 0, 10); // for sigma_x
    f2->SetParLimits( 5, 0, 10); // for sigma_y

    f2->SetParNames( "height_x", "center_x", "sigma_x",
                     "height_y", "center_y", "sigma_y" );



    
    // show the fit results.
    histo2D->Fit( f2, "MN"  ); 
    // "R" for using the range defined in TF2

    cout <<  "==================\n" << endl;
    
    printf( "center is at  x=%6.3f, y=%6.3f\n",
	    f2->GetParameter(1), f2->GetParameter(4));
    
    printf( "sigma_x = %6.3f, sigma_y = %6.3f\n",
	    f2->GetParameter(2), f2->GetParameter(5)  );

    histo2D->Draw();
    f2->Draw( "same");

}
