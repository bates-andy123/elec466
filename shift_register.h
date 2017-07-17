SC_MODULE (SHIFT_REGISTER)
{
  sc_in<NN_DIGIT> input;
  sc_in<NN_DIGIT> shiftBy;
  sc_out<NN_DIGIT> output;

  //void process_bitwise();

  SC_CTOR(SHIFT_REGISTER)
  {
    SC_THREAD(process_shift_register);
  }

  void process_shift_register()
  {
    output.write(input.read() >> shiftBy.read());
  }
};
