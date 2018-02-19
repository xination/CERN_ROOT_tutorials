#include "TObject.h"

class particle_CLASS: public TObject  {

public:
        Float_t position[3]; 
        Float_t energy; 
         

public:
    particle_CLASS() { } // empty constructor


    //----Setter----
    void Set_position(Float_t x, Float_t y, Float_t z) 
    {
        position[0] = x; position[1] = y; position[2] = z;        
    }
    
    void Set_energy(Float_t e) { energy = e;}


    //----Getter----
    Float_t* Get_position() { return position; }
    Float_t  Get_energy()   { return energy; }



    Int_t test_only() const { return 0; } 
    // for a method with 'const' in the end, meaning it cannot change the 
    // private member data.
    // this method will be write out to the root file.





    ClassDef(particle_CLASS,1)  
    /*
    |   ClassDef is a C preprocessor macro 
    |   that must be used if your class derives from TObject.
    */
};

