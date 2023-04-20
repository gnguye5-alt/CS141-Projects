#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

struct PhoneNumbers {
    string number;
    char phoneType;
};
class Contact {
    public:
        Contact();
        Contact(char contactType, string firstName, string lastName) {
            this -> contactType = contactType;
            this -> firstName = firstName;
            this -> lastName = lastName;
        }

        Contact(char contactType, vector<PhoneNumbers> phoneNumber) {
            this -> contactType = contactType;
            for (int i = 0; i < phoneNumber.size(); i++) {
                this -> phoneNumber.push_back(phoneNumber.at(i));
            }
        }

        bool operator<(Contact& c1) {
            return this -> getName() < c1.getName(); // assuming getName() is a member function of Contacts that returns the name
        }

        string getFirstName() {
            return this -> firstName;
        }

        string getLastName() {
            return this -> lastName;
        }

        char getContactType() {
            return this -> contactType;
        }

        string getState() {
            return this -> state;
        }

        string getEmail() {
            return this -> email;
        }

        string getDateOfBirth() {
            return this -> dateOfBirth;
        }

        void setFirstName(string firstName) {
            this -> firstName = firstName;
        }

        void setLastName(string lastName) {
            this -> lastName = lastName;
        }

        void setContactType(char contactType) {
            this -> contactType = contactType;
        }

        void setEmail(string email) {
            this -> email = email;
        }

        void setDateOfBirth(string dateOfBirth) {
            this -> dateOfBirth = dateOfBirth;
        }

        void setAddress(int number, string street, string city, string state, int postalCode) {
            this -> streetNumber = number;
            this -> streetName = street;
            this -> city = city;
            this -> state = state;
            this -> postalCode = postalCode;
        }

        string getName() {
            string res = this -> lastName + ", " + firstName;
            return res;
        }

        string getAddress() {
            string res = to_string(streetNumber) + " " + streetName + ", " + city + ", " + state + " " + to_string(postalCode);
            return res;
        }


        string getAsString() {
            string phone = "";
            for (int i = 0; i < phoneNumber.size(); i++) {
                if (phoneNumber[i].phoneType == 'W') {
                    phone += "Work: " + phoneNumber[i].number + "\n";
                } else if (phoneNumber[i].phoneType == 'C') {
                    phone += "Cell: " + phoneNumber[i].number + "\n";
                } else if (phoneNumber[i].phoneType == 'H') {
                    phone += "Home: " + phoneNumber[i].number + "\n";
                }
            }
            string type = "";
            if (getContactType() == 'W') {
                type = "Work";
            } else if (getContactType() == 'P') {
                type = "Personal";
            }
            string res = this -> getName() + "\n" + 
                        type + "\n" + 
                        this -> getAddress() + "\n" + 
                        this -> getDateOfBirth() + "\n" + 
                        this -> getEmail() + "\n" +
                        phone;
            return res;
        }

        string addPhone(char type, string number) {
            if (type != 'W' && type != 'H' && type != 'C') {
                string s(1, type);
                string res = "failure: invalid phone type - " + s;
                return res;
            }
            PhoneNumbers newP;
            newP.phoneType = type;
            newP.number = number;
            phoneNumber.push_back(newP);
            if (type == 'W') {
                string res = "success: added number " + number + " Work";
                return res;
            } else if (type == 'C') {
                string res = "success: added number " + number + " Cell";
                return res;
            } else if (type == 'H') {
                string res = "success: added number " + number + " Home";
                return res;
            }
        }

        string deletePhone(int index) {
            if (index >= phoneNumber.size() || index < 0) {
                return "failure: unable to delete phone " + to_string(index);
            } else {
                return "success: deleted phone " + to_string(index);
                phoneNumber.erase(phoneNumber.begin() + index);
            }
        }
  
        string tester();


    private:
        char contactType;
        string firstName;
        string lastName;
        string streetName;
        string city;
        string state;
        string email;
        string dateOfBirth;
        int streetNumber;
        int postalCode;
        vector<PhoneNumbers> phoneNumber;
};

class ContactList {
    public:
        string loadContactsFromFile(string filename) {
            ifstream file(filename);
            if (!file.is_open()) {
                return "failure: " + filename + " not found";
            }
            // Read the header
            string header;
            int count = 0;
            getline(file, header);
            string line;
            while (getline(file, line)) {
                count++;
                istringstream iss(line);
                string type;
                string firstName;
                string lastName;
                string streetNumber;
                string streetName;
                string city;
                string state;
                string postcode;
                string email;
                string dob;
                string age;
                string phone;
                string cell;
                getline(iss, type, ',');
                getline(iss, firstName, ',');
                getline(iss, lastName, ',');
                getline(iss, streetNumber, ',');
                getline(iss, streetName, ',');
                getline(iss, city, ',');
                getline(iss, state, ',');
                getline(iss, postcode, ',');
                getline(iss, email, ',');
                getline(iss, dob, ',');
                getline(iss, age, ',');

                PhoneNumbers temp;
                vector<PhoneNumbers> p;
                getline(iss, phone, ',');
                temp.number = phone;
                temp.phoneType = 'H';
                p.push_back(temp);
                getline(iss, cell);
                temp.number = cell;
                temp.phoneType = 'C';
                p.push_back(temp);

                Contact contact(toupper(type[0]), p);
                contact.setFirstName(firstName);
                contact.setLastName(lastName);
                contact.setAddress(stoi(streetNumber), streetName, city, state, stoi(postcode));
                contact.setEmail(email);
                contact.setDateOfBirth(dob);
                contacts.push_back(contact);
            }

            file.close();
            return "success: " + to_string(count) + " contacts added";
        }


        int getCount() {
            return contacts.size();
        }

        bool compareInterval(Contact i1, Contact i2) const {
            return (i1.getName() < i2.getName());
        }

        string sortContacts() {
            // sort the vector using sort
            sort(contacts.begin(), contacts.end());
            return "success";
        }

        vector<int> findContactsByName (string searchStr) {
            vector<int> res;
            string lowercaseSearchStr = searchStr;
            transform(lowercaseSearchStr.begin(), lowercaseSearchStr.end(), lowercaseSearchStr.begin(), ::tolower);
            for (int i = 0; i < contacts.size(); i++) {
                string lowercaseName = contacts[i].getName();
                transform(lowercaseName.begin(), lowercaseName.end(), lowercaseName.begin(), ::tolower);
                if (lowercaseName.find(lowercaseSearchStr) != string::npos) {
                    res.push_back(i);
                }
            }

            return res;
        }


        void printContacts() {
            for (int i = 0; i < contacts.size(); i++) {
                cout << "--------------------" << endl << endl;
                contacts[i].getAsString();
            }
        }

        void printContacts(vector<int> pos) {
            for (int i = 0; i < pos.size(); i++) {
                cout << "--------------------" << endl << endl;
                cout << contacts[pos[i]].getAsString() << endl; 
            }
        }

        string addContact(Contact object) {
            contacts.push_back(object);
            return "success: contact " + object.getName() + " added";
        }

        string deleteContact(int index) {
            if (index >= 0 && index < contacts.size()) {
                contacts.erase(contacts.begin() + index);
                return "success: deleted contact " + contacts[index].getName();
            } else {
                return "failure: unable to delete contact " + to_string(index);
            }
        }

        string tester();

    private:
        vector<Contact> contacts;
};

int main() {
    ContactList myContacts;
    myContacts.loadContactsFromFile("contacts.csv");
    myContacts.sortContacts();
    vector<int> v = myContacts.findContactsByName("ra");
    myContacts.printContacts(v);
    return 0;
}