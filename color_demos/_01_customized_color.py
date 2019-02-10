import ROOT 

""" This script demo how to set customize color """

# color picker
# https://www.w3schools.com/colors/colors_picker.asp 

rgb = (146, 50, 195) # light purple.
# normally, each r, g, b ranges from 0 to 255,
# but in ROOT, each r, g, b range from 0 to 1.
# so you have to divide them by 255.


# we have to find a free color index to register our color 
# it can be done by TColor::GetFreeColorIndex()
# then construct the color.
myColor_idx = ROOT.TColor.GetFreeColorIndex()
myColor = ROOT.TColor( myColor_idx, rgb[0]/255., rgb[1]/255., rgb[2]/255. ); 


# just use TF1 object to demo the color. 
func = ROOT.TF1("func", "sin(x)", -1, 1 )
func.SetLineColor( myColor_idx  )  # put in the color idx.
func.SetLineWidth( 10 )
func.Draw()
