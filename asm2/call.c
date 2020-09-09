//Assembly subroutine declaration - keeps the compiler from
//generating a warning concerning implicit declaration.
int AsmSubroutine(int iParam1, int iParam2);

//This is the main 'C' routine
int main()
{
    int iRetVal = 0;
    //Call to our assembler subroutine
    iRetVal = AsmSubroutine(1024, 16);
    return iRetVal;
}
