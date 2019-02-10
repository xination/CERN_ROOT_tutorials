
// This script demo how to set customize color
// color picker
// https://www.w3schools.com/colors/colors_picker.asp 
void _01_customized_color() {


    int rgb [] = { 102, 255, 225 }; // light blue
    // normally, each r, g, b ranges from 0 to 255,
    // but in ROOT, each r, g, b range from 0 to 1.
    // so you have to divide them by 255.

    // to use a customize color,
    // we have to find a "free" color index to register the new color 
    // it can be done by TColor::GetFreeColorIndex()
    // then construct the color.
    
    int myColor_idx = TColor::GetFreeColorIndex(); // get the free index.

    TColor* myColor = new TColor( myColor_idx, rgb[0]/255., rgb[1]/255., rgb[2]/255.  ); 
    // use free index, and rgb values to instantiate TColor object.


    // part 2
    // demo for how to use customized color.
     
    TF1* func = new TF1("func", "sin(x)", -1, 1 );

    // put in the color idx  
    func->SetLineColor( myColor_idx  ); 
    
    func->SetLineWidth( 10 );
    func->Draw();
}

