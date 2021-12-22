#include "Message.h"

Message::Message(){}

Message::Message(string Provider, int EventID, int Level, string Time, string Computer, vector<string> Event_data_names, vector<string> Event_data_values){
    string provider = Provider;
    int eventID = EventID;
    int level = Level;
    string time = Time;
    string computer = Computer;
    vector<string> event_data_names = Event_data_names;
    vector<string> event_data_values = Event_data_values;
}

ostream& operator<<(std::ostream& os, const Message& mess){
    os << "Provider name: " << mess.provider << endl;
    os << "EventID: " << mess.eventID << endl;
    os << "Level: " << mess.level << endl;
    os << "Time: " << mess.time << endl;
    os << "Computer: " << mess.computer << endl;
    return os;
}