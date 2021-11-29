#pragma once
#include <thread>
#include <iostream>
#include <mutex>
#include <string>
#include <chrono>
#include <ctime>
#include <fstream>
#include "Adafruit_ADS1015.h"
#define EKG 0
#define GSR 1
#define EMG1 2
#define EMG2 3


Adafruit_ADS1015 adc;

auto start = std::chrono::high_resolution_clock::now();
auto program_begin = start;
std::mutex adc_lock; //access to adc is not likely to be thread safe

std::fstream file_ekg;
std::fstream file_rest;
bool run = true;

void listen_on_stdin()
{
    char c;
    std::cout<<"> ";
    std::cin>>c;
    while(c != 'q' && run){
        std::cout<<"> ";
        std::cin>>c;
    }
    run = false;
}

void collect_from_EKG(){
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::chrono::duration<double, std::milli> elapsed_from_begin = end - program_begin;
    while(elapsed.count() < 2000){
        adc_lock.lock();
            uint16_t data_EKG = adc.readADC_SingleEnded(EKG);
        adc_lock.unlock();
        file_ekg << elapsed_from_begin.count() << "," << data_EKG << '\n';
        end = std::chrono::high_resolution_clock::now();
        elapsed_from_begin = end - program_begin;
    }    
}

void collect_everything_else(){
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::chrono::duration<double, std::milli> elapsed_from_begin = end - program_begin;
    while(elapsed.count() < 2000){
        adc_lock.lock();
            uint16_t data = adc.readADC_SingleEnded(GSR);
        adc_lock.unlock();
        file_rest << elapsed_from_begin.count() << "," << data <<", ";
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        adc_lock.lock();
            data = adc.readADC_SingleEnded(EMG1);
        adc_lock.unlock();
        file_rest << data <<", ";
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        adc_lock.lock();
            data = adc.readADC_SingleEnded(EMG2);
        adc_lock.unlock();
        file_rest << data <<" \n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        end = std::chrono::high_resolution_clock::now();
        elapsed = end-start;
        elapsed_from_begin = end - program_begin;
    }
}
