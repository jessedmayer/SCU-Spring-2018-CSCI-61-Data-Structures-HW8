#include <iostream>
#include <cstdlib>
#include <queue>
#include "officehours.h"
#include <map>
#include <fstream>
#include <ctime>
#include <string>
#include <random>
#include <vector>

using namespace std;

averager waiting_time_average;
averager service_time_average;
averager extended_professor_time_average;



string get_name() {
    string name;
    int line_number = 0;
    ifstream File("names.txt");
    int random_number = (rand() % 99);
    if(File.is_open()) {
        while (getline(File, name,'\n')) {
            ++line_number;
            //cout << name << endl;
            if (line_number == random_number) {
                return name;
            }
        }
    }
}

string get_topic() {
    string topic;
    fstream infile;
    infile.open("topics.txt", ios_base::in);
    if (!infile) {
        cout << "Error reading file!";
        return 0;
    } else {

        getline(infile, topic);
        infile.close();
        return topic;
    }
}

void initialize_student_frequency(map<string, int> &student_frequency){
    string name;
    ifstream File("names.txt");
    if(File.is_open()) {
        while (getline(File, name,'\n')) {
            student_frequency[name] = 0;
        }
    }
}

void student_report(multimap<string,string> student_topics, map<string, int> student_frequency){
    string name;
    ifstream File("names.txt");
    if(File.is_open()) {
        while (getline(File, name,'\n')) {
            cout << name << "attended office hours: " << student_frequency[name] << " times" <<endl;
            typedef multimap<string, string>::iterator Iterator;

            // It returns a pair representing the range of elements with key equal to name
            pair<Iterator, Iterator> result = student_topics.equal_range(name);

            cout << name << " went to office hours for help with: " <<endl;

            // Iterate over the range
            for (Iterator it = result.first; it != result.second; it++)
                cout << it->second <<endl;

        }
    }
}

int random_number() {
    return ((rand() % 14) + 5); // service time is between 5 - 15 minutes per student
}

int random_priority() {
    return ((rand() % 1000) + 1); // student urgency between 1 - 999
}

void office_hours_func(double prob, unsigned int total_time, multimap<string,string> &student_topics, map<string, int> &student_frequency,
                       string student_names[], multimap<string,int> officehours_attended ,int runs) {
    queue<unsigned int> wait_times;             //arrival_times
    map<unsigned int, unsigned int> student;    //correlates students priority to service time
    priority_queue<int> priority;
    int next;
    bool_source arrival(prob);

    officehours office_hour;
    averager current_wait_times;
    averager current_service_times;
    unsigned int current_time;

    for (current_time = 1; current_time <= total_time; ++current_time) {
        if (arrival.query()){
            string name = get_name();
            string topic = get_topic();
            student_topics.insert(pair<string, string>(name,topic));
            officehours_attended.insert(pair<string, int>(name,runs));
            student_frequency[name]+=student_frequency[name];
            int service_time = random_number();
            int student_priority = random_priority();
            student[student_priority] = service_time;
            priority.push(student_priority);
            wait_times.push(current_time);

            current_service_times.next_number(service_time);
            service_time_average.next_number(service_time);
        }
        if ((!office_hour.is_busy()) && (!priority.empty())) {
            next = student[priority.top()];
            priority.pop();

            int wait_time = wait_times.front();
            wait_times.pop();
            current_wait_times.next_number(current_time - wait_time);
            waiting_time_average.next_number(current_time - wait_time);

            office_hour.start(next);
        }
        office_hour.one_second();
    }

    while(!priority.empty()) {
        if (!office_hour.is_busy()) {
            next = student[priority.top()];
            priority.pop();

            int wait_time = wait_times.front();
            wait_times.pop();
            current_wait_times.next_number(current_time - wait_time);
            waiting_time_average.next_number(current_time - wait_time);

            office_hour.start(next);
        }
        office_hour.one_second();
        current_time++;
    }
    extended_professor_time_average.next_number(current_time-total_time);
}

void insertionsort(string student_names[], int n){
    for(int i = 1; i < n ; i++){
        string temp = student_names[i];
        int m = i;
        while(temp > student_names[m-1] && m > 0){
            student_names[m] = student_names[m-1];
            m--;
        }
        student_names[m] = temp;
    }
}

void make_Report(string student_names[], multimap<string,string> &student_topics, map<string, int> &student_frequency){

}

void search(multimap<string,string> &student_topics, multimap<string,int> officehours_attended, string name){
    cout << "The office hours attended by " << name << " are: ";
    typedef multimap<string, int>::iterator Iterator1;
    pair<Iterator1, Iterator1> result = student_topics.equal_range(name);
    for (Iterator1 it = result.first; it != result.second; it++) {
        cout << it->second <<", ";
    }
    cout<<endl;
    cout << "The topics asked about by " << name << " are: ";
    typedef multimap<string, string>::iterator Iterator2;
    pair<Iterator2, Iterator2> result = student_topics.equal_range(name);
    for (Iterator2 it = result.first; it != result.second; it++) {
        cout << it->second <<", ";
    }
}

int main()
{

    int runs; // 100
    double prob; // 0.2
    multimap<string,string> student_topics;
    map<string, int> student_frequency;
    initialize_student_frequency(student_frequency);
    string student_names[];

    cout << "Enter number of office hours to run: " <<endl;
    cin >> runs;
    cout <<endl;
    cout << "Enter probability of a student arriving in any given second: " <<endl;
    cin >> prob;

    for (int i=1; i<=runs; i++) {
        office_hours_func(prob, 60, student_topics, student_frequency, student_names, runs);
    }
    int n = student_names->size();
    insertionsort(student_names, n);
    make_Report(student_names,student_topics,student_frequency);

    cout << "After running " << runs << " rounds " << endl;
    cout.precision(5);
    cout << "Average time a student waits: " << waiting_time_average.average() << " seconds" << endl;
    cout << "Average time a student meets with professor: " << service_time_average.average() << " seconds" << endl;
    cout << "Average time professor stays after 60 seconds: " << extended_professor_time_average.average() << " seconds"<< endl;

    student_report(student_topics, student_frequency);


    //cout<< get_name()<<endl;
    //cout<< get_name()<<endl;
    //cout<< get_name()<<endl;
    //cout<< get_topic() <<endl;



    return 0;
}

