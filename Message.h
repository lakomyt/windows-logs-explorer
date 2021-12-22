#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Message{
public:

    string provider;
    int eventID;
    int level;
    string time;
    string computer;
    vector<string> event_data_names;
    vector<string> event_data_values;

    Message();
    Message(string provider, int EventID, int level, string time, string computer, vector<string> event_data_names, vector<string> event_data_values);
};

ostream& operator<<(std::ostream& os, const Message& mess);