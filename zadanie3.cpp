#include <iostream>
#include <thread>
#include <vector>
#include <stdlib.h>
#include <condition_variable>
#include <mutex>
#include <random>
#include <queue>
#include <utility>


/* W pewnym systemie występuje wiele czujników zwracających dane w nieregularnych
 * odstępach czasu. Napisz program, który będzie cyklicznie pobierał dane ze wszystkich
 * czujników (każdy czujnik w oddzielnym wątku), a po uzbieraniu w buforze 5 wartości,
 * wypisywał dane na ekranie w postaci:

    New data!: ID:wartosc ID:wartosc ID:wartosc ID:wartosc ID:wartosc

   Użytkownik wprowadza liczbę czujników n przy uruchomieniu programu, a ID przyjmują
   wartości od 0 do n-1.

   Do powiadamiania wątku realizującego wyświetlanie o nowych danych wykorzystaj zmienną
   warunkową. Komunikację z czujnikiem oraz odczyt danych symuluj dostarczoną funkcją
   double read_sensor(int id) */


namespace
{

    double read_sensor(int id){
        static thread_local std::mt19937 generator(id);
        std::uniform_int_distribution<int> sleep_distribution(100,(id+1)*1000);
        std::uniform_real_distribution<double> value_distribution(id,id+0.99);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_distribution(generator)));
        return value_distribution(generator);
    }

    void print(std::queue<std::pair<int, double> > &queue, std::mutex &q_mtx, std::condition_variable &q_cv, bool &ready)
    {
        while(1)
        {
            std::unique_lock<std::mutex> lock(q_mtx);

            q_cv.wait(lock, [&]{ return queue.size() >= 5; });

            std::cout << "New data!: ";
            for (int i = 0; i < 5; ++i)
            {
                std::cout << queue.front().first << ":" << queue.front().second << " ";
                queue.pop();
            }
            std::cout << std::endl;

            ready = true;
        }
    }

    void work(std::queue<std::pair<int, double> > &queue, int id, std::mutex &q_mtx, std::condition_variable &q_cv, bool &ready)
    {
        while(1)
        {
            if(ready)
            {
                std::unique_lock<std::mutex> lock(q_mtx);

                queue.push(std::pair<int, double>(id, read_sensor(id)));

                if(queue.size() == 5)
                {
                    ready = false;
                    q_cv.notify_one();
                }
            }
        }
    }

    int zadanie3()
    {
        std::vector<std::thread> threads;
        std::queue<std::pair<int, double> > queue;
        std::condition_variable q_cv;
        std::condition_variable s_cv;
        std::mutex q_mtx;
        int sensNum;
        bool ready = true;

        std::cout << "Zadanie 3:" << std::endl;

        std::cout << "Podaj liczbe czujnikow: ";
        std::cin >> sensNum;

        threads.emplace_back(print, std::ref(queue), std::ref(q_mtx), std::ref(q_cv), std::ref(ready));

        for(int i = 0; i < sensNum; ++i)
        {
            threads.emplace_back(work, std::ref(queue), i, std::ref(q_mtx), std::ref(q_cv), std::ref(ready));
        }

        system("CLS");
        std::cout << "Zadanie 3:" << std::endl;

        for(auto &it : threads)
        {
            it.join();
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
