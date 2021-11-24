#include <thread>
#include <iostream>
#include <sstream>
#include <iomanip>
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

std::string ekg_filename;
std::string rest_filename;


int main_loop(){
    std::cout<<"Collecting data... type q to stop\n";
    std::thread InputListen(listen_on_stdin);
    while(run){
        file_ekg.open(ekg_filename, std::ios::app);
        file_rest.open(rest_filename, std::ios::app);
        if(!file_ekg.is_open() || !file_rest.is_open()){
            run = false;
            InputListen.join();
            return 1;
        }
        std::thread EKG_thread(collect_from_EKG);
        std::thread Ev_else(collect_everything_else);
        EKG_thread.join();
        Ev_else.join();
        file_ekg.close();
        file_rest.close();
    }
    InputListen.join();
    return 0;
}

void create_files(){
    auto now  =  std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ekg_filestream;
    std::stringstream rest_filestream;
    ekg_filestream<<std::put_time(std::localtime(&in_time_t), "EKG_%m%d%X");
    rest_filestream<<std::put_time(std::localtime(&in_time_t), "Others_%m%d%X");
    ekg_filename = ekg_filestream.str();
    rest_filename = rest_filestream.str();
}

int main(){
    adc.setGain(GAIN_FOUR);
    adc.begin();
    file_ekg.open(ekg_filename, std::ios::out);
    file_rest.open(rest_filename, std::ios::out);
    file_ekg.close();
    file_rest.close();
    std::srand(std::time(nullptr));
    int retVal = main_loop();
    
    if(retVal){
        run = false;
        std::cout<<"Collecting Failed with error code: "<<retVal<<" \n";
    }
}
