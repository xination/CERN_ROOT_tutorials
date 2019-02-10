import ROOT 
from array import array

# in this script, we discuss how to set contour layers.

PI = ROOT.TMath.Pi()

# create a canvas, and draw a function z=f(x,y).
canvas_1 = ROOT.TCanvas("canvas_1","");
func = ROOT.TF2("f2","cos(x)*cos(y)", -PI/2,PI/2, -PI/2, PI/2);
func.SetLineWidth(2); # the mesh
func.SetLineColor(ROOT.kGray);

if( 0 ):
    #  example 1
    # by default for surf1z, we have colors for 20 layers.
    # and z values are equally divided.
    func.Draw("surf1z")
    


if( 0 ):
    #  example 2
    # Let's use just 4 colors, and set z values are equally divided 
    nLevels = 4
    func.SetContour( nLevels )
    func.Draw("surf1z")

if( 0 ):
    #  example 3
    #  4 colors, but  z values are non-equally divided 
    #  from the previous example or formula, we know z ranges from 0 to 1.
    
    #  suppose, we want to divide in the following way:
    #   0----0.7-----0.8------0.9------>inf
    #    col1 |  col2 |  col3  |  col4
    nLevels = 4
    cutValueArray = array( 'd', [ 0, 0.7, 0.8, 0.9 ] )
    func.SetContour( nLevels, cutValueArray )
    func.Draw("surf1z")
     

if( 1 ):
    pass 
    #  example 4
    #  customized Palette
    #  let's say, we still want the division setting from example3
    #  but we want to customize our colors.
    #  we define 4 colors : 
    #  color1 = 232, 185, 171
    #  color2 = 224, 152, 145
    #  color3 = 203, 118, 158
    #  color4 = 140,  95, 102
    #  note: you can set more colors, but since we only have nLevels =4
    #  there redundant colors will not be shown.

    rgb_set = [ (232, 185, 171), (224, 152, 145),\
                (203, 118, 158), ( 140, 95, 102)]
    
    myColorIdxList = []
    myColors = []
    for i in range( len(rgb_set) ):

        myColorIdx = ROOT.TColor.GetFreeColorIndex()
        myColorIdxList.append( myColorIdx )
        
        rgb = rgb_set[i]
        myColor = ROOT.TColor( myColorIdx, rgb[0]/255., rgb[1]/255., rgb[2]/255. ); 
        myColors.append( myColor )

    colorAarry = array( 'i', myColorIdxList )
    ROOT.gStyle.SetPalette( len(colorAarry), colorAarry )
    # there are many default Palette.
    # https://root.cern.ch/doc/master/classTColor.html

    # the following is the same for example3
    nLevels = 4
    cutValueArray = array( 'd', [ 0, 0.7, 0.8, 0.9 ] )
    func.SetContour( nLevels, cutValueArray )
    func.Draw("surf1z")

