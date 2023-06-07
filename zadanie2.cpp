#include <iostream>
#include <thread>
#include <vector>
#include <stdlib.h>
#include <queue>
#include <random>
#include <mutex>
#include <fstream>
#include <thread>
#include <atomic>


/* Napisz program analogiczny do Zadania 1 tak, aby wykorzystywał bufor cykliczny. */


namespace
{

    int random_int2(int min, int max) {
        static std::mt19937 random_number_engine;
        static std::mutex rng_mtx;
        std::uniform_int_distribution<int> distribution(min, max);
        std::lock_guard<std::mutex> lk_guard(rng_mtx);
        int retval = distribution(random_number_engine);
        return retval;
    }

    void producent2(int tMinProd, int tMaxProd, std::vector<std::string> &array, size_t &head, size_t &tail)
    {
        while(1)
        {
            std::string temp;
            temp.resize(10);
            for (int i = 0; i < 10; i++) {
                temp[i] = random_int2('a', 'z');
            }

            {
                if(head == tail - 1 || ((head == array.size() - 1) && (tail == 0)))
                {
                    std::cerr << "Error: " << temp << std::endl;
                }
                else
                {
                    array[head] = temp;
                    head++;
                    if(head == array.size())
                    {
                        head = 0;
                    }
                }
            }

            int time = random_int2(tMinProd, tMaxProd);

            std::this_thread::sleep_for(std::chrono::milliseconds(time));
        }
    }

    void consumer2(int tCon, std::vector<std::string> &array, size_t &head, size_t &tail)
    {
        while(1)
        {
            std::ofstream textFile;

            {
                while (head != tail)
                {
                    textFile.open("text.txt", std::ios::app);
                    textFile << array[tail] + "\n";
                    array[tail].clear();
                    textFile.close();
                    tail++;
                    if(tail == array.size())
                    {
                        tail = 0;
                    }
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(tCon));
        }
    }

    int zadanie2()
    {
        // minimalny okres pracy producenta miliseconds
        int tMinProd = 10;

        // maksymalny okres pracy producenta miliseconds
        int tMaxProd = 50;

        // okres pracy konsumenta miliseconds
        int tCon = 300;

        // maksymalna głębokość kolejki
        size_t qMaxDepth = 10;

        std::cout << "Zadanie 2" << std::endl;
        std::vector<std::string> array;
        array.resize(qMaxDepth);
        size_t head = 0;
        size_t tail = 0;

        std::atomic<bool> stop_flag = false;

        std::thread tProducent(producent2, tMinProd, tMaxProd, std::ref(array), std::ref(head), std::ref(tail));
        std::thread tConsumer(consumer2, tCon, std::ref(array), std::ref(head), std::ref(tail));
        tProducent.join();
        tConsumer.join();

        char quit = ' ';

        std::cout << std::endl;
        std::cout << "Press q/Q to go back to menu: ";
        std::cin >> quit;

        while(quit != 'q' && quit != 'Q')
        {
            std::cin >> quit;
        }

        system("CLS");

        if(quit == 'q' || quit == 'Q')
        {
            return 0;
        }

        return 1;
    }

}
