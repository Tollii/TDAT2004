#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>
#include <stack>

using namespace std;

class PrimeNumbers {

private:
    bool isPrime(int &n) {
        if(n == 0 || n == 1)
            return false;
        if(n == 2)
            return true;
        if(n > 2 && n % 2 == 0)
            return false;

        int maxDivisor = (int)floor(sqrt(n));

        for(int i = 3; i < maxDivisor; i += 2) {
            if(n % i == 0)
                return false;
        }
        return true;
    }

public:

    vector<int> primeRange(int start, int end, int &threadNum) {
        vector<int> primeNumbers;
        mutex s_mutex;
        vector<thread> threads;
        stack<int> s;

        threads.reserve(threadNum);
        for (;start < end; start++) {
            s.push(start);
        }

        // Each thread calculates the primes in their own range
        for(int i = 0; i < threadNum; i++) {

            threads.emplace_back(thread([&] {
                while (!s.empty()) {
                    s_mutex.lock();
                    int num = s.top();
                    s.pop();

                    if (isPrime(num)){
                        primeNumbers.push_back(num);

                    }
                    s_mutex.unlock();
                }
            }));
        }
        for (auto &thread : threads)
            thread.join();

        return primeNumbers;
    }
};


int main() {
    int start = 5000;
    int end = 10000;
    int threads = 10;
    PrimeNumbers primes;

    cout << "\nMulti-threaded prime numbers..." << endl;
    vector<int> primeThreads = primes.primeRange(start, end, threads);
    for(int &primeNum : primeThreads) {
        cout << primeNum << " is a prime number."<< " Address is " << &primeNum << endl;
    }

}
