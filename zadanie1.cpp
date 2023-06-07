#include <iostream>
#include <thread>
#include <vector>
#include <stdlib.h>
#include <mutex>
#include <queue>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>


/* Napisz program, który będzie pokazywał działanie producenta i konsumenta na
 * przykładzie produktów - losowych ciągów znaków.

  Producent i konsument powinni pracować w dwóch wątkach, wykonujących się w pętli
  nieskończoność:
  - Producent: cyklicznie generuje losowe 10-znakowe ciągi i próbuje umieścić je
  w buforze, a następnie czeka przez losowy czas (ograniczony parametrami). Jeśli
  bufor jest pełny, wygenerowany „produkt” zostaje porzucony, a do konsoli, do
  strumienia błędów (stderr) zostaje wypisany odpowiedni komunikat.
  - Konsument: sprawdza czy w buforze są jakieś elementy, jeśli tak - pobiera je
  i zapisuje do pliku, każdy w oddzielnej linii, następnie czeka przez czas
  określony parametrem.

  Program powinien mieć następujące parametry, definiowane stałymi w kodzie:
  - minimalny okres pracy producenta
  - maksymalny okres pracy producenta
  - okres pracy konsumenta
  - maksymalna głębokość kolejki

  Napisz program w dwóch wersjach:
  - Kolejka FIFO 1-1
      Uruchom jednego producenta i jednego konsumenta, działających w dwóch wątkach
      i wymieniających się danymi poprzez współdzielony bufor.
  - Kolejka FIFO many-many
      Rozbuduj program z podpunktu a) o możliwość uruchomienia dowolnej liczby producentów
      i konsumentów podanej przez użytkownika. Każdy konsument powinien zapisywać dane w
      pliku o nazwie zawierającej swój ID wątku. */


namespace
{

    int random_int(int min, int max) {
        static std::mt19937 random_number_engine;
        static std::mutex rng_mtx;
        std::uniform_int_distribution<int> distribution(min, max);
        std::lock_guard<std::mutex> lk_guard(rng_mtx);
        int retval = distribution(random_number_engine);
        return retval;
    }

    void producent(int tMinProd, int tMaxProd, size_t qMaxDepth, std::queue<std::string> &queue, std::mutex &qMtx)
    {
        while(1)
        {
            std::string temp;
            temp.resize(10);
            for (int i = 0; i < 10; i++) {
                temp[i] = random_int('a', 'z');
            }

            {
                std::lock_guard<std::mutex> lk_guard(qMtx);
                if(queue.size() < qMaxDepth)
                {
                     queue.push(temp);
                }
                else
                {
                    std::cerr << "Error: " << temp << std::endl;
                }
            }

            int time = random_int(tMinProd, tMaxProd);

            std::this_thread::sleep_for(std::chrono::milliseconds(time));
        }
    }

    void consumerA(int tCon, std::queue<std::string> &queue, std::mutex &qMtx)
    {
        while(1)
        {
            std::ofstream textFile;

            {
                std::lock_guard<std::mutex> lk_guard(qMtx);
                while (!queue.empty())
                {
                    textFile.open("text.txt", std::ios::app);
                    textFile << queue.front() + "\n";
                    queue.pop();
                    textFile.close();
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(tCon));
        }
    }

    void consumerB(int tCon, std::queue<std::string> &queue, std::mutex &qMtx)
    {
        while(1)
        {
            std::ofstream textFile;

            {
                std::lock_guard<std::mutex> lk_guard(qMtx);
                while (!queue.empty())
                {
                    std::stringstream ss;
                    ss << std::this_thread::get_id();
                    textFile.open(ss.str() + ".txt", std::ios::app);
                    textFile << queue.front() + "\n";
                    queue.pop();
                    textFile.close();
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(tCon));
        }
    }

    void zadanie1a(int tMinProd, int tMaxProd, int tCon, size_t qMaxDepth)
    {
        std::cout << "Zadanie 1" << std::endl;
        std::queue<std::string> queue;
        std::mutex qMtx;

        std::thread tProducent(producent, tMinProd, tMaxProd, qMaxDepth, std::ref(queue), std::ref(qMtx));
        std::thread tConsumer(consumerA, tCon, std::ref(queue), std::ref(qMtx));
        tProducent.join();
        tConsumer.join();

    }

    void zadanie1b(int tMinProd, int tMaxProd, int tCon, size_t qMaxDepth)
    {
        std::cout << "Zadanie 2" << std::endl;
        std::queue<std::string> queue;
        std::vector<std::thread> threads;
        std::mutex qMtx;
        int numProd;
        int numCon;

        std::cout << "Podaj liczbe producentow: ";
        std::cin >> numProd;
        std::cout << "Podaj liczbe konsumentow: ";
        std::cin >> numCon;
        std::cout << std::endl;

        for(int i = 0; i < numProd; ++i)
        {
            threads.emplace_back(producent, tMinProd, tMaxProd, qMaxDepth, std::ref(queue), std::ref(qMtx));
        }
        for(int i = 0; i < numCon; ++i)
        {
            threads.emplace_back(consumerB, tCon, std::ref(queue), std::ref(qMtx));
        }
        for(auto &it : threads)
        {
            it.join();
        }
    }

    int zadanie1()
    {
        // minimalny okres pracy producenta miliseconds
        int tMinProd = 10;

        // maksymalny okres pracy producenta miliseconds
        int tMaxProd = 50;

        // okres pracy konsumenta miliseconds
        int tCon = 300;

        // maksymalna głębokość kolejki
        size_t qMaxDepth = 10;

        int choice;

        system("CLS");
        std::cout << "Choose which part of the exersize you want to run:" << std::endl;
        std::cout << "1. FIFO queue 1-1" << std::endl;
        std::cout << "2. FIFO queue many-many" << std::endl;
        std::cin >> choice;
        system("CLS");

        while( choice != 1 && choice != 2 )
        {
            std::cout << "Choose which part of the exersize you want to run:" << std::endl;
            std::cout << "1. FIFO queue 1-1" << std::endl;
            std::cout << "2. FIFO queue many-many" << std::endl;
            std::cout << "Wrong number! Try again:";
            std::cin >> choice;

            system("CLS");
        }

        switch(choice)
        {
        case 1:
            zadanie1a(tMinProd, tMaxProd, tCon, qMaxDepth);
            break;
        case 2:
            zadanie1b(tMinProd, tMaxProd, tCon, qMaxDepth);
            break;
        }

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
