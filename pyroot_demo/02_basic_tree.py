import ROOT 
 


#===============================================
# structure of this tutorial:
# part 1: 
#   load a c++ class.
#           
#
# part 2:
#   basic TTree method: Show, Scan, Draw 
#
# part 3:
#    manually loop through a tree 
#
# part 4:
#    write tree to a TFile



#=============================================
# part 1 
# to use the c++ 'particle_CLASS.h' 
# we can run ACliC by:
# $ root particle_CLASS.h+

# then we load in the class definition.
ROOT.gSystem.Load( "particle_CLASS_h" ) 
from ROOT import particle_CLASS



# instantiate an object from particle_CLASS
myParticle = particle_CLASS()

# create a tree, and set a branch to connect the 'myParticle'
t = ROOT.TTree("t", "my tree for demo") # params: key and title.
t.Branch( "particle", myParticle )    # params: branchName, obj

 
# add some entries with dummy data
myParticle.Set_position( 1, 2, 3 )
myParticle.Set_energy( 10 )
t.Fill()

myParticle.Set_position( 4, 5, 6 )
myParticle.Set_energy( 20 )
t.Fill()


myParticle.Set_position( 7, 8, 9 )
myParticle.Set_energy( 30 )
t.Fill()



#=============================================
# part 2 Basic TTree methods


if(0):
    # show method
    t.Show(0)
    t.Show(1)
    t.Show(2)

    # scan method
    t.Scan()
    t.Scan("particle.energy", "", "colsize=20")

    # ***********************************
    # *    Row   *      particle.energy *
    # ***********************************
    # *        0 *                   10 *
    # *        1 *                   20 *
    # *        2 *                   30 *
    # ***********************************



    t.Scan("particle.position", "", "colsize=20" )
    # **********************************************
    # *    Row   * Instance *    particle.position *
    # **********************************************
    # *        0 *        0 *                    1 *
    # *        0 *        1 *                    2 *
    # *        0 *        2 *                    3 *
    # *        1 *        0 *                    4 *
    # *        1 *        1 *                    5 *
    # *        1 *        2 *                    6 *
    # *        2 *        0 *                    7 *
    # *        2 *        1 *                    8 *
    # *        2 *        2 *                    9 *
    # **********************************************

    # t.Scan("particle.position:Iteration$", "", "colsize=20", )
    # Entry$   represents the current run number.
    # Entries$ represents the total run number.
    # Iteration$  returns the current index.
         

    # you can apply formula
    t.Scan("particle.position[0]* particle.position[0]", "", "colsize=20", )
    # ***********************************
    # *    Row   * particle.position[0] *
    # ***********************************
    # *        0 *                    1 *
    # *        1 *                   16 *
    # *        2 *                   49 *
    # ***********************************
       

    # Draw method
    t.Draw("particle.energy", "", "goff") 

    # redirect
    # histo = ROOT.TH1F("histo", "histogram", 30, 0, 30);
    # t.Draw("particle.energy>>histo","particle.energy>15" )




#=============================================
# part 3 
# manually loop through a tree
# the best part of using PyROOT is that you don't need to 
# link the variables ( in C++ you need.)

 
if(1):

    for i in xrange( t.GetEntriesFast() ):
        
        t.GetEntry(i)

        # print t.particle.Get_energy() 
        print "eng = %7.2f" %t.particle.energy 

        print "x=%7.2f\ty=%7.2f\tz=%7.2f\n" \
            %( t.particle.position[0], t.particle.position[1], t.particle.position[2] )



#=============================================
# part 4 
# write tree to a TFile.
if(0):
    ouf = ROOT.TFile("myParticle_tree.root", "recreate" );
    t.Write()
    ouf.Close() 
 
