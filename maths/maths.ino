const int baudrate = 9600;
int byte_read = 0; ///< The current byte read.
int coords[2]; ///< Contains x and y.
int index = 0; ///< 0: reading x, 1: reading y.
int separator = 44; ///< The separator between the integers (44: ,)

void setup()
{
    Serial.begin(baudrate);
}


boolean is_a_number(int n)
{
  return n >= 48 && n <= 57;
}


int ascii2int(int n, int byte_read)
{
  return n*10 + (byte_read - 48);
}


void loop()
{
  coords[0] = 0;
  coords[1] = 0;
  index = 0;
  while ( Serial.available() > 0 )
    {
      byte_read = Serial.read();
      if ( is_a_number(byte_read) )
        {
          coords[index] = ascii2int( coords[index], byte_read );
        }
      else if ( byte_read == separator )
	{
	  ++index;
	}
    }

  if ( index )
    {
      Serial.print("x + y = ");
      Serial.println(coords[0] + coords[1]);
      
      Serial.print("x * y = ");
      Serial.println(coords[0] * coords[1]);
    }
  else
    {
      Serial.println();
    }
  delay(1000);
}
