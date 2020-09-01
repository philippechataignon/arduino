void putdec(short int n, char buff[])
{
    unsigned char d4, d3, d2, d1, d0, q;
    char* p = buff;

    if (n < 0) {
        *p++ = '-';
        n = -n;
    }

    d1 = (n>>4)  & 0xF;
    d2 = (n>>8)  & 0xF;
    d3 = (n>>12) & 0xF;

    d0 = 6*(d3 + d2 + d1) + (n & 0xF);
    q = (d0 * 0xCD) >> 11;
    d0 = d0 - 10*q;

    d1 = q + 9*d3 + 5*d2 + d1;
    q = (d1 * 0xCD) >> 11;
    d1 = d1 - 10*q;

    d2 = q + 2*d2;
    q = (d2 * 0x1A) >> 8;
    d2 = d2 - 10*q;

    d3 = q + 4*d3;
    d4 = (d3 * 0x1A) >> 8;
    d3 = d3 - 10*d4;

    *p++ = d4 + '0';
    *p++ = d3 + '0';
    *p++ = d2 + '0';
    *p++ = d1 + '0';
    *p++ = d0 + '0';
}

char buffer[8] = {0};
int i = 12345;

void setup() {
    Serial.begin(9600);
    putdec(i, buffer);
    Serial.println(buffer);
}

void loop()
{}
