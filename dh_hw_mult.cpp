#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"
#include "shift_register.h"
#include "bitwise.h"

typedef enum{
	state_wait = 0,
	state_execute,
	state_output,
	state_finish
}hw_mult_states;

void dh_hw_mult::process_hw_mult()
{
	
  NN_DIGIT a[2], b, c, t, u;
  NN_HALF_DIGIT bHigh, bLow, cHigh, cLow;
  hw_mult_states hw_mult_curr_state = state_wait;

  BITWISE bitwise1(); 
  BITWISE bitwise2();
  SHIFT_REGISTER1();

  for (;;) {  

    switch(hw_mult_curr_state){
      case state_wait:
        std::cout << "state wait (hw_mult)"  << std::endl;
        if(hw_mult_enable.read() == true){
          hw_mult_curr_state = state_execute;
        }
        wait();
	break;
      case state_execute:
        std::cout << "state execute (hw_mult)"  << std::endl;
	// Read inputs	
	b = in_data_1.read();
	c = in_data_2.read();
		
	// Original code from NN_DigitMult()...		
  	bHigh = (NN_HALF_DIGIT)HIGH_HALF (b);
        std::cout << b << std::endl;
        std::cout << bHigh << std::endl;
        
  	bLow = (NN_HALF_DIGIT)LOW_HALF (b);
  	cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
  	cLow = (NN_HALF_DIGIT)LOW_HALF (c);

  	a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
  	t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
  	u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
  	a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;
  
  	if ((t += u) < u) a[1] += TO_HIGH_HALF (1);
  	u = TO_HIGH_HALF (t);
  
  	if ((a[0] += u) < u) a[1]++;
  	a[1] += HIGH_HALF (t);

        wait();
        hw_mult_curr_state = state_output;
        break;
      case state_output:
        std::cout << "state output (hw_mult)"  << std::endl;
	// Write outputs
	out_data_low.write(a[0]);
	out_data_high.write(a[1]);
	hw_mult_curr_state = state_finish;
        hw_mult_done.write(true);
        wait();
        break;
      case state_finish:
        std::cout << "state finish (hw_mult)"  << std::endl;
        if(hw_mult_enable.read() == false){
          hw_mult_done.write(false);
          hw_mult_curr_state = state_wait;
        }
        wait();
        break;
    }
  
    /*
    if (hw_mult_enable.read() == true) 
    {	

	// Read inputs	
	b = in_data_1.read();
	c = in_data_2.read();
		
	// Original code from NN_DigitMult()...		
  	bHigh = (NN_HALF_DIGIT)HIGH_HALF (b);
  	bLow = (NN_HALF_DIGIT)LOW_HALF (b);
  	cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
  	cLow = (NN_HALF_DIGIT)LOW_HALF (c);

  	a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
  	t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
  	u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
  	a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;
  
  	if ((t += u) < u) a[1] += TO_HIGH_HALF (1);
  	u = TO_HIGH_HALF (t);
  
  	if ((a[0] += u) < u) a[1]++;
  	a[1] += HIGH_HALF (t);
		
 	// Hardware multiplication delay = 100 ns
	wait (100, SC_NS);
	
	// Write outputs
	out_data_low.write(a[0]);
	out_data_high.write(a[1]);
		
    }

    wait();		// wait for a change of hw_mult_enable	
    */
  }
	  	  
}

