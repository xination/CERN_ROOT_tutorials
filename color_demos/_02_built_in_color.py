import ROOT 

 
# TColor Reference
# https://root.cern.ch/doc/master/classTColor.html

if( 0 ):
    # to show the color_index for built-in 50 colors  
    
    canvas_1 = ROOT.TCanvas("canvas_1","50 basic colors");
    
    canvas_1.DrawColorTable();

    # usage: xxx.SetLineColor( color_index )



if( 0 ):

    # there are 15 color names   

    color_names\
    = ( ROOT.kWhite, ROOT.kBlack,  ROOT.kGray,    ROOT.kRed,   ROOT.kGreen,\
        ROOT.kBlue , ROOT.kYellow, ROOT.kMagenta, ROOT.kCyan,  ROOT.kOrange,\
        ROOT.kSpring, ROOT.kTeal,  ROOT.kAzure,   ROOT.kViolet,ROOT.kPink )

     
    canvas_2 = ROOT.TCanvas("canvas_2","color name");
    canvas_2.Range( 0, 0, 5, 3 );
    
    # use box to demo the colors.
    box = ROOT.TBox();
    box.SetFillStyle(1001)
    x1 = 0.2; x2=0.8;  y1 = 0.2; y2=0.8
    
    for idx in range(15):
        box.SetFillColor( color_names[idx] )  
        box.DrawBox( x1, y1, x2, y2 );

        # to check the position.
        # print "idx=%2d, x1 =%4.1f, x2 =%4.1f, y1 =%4.1f, y2 =%4.1f " \
        # %(idx, x1, x2, y1, y2 )
    
        x1 +=1; x2+=1;

        # to move to upper row.
        if( (idx+1) %5 ==0 ): 
            x1 = 0.2; x2=0.8; 
            y1+=1; y2+=1
        



if( 1 ):
    canvas_3 = ROOT.TCanvas("canvas_3","color wheel", 800, 800);
    colorwheel = ROOT.TColorWheel();
   
    colorwheel.SetCanvas( canvas_3 );
    colorwheel.Draw();

    # usage: xxx.SetLineColor( ROOT.kRed+2 )
    pass

