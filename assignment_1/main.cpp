#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>
#define class struct

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
    vector<int> primeRange(const int &start, const int &end) {
        vector<int> primeNumbers;
        for(int i = start; i < end; i++) {
            if(isPrime(i))
                primeNumbers.push_back(i);
        }
        return primeNumbers;
    }

    vector<int> primeRangeThreads(const int &start, const int &end, int &threadNum) {
        vector<int> primeNumbers;
        int length = end - start;
        int load = length / threadNum;
        mutex prime_mutex;
        vector<thread> threads;
        threads.reserve(threadNum);
        int startTemp = start;
        int loadTemp = load;

        for(int i = 0; i < threadNum; i++) {
            threads.emplace_back(thread([&, i] {
                // Each thread calculates the prime in their own range
                lock_guard<mutex> lock(prime_mutex);
                vector<int> primeThreads = primeRange(startTemp, loadTemp);
                startTemp = loadTemp + 1;
                loadTemp += load;
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
    int start = 0;
    int end = 2000;
    int threads = 10;
    PrimeNumbers primes;

    vector<int> primeSingle = primes.primeRange(start, end);
    vector<int> primeThreads = primes.primeRangeThreads(start, end, threads);

    cout << " Single-threaded prime numbers:" << endl;
    for(int &primeNum : primeSingle) {
        cout << primeNum << " is a prime number."<< " Address is " << &primeNum << endl;
    }

    cout << " Multi-threaded prime numbers:" << endl;
    for(int &primeNum : primeThreads) {
        cout << primeNum << " is a prime number."<< " Address is " << &primeNum << endl;
    }

    // Checks if both vectors are equal
    for(int i = 0; i < primeSingle.size(); i++) {
        if(primeSingle[i] != primeThreads[i])
            cout << "Not equal!" << endl;
    }
}
