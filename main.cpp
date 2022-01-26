#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <functional>

std::vector<std::thread> waitevents;
std::vector<std::function<void()>> waitfunc;
std::vector<bool> waitemits;
int tindex = 0;

void captureemit() {

    int finished_processes = 0;
    
    while (true) {

        for (int i = 0; i < waitemits.size(); i++) {
            
            if (waitemits.at(i) == true) {
                finished_processes++;
                waitemits.at(i) = false;
                waitfunc.at(i)();
            }
        }

        if (finished_processes == waitemits.size()) break;
    }
}

void tcaptureemit() {
    std::thread obj(captureemit);
    waitevents.push_back(move(obj));
}

void _wait(int seconds, int i) {
    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(seconds));
    bool res = true;
    waitemits.at(i) = true;
}
void twait(int seconds, std::function<void()> function) {
    std::thread obj(_wait, seconds, tindex);
    waitevents.push_back(move(obj));
    waitemits.push_back(false);
    waitfunc.push_back(function);
    tindex++;
}

void printhello() {
    std::cout << "hello" << '\n';
}
void printhi() {
    std::cout << "hi" << '\n';
}

int main() {

    // captures the end of the sleep call
    tcaptureemit();

    twait(1, printhello);
    twait(4, printhi);

    // main code here
    // [...]
    // printhello();

    // at the end of the code block
    for (int i = 0; i < waitevents.size(); i++) {
        waitevents.at(i).join();
    }
}
