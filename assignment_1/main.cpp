#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>

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
    vector<int> primeRange(int start, const int &end) {
        vector<int> primeNumbers;
        for(; start < end; start++) {
            if(isPrime(start))
                primeNumbers.push_back(start);
        }
        return primeNumbers;
    }

    vector<int> primeRange(int start, int end, int &threadNum) {
        vector<int> primeNumbers;
        mutex prime_mutex;
        vector<thread> threads;
        threads.reserve(threadNum);
        int range = (end - start) / threadNum;
        end = start + range;

        // Each thread calculates the primes in their own range
        for(int i = 0; i < threadNum; i++) {
            threads.emplace_back(thread([&] {
                lock_guard<mutex> lock(prime_mutex);
                vector<int> primeThreads = primeRange(start, end);
                start += range;
                end += range;
                primeNumbers.reserve(primeNumbers.size() + primeThreads.size());
                primeNumbers.insert(primeNumbers.end(), primeThreads.begin(), primeThreads.end());
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

    cout << "\nSingle-threaded prime numbers..." << endl;
    vector<int> primeSingle = primes.primeRange(start, end);
    for(int &primeNum : primeSingle) {
        cout << primeNum << " is a prime number."<< " Address is " << &primeNum << endl;
    }

    cout << "\nMulti-threaded prime numbers..." << endl;
    vector<int> primeThreads = primes.primeRange(start, end, threads);
    for(int &primeNum : primeThreads) {
        cout << primeNum << " is a prime number."<< " Address is " << &primeNum << endl;
    }

     // Checks if both vectors are equal
    for(int i = 0; i < primeSingle.size(); i++) {
        if(primeSingle[i] != primeThreads[i]){
            cout << "Not equal!" << endl;
            cout << "Single thread: " << primeSingle[i] << " --- ";
            cout << "Multi thread: " << primeThreads[i] << endl;
        }
    }
}
