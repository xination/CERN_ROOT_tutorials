import ROOT 
import math
# regarding the import you can do "from ROOT import *" 
# then you can omit "ROOT." prefix
# But this is not a good practice.
# to use the class from ROOT, such as TH1F (1D histogram with float type)
# h = ROOT.TH1F( args for the constructor ... )

# it will still read the ".rootlogon.C" file, which is C++ file.
ROOT.gStyle.SetPadGridX( True )
ROOT.gStyle.SetPadGridY( True )
ROOT.gStyle.SetOptStat("e")
ROOT.gStyle.SetStatX(0.9)
ROOT.gStyle.SetStatY(0.9)

# you can put style setting into .rootlogon.C file 
# in the current folder, it looks like:
#  { gStyle->SetOptStat("eI"); 
#    ....
#  }



#===============================================
# structure of this tutorial:
# part 1: 
#   create a histogram, and fill it with random number.
#   then write out to a root file.        
#
# part 2:
#   basic style setting for the histogram.
#
# part 3:
#   fit the histogram
#
# part 4:
#   read in the histogram stored in the root file we just created.



# functions for fitting.
class myGaus_with_bg:
    def __call__( self, t, par ):
        a = par[0] #amplitude
        c = par[1] #center
        s = par[2] #sigma
        b = par[3] #bg
        x = t[0]   
        tmp = -1.*(x-c)*(x-c)/2. /(s*s)
        return a * math.exp( tmp ) + b

class myGaus:
    def __call__( self, t, par ):
        a = par[0] #amplitude
        c = par[1] #center
        s = par[2] #sigma
        x = t[0]   
        tmp = -1.*(x-c)*(x-c)/2. /(s*s)
        return a * math.exp( tmp )
  


#=============================================
# part 1 
c1 = ROOT.TCanvas( 'c1', 'canvas title' )
# create a canvas (TCanvas)
# params
#   name: 'c1', which can be used to retrieve the canvas.
#   title: 'canvas title', which will shown on the canvas windows

 


f = ROOT.TFile( 'my_output.root', 'recreate' )
# create a output file (TFile), for writing out.
# 'recreate' means to overwrite the file. 


# create an empty histogram.
xbin = 100
xmin = -3.
xmax =  3.
h1 = ROOT.TH1F( 'h1', 'histogram 1; x axis; y axis', xbin, xmin, xmax )
h2 = ROOT.TH1F( 'h2', 'histogram 2; x axis; y axis', xbin, xmin, xmax )
# TH1F, TH1D, TH1I, TH1C ... belongs to TH1 class.
# for example, TH1C is for char type bin.
# so one single bin can store the data within -128..127


# fill the random number in to the histogram.
# we use global object gRandom
# gRandom has a method called: Gaus( c, sigma )
# in ROOT, g prefix is for Global variable.
# we have gStyle, gRandom, gSystem,.....
center = 0.1
sigma  = 0.5 
for i in range( 10000 ):
    g = ROOT.gRandom.Gaus(center, sigma)
    k = ROOT.gRandom.Uniform(-3,3)  # for background.
    h1.Fill( g )
    h2.Fill( g ); h2.Fill( k )
    


# draw the histogram into the canvas.
h2.Draw()
h1.Draw("same")
c1.Update()

h1.Write() 
# write the histogram h1 into "my_output.root" file.



#=============================================
# part 2
# style setting.

if( 0 ):
    h2.SetLineWidth(3 )
    h2.SetLineStyle( 3 )
    h2.SetLineColor( ROOT.kRed )

    h2.GetXaxis().SetTitle("x axis for h2")
    h2.GetYaxis().SetTitle("y axis for h2")
   
    c1.Update()
     
   
    
    


#=============================================
# part 3
# simple fitting.

#
# fit h1
#
if( 0 ):
    h1.Draw()
    c1.Update() # good to put this when using PyRoot.
   
    # set up our fit function 'func'
    fit_range1 = -3.
    fit_range2 =  3.
    parN = 3
    func = ROOT.TF1("myfunction1", myGaus(), fit_range1, fit_range2, parN )
    
    # set initial value.
    func.SetParameters(1., 0., 0.5)
    
    h1.Fit( func ,"Q" )
    # "Q" for quiet, it will not print message to screen.

    # by default, we will have the fitting curve attach to h1 object,
    # but in pyroot, we have to call canvas update() to show the fitting curve.
    c1.Update() 
    
    fit_results = func.GetParameters()
    print " for h1 :"
    print " ampitude = %6.2f\n center = %6.2f\n sigma = %6.2f " \
            %(fit_results[0], fit_results[1], fit_results[2] )    

    pass



#
# fit h2
#
if( 0 ):
    h2.Draw()
    c1.Update()
    
    fit_range1 = -3.
    fit_range2 =  3.
    parN = 4
    func2 = ROOT.TF1("myfunction2", myGaus_with_bg(), fit_range1, fit_range2, parN )
    
    # set initial value.
    func2.SetParameters( 600, 0, 1, 100 )
    
    h2.Fit( func2 ,"Q" )
    # "Q" for quiet, it will not print message to screen.

    # by default, we will have the fitting curve attach to h1 object,
    # but in pyroot, we have to call canvas update() to show the fitting curve.
    c1.Update() 
    
    fit_results = func2.GetParameters()
    print " for h2 :"
    print " ampitude = %6.2f\n center = %6.2f\n sigma = %6.2f\n bg = %6.2f " \
            %(fit_results[0], fit_results[1], fit_results[2], fit_results[3]  )    

    pass




#=============================================
# part 4 

if( 1 ):
    c1.Clear()
    f.Close() 
    # for pyroot,  we will make the type of h1
    # become "PyROOT_NoneType" if we close the TFile.
    # ( it is not true for ROOT )
    print( type(h1) ) #it prints " <type 'PyROOT_NoneType'>
    del h1

    f2 = ROOT.TFile("my_output.root")
 
    h1New = f2.Get("h1")
    h1New.Draw()
    c1.Update() 

 
