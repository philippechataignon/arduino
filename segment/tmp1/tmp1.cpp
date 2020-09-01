int main()
{
    std::bitset < 5 > x;

    x[1] = 1;
    x[2] = 0;
    // Note x[0-4]  valid

    std::cout << x << std::endl;
}
