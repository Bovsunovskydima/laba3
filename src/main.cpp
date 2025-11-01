#include <iostream>
#include <thread>
#include <syncstream>
#include <latch>
#include <chrono>
#include <fcntl.h>
#include <io.h>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

constexpr int A = 5, B = 6, C = 4, D = 8, E = 9, F = 6, G = 6, H = 9, I = 7, J = 6;

void f(char action_set, int action_number) {
    std::osyncstream(std::cout) << "З набору " << action_set 
                                 << " виконано дію " << action_number << ".\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void worker1(std::latch& start, std::latch& done_a) {
    start.wait();
    for (int i = 1; i <= A; i++) f('a', i);
    done_a.count_down();
}

void worker2(std::latch& start, std::latch& done_a, std::latch& done_b, 
             std::latch& done_c, std::latch& done_e, std::latch& done_i) {
    start.wait();
    
    for (int i = 1; i <= B; i++) f('b', i);
    done_b.count_down();
    
    done_a.wait();
    for (int i = 1; i <= C; i++) f('c', i);
    done_c.count_down();
    
    done_c.wait();
    for (int i = 1; i <= E; i++) f('e', i);
    done_e.count_down();
    
    done_e.wait();
    for (int i = 1; i <= I; i++) f('i', i);
    done_i.count_down();
}

void worker3(std::latch& start, std::latch& done_c, std::latch& done_d, 
             std::latch& done_g, std::latch& done_h) {
    start.wait();
    
    done_c.wait();
    for (int i = 1; i <= D; i++) f('d', i);
    done_d.count_down();
    
    for (int i = 1; i <= G; i++) f('g', i);
    done_g.count_down();
    
    done_d.wait();
    done_g.wait();
    for (int i = 1; i <= H; i++) f('h', i);
    done_h.count_down();
}

void worker4(std::latch& start, std::latch& done_a, std::latch& done_b, 
             std::latch& done_f, std::latch& done_i, std::latch& done_h) {
    start.wait();
    
    done_a.wait();
    done_b.wait();
    for (int i = 1; i <= F; i++) f('f', i);
    done_f.count_down();
    
    done_i.wait();
    done_h.wait();
    done_f.wait();
    for (int i = 1; i <= J; i++) f('j', i);
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    if (!_isatty(_fileno(stdout))) {
        std::cout << "\xEF\xBB\xBF";
    }
#endif
    
    std::osyncstream(std::cout) << "Обчислення розпочато.\n";
    
    std::latch start{1};
    std::latch done_a{1}, done_b{1}, done_c{1}, done_d{1}, done_e{1};
    std::latch done_f{1}, done_g{1}, done_h{1}, done_i{1};
    
    {
        std::jthread t1(worker1, std::ref(start), std::ref(done_a));
        std::jthread t2(worker2, std::ref(start), std::ref(done_a), std::ref(done_b), 
                        std::ref(done_c), std::ref(done_e), std::ref(done_i));
        std::jthread t3(worker3, std::ref(start), std::ref(done_c), std::ref(done_d), 
                        std::ref(done_g), std::ref(done_h));
        std::jthread t4(worker4, std::ref(start), std::ref(done_a), std::ref(done_b), 
                        std::ref(done_f), std::ref(done_i), std::ref(done_h));
        
        start.count_down();
    }
    
    std::osyncstream(std::cout) << "Обчислення завершено.\n";
    
    return 0;
}
