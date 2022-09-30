#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>

using namespace std;
mutex m;
condition_variable cv;
string dataSrc;

bool ready = false;
bool processed = false; 

void work_thread() {
    unique_lock<mutex> lk(m);
    cv.wait(lk, [] {return ready;});

    cout << "Worker thread is processing data" << endl;
    dataSrc += " after processing";

    //send data to main
    processed = true;
    cout << "Worker thread finish processing" << endl;

    cv.notify_one();
}

int main() {
    thread worker(work_thread);

    dataSrc = "Example";
    {
        lock_guard<mutex> lk(m);
        ready = true;
        cout << "main() data ready" << endl;
    }
    cv.notify_one();

    {
        lock_guard<mutex> lk(m);
        cout << "processed=" << processed << endl;
        cv.wait(lk, [] {return processed;});
        cout << "processed=" << processed << endl;
    }
    worker.join();
    cout<<"Main finished"<<endl;
}