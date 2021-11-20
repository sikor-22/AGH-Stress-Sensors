#include <thread>
#include <iostream>
#include <mutex>
#include <string>
#include <chrono>
#include <ctime>
#include <fstream>
#include "Adafruit_ADS1015.h"
#include "common_header.h"
#define EKG 0
#define GSR 1
#define EMG1 2
#define EMG2 3




int main_loop(){
    adc.setGain(GAIN_FOUR);
    adc.begin();
    while(run){
        file_ekg.open("thread_test_ekg.txt", std::ios::out);
        file_rest.open("thread_test_rest.txt", std::ios::out);
        if(!file_ekg.is_open() || !file_rest.is_open()){
            return 1;
        }
        std::thread EKG_thread(collect_from_EKG);
        std::thread Ev_else(collect_everything_else);
        EKG_thread.join();
        Ev_else.join();
        file_ekg.close();
        file_rest.close();
    }
    return 0;
}


int main(){
    adc.setGain(GAIN_FOUR);
    adc.begin();
    
    std::srand(std::time(nullptr));
    int retVal = main_loop();
    std::cout<<"Collecting data... type q to stop\n";
    std::thread InputListen(listen_on_stdin);
    if(retVal){
        run = false;
        std::cout<<"Collecting Failed with error code: "<<retVal<<"\n";
    }
    InputListen.join();
}