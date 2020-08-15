int byte_read = 0; ///< The current byte
int recieved_integer = 0; ///< The current recieved integer

void setup() {
  Serial.begin(9600);
}

boolean is_a_number(int n)
{
  return n >= 48 && n <= 57;
}

int ascii2int(int n, int byte_read)
{
  return n*10 + (byte_read - 48);
}

void loop() {
  recieved_integer = 0;
  while ( Serial.available() ) {
    byte_read = Serial.read();
    if ( is_a_number(byte_read) ) {
          recieved_integer = ascii2int(recieved_integer, byte_read);
    }
  }
  Serial.println( recieved_integer );
  delay(1000);
}
