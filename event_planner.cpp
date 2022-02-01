#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>
#include <sstream>

using namespace std;

class Date
{
public:
    Date(int day,int month, int year):m_year(year)
    {
        if(month > 12 || month < 1)
        {
            throw logic_error("Month value is invalid: " + to_string(month));
        }
        else
            m_month = month;
        if (day > 31 || day < 1)
        {
            throw logic_error("Day value is invalid: " + to_string(day));
        }
        else 
            m_day = day;
    }

    int getDay()const
    {
        return m_day;
    }

    int getMonth()const
    {
        return m_month;
    }

    int getYear()const
    {
        return m_year;
    }

private:
    int m_day;
    int m_month;
    int m_year;
};

bool operator< (const Date& obj,const Date& obj2)
{
    vector<int>v_data  {obj.getDay(),obj.getMonth(),obj.getYear()};
    vector<int>v_data2 {obj2.getDay(), obj2.getMonth(), obj2.getYear()};
    return (v_data < v_data2);
}

ostream& operator<< (ostream& stream, const Date& date)
{
    stream << setw(2) <<  setfill('0') << date.getDay() <<
    "/" << setw(2) << setfill('0') << date.getMonth() << 
    "/" << setw(4) << setfill('0') << date.getYear();

    return stream;
}

class DataBase
{
public:
    void addEvent(const Date& date, const string& event)
    {
        storage[date].insert(event);
    }
    bool deleteEvent(const Date& date, const string& event)
    {
        if(storage.count(date) > 0 && storage[date].count(event) > 0)
        {
            storage[date].erase(event);
            return true;
        }
        return false;
    }

    int deleteDate(const Date& date)
    {
        if (storage.count(date) == 0)
        {
          return 0;
        }
        else
        {
            const int count_event = storage[date].size();
            storage.erase(date);
            return count_event;
        }
        
    }

    set<string> findData(const Date& date)const
    {
        if(storage.count(date) > 0)
            return storage.at(date);
        else
            return {};
    }

    void print() const
    { 
        for (const auto &item : storage)
        {
            for(const string &event : item.second)
            {
                cout << item.first << " " << event << endl;
            }
        }  

    }

private:
    map<Date, set<string>> storage;
};

Date ParseDate(const string& date) 
{
   istringstream date_stream(date);
   bool ok = true;
   
   int day;
   ok = ok && (date_stream >> day);
   ok = ok && (date_stream.peek() == '-');
   date_stream.ignore(1);

   int month;
   ok = ok && (date_stream >> month);
   ok = ok && (date_stream.peek() == '-');
   date_stream.ignore(1);

   int year;
   ok = ok && (date_stream >> year);
   ok = ok && (date_stream.eof());
   
   if(!ok)
   {
       throw logic_error("Wrong date format: " + date);
   }
   return Date(day,month,year);
}

void getUserInput()
{
    try{

    DataBase db;
    string command_line;

    while(getline(cin, command_line)){

        stringstream ss(command_line);
        string command;
        ss >> command;

        if(command == "Add"){
            string date_str;
            string event;
            ss >> date_str >> event;
            const Date date = ParseDate(date_str);
            db.addEvent(date, event);
        }

        else if(command == "Del"){
            string date_str;
            ss >> date_str;
            string event;
            if (!ss.eof()) 
            {
                ss >> event;
            }
            const Date date = ParseDate(date_str);
            if (event.empty()) 
            {
                const int count = db.deleteDate(date);
                cout << "Deleted " << count << " events" << endl;
            } 
            else 
            {
                if (db.deleteEvent(date, event))
                {
                    cout << "Deleted successfully" << endl;
                } 
                else 
                {
                    cout << "Event not found" << endl;
                }
            }
        }

        else if (command == "Find"){
            string date_str;
            ss >> date_str;
            const Date date = ParseDate(date_str);
            for (const string& event : db.findData(date)){
                cout << event << endl;
            }
        } 

        else if (command == "Print"){

            db.print();
        } 

        else if (!command.empty()){

        throw logic_error("Unknown command: " + command);

      }
    }
  } catch (const exception& e){
        cout << e.what() << endl;
    }
}



int main()
{

    getUserInput();
    
    return 0;

}