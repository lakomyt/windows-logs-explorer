#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "pugixml.hpp"
#include "Message.h"
using namespace std;
using namespace pugi;

string get_logs(string param){
    string command = "wevtutil"+param;

    const char * c = command.c_str();
    char psBuffer[128];
    FILE *pPipe;

    if((pPipe = popen( c, "rt" )) == NULL){
        exit(1);
    }

    vector <string> out;
    string str = "";
    while(fgets(psBuffer, 128, pPipe)){
        str += psBuffer;
    }
    
    if (!feof(pPipe)){
        cout << "Error: Failed to read the pipe to the end." << endl;
    }

    return str;
}

int main(){
    string log_groups[] = {"Application", "HardwareEvents", "Security", "System"};
    for(int l=0; l<4; l++){
        cout << l+1 << ". " << log_groups[l] << endl;
    }
    cout << "Choose log type: ";
    int log_type;
    cin >> log_type;
    log_type--;

    cout << endl << "LOG INFO" << endl << get_logs(" gl "+log_groups[log_type]) << endl;

    string logs = get_logs(" qe "+log_groups[log_type]);
    vector<Message> messages;
    xml_document doc;
    xml_parse_result result;
    result = doc.load_string(logs.c_str());

    int max_event_data = 0;

    for(xml_node event: doc.children("Event")){
        Message *mess = new Message();
        xml_node system = event.child("System");
        mess->provider = system.child("Provider").attribute("Name").value();
        mess->eventID = system.child("EventID").text().as_int();
        mess->level = system.child("Level").text().as_int();
        mess->time = system.child("TimeCreated").attribute("SystemTime").value();
        mess->computer = system.child("Computer").text().as_string();

        xml_node eventdata = event.child("EventData");
        vector<string> Event_data_names;
        vector<string> Event_data_values;
        for(xml_node data: eventdata.children("Data")){
            mess->event_data_names.push_back(data.attribute("Name").value());
            mess->event_data_values.push_back(data.text().as_string());
        }
        if(mess->event_data_names.size() > max_event_data){
            max_event_data = mess->event_data_names.size();
        }

        messages.push_back(*mess);
    }

    ofstream out;
    out.open(log_groups[log_type]+".csv");
    out << "provider,eventid,level,time,computer";
    for(int i=1; i<=max_event_data; i++){
        out << ",eventdata" << i;
    }
    out << endl;
    for(auto & m : messages){
        out << m.provider << ",";
        out << m.eventID << ",";
        out << m.level << ",";
        out << m.time << ",";
        out << m.computer;
        for(int i=0; i<m.event_data_names.size(); i++){
            out << "," << m.event_data_names[i] << ": " << m.event_data_values[i];
        }
        out << endl;
    }
    out.close();

    cout << messages.size() << " events exported to `" << log_groups[log_type] << ".csv`." << endl;

    return 0;
}