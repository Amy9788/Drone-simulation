#include "TransitServer.h"
#include <fstream>
#include <iostream>

TransitServer::TransitServer(){
    std::ofstream myfile;
    myfile.open("libs/transit/mgsInfo.txt");
    myfile << "";
    myfile.close(); 
    currMsg = "";
}

void TransitServer::Update(std::string msg){
    std::ofstream myfile;
    myfile.open("libs/transit/mgsInfo.txt", std::ofstream::app);
    myfile << msg << "\n";
    myfile.close(); 
    currMsg = msg + "\n";
}

std::string TransitServer::GetCurrentMsg(){
    return currMsg;
}
