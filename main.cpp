#include <iostream>
#include <thread>
#include <chrono>
#include <stdlib.h>

#include "zadanie1.cpp"
#include "zadanie2.cpp"
#include "zadanie3.cpp"


int main()
{
    int wybor;
    int back = 1;

    system("CLS");

    std::cout << "Wybierz, ktory z programow chcesz uruchomic: " << std::endl;
    std::cout << "1. Zadanie 1. - producent i konsument" << std::endl;
    std::cout << "2. Zadanie 2. - bufor cykliczny" << std::endl;
    std::cout << "3. Zadanie 3. - komunikacja z czujnikami" << std::endl;
    std::cin >> wybor;

    while( wybor != 1 && wybor != 2 && wybor != 3 )
    {
        std::cout << "Wrong number! Try again:";
        std::cin >> wybor;

        system("CLS");
    }

    system("CLS");

    switch(wybor)
    {
    case 1:
        back = zadanie1();
        if(back == 0)
        {
            system("CLS");
            main();
        }
        break;
    case 2:
        back = zadanie2();
        if(back == 0)
        {
            system("CLS");
            main();
        }
        break;
    case 3:
        back = zadanie3();
        if(back == 0)
        {
            system("CLS");
            main();
        }
        break;
    }
    return 0;
}
