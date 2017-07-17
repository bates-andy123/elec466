SC_MODULE(BITWISE)
{
  sc_in<NN_DIGIT> input1;
  sc_in<NN_DIGIT> input2;
  sc_out<NN_DIGIT> output;

  SC_CTOR(BITWISE)
  {
    SC_THREAD(process_bitwise);
  }

  void process_bitwise()
  {
    output.write(input1.read() | input2.read());
  }
};
