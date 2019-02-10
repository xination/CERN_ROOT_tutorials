import ROOT 
import pickle
from array import array

ROOT.gStyle.SetOptStat(0)
 

with open( "nuData.pickle", "rb") as f:
    nuclearData = pickle.load( f )

# to preview a subset of data.
if(0):
    for nuclei in nuclearData[:2]:
            print nuclei['A'], nuclei['Z'], nuclei['half_life'],  nuclei['unit']
    
    # print all the keys
    for key in nuclearData[0].keys():
        print key


histo = ROOT.TH2F("histo", "nuclei chart", 180, 0, 180, 120, 0, 120 );
# N = 180 bins, with bin size = 1
# bin0 = underflow, not shown.
# bin1 = [0-1]
# bin180 = [179-180]
 
 
for nuclei in nuclearData:

    # x == neutron number 
    # y == proton  number
    x = float( nuclei['A'] ) - float( nuclei['Z'] ) 
    y = float( nuclei['Z'] )                        
    
    # adjust x,y  for the position in 2D histogram.
    x -= 0.5
    y -= 0.5
    

    half_life_order = ROOT.TMath.Log10( nuclei['half_life_s']  )
    

    ibinx = histo.GetXaxis().FindBin( x )
    ibiny = histo.GetYaxis().FindBin( y )
    histo.SetBinContent( ibinx, ibiny, half_life_order )
   
      
    #================================ end of for loop.

histo.GetXaxis().SetNdivisions( 5 )
histo.GetYaxis().SetNdivisions( 5 )

# palette setting
# ROOT.gStyle.SetPalette( ROOT.kRainBow )
ROOT.gStyle.SetPalette( ROOT.kGreyScale )
ROOT.TColor.InvertPalette()

# contour setting 
#
# set up levels  
#
cutValueList = [-15., -7., -6., -5., -4., \
                 -3., -2., -1.,  0.,  1., \
                  2.,  3.,  4.,  5.,  7., \
                 10., 15. ]
 
cutValueArray = array('d', cutValueList ) 
histo.SetContour( len(cutValueArray), cutValueArray );


c1_canvas = ROOT.TCanvas("c1_canvas", "nuclei chart ", 800, 800 )
histo.GetYaxis().SetTitle("proton number")
histo.GetXaxis().SetTitle("neutron number")
histo.SetTitle("Log10( T_{1/2} )")
histo.Draw( "colz1")


 
 