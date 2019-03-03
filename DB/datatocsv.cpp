#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

using namespace std;

list<string> ssplit(const string &s, char delim, list<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim) && !ss.eof()) {
        elems.push_back(item);
    }
    return elems;
}


list<string> split(const string &s, char delim) {
    list<string> elems;
    ssplit(s, delim, elems);
    return elems;
}

int main(int argc, char *argv[]){
	ifstream input(argv[1]);
	string name(argv[1]), new_name, buffer;
	bool flag = false;
	
	for(char i : name){
		if(i == '.')
			break;

		new_name.push_back(i);
	}
	
	new_name = new_name + ".csv";
	cout << new_name << endl;
	
	ofstream out(new_name.c_str());
	
	if(!input || !out){
		cerr << new_name << " File could not be opened!" << endl;
		return 1;
	}
	
	while(getline(input, buffer) && !input.eof()){
		list<string> tokens = split(buffer, ' ');
		bool flag = false;
		bool fl = true;
		string o;
		int j = 0;
		out << *(tokens.begin()) << ',';
		for(string str: tokens){
			for(char i : str){
				if(flag){
					out << i;
				}
				if(i == ':'){
					flag = true;
				}
			}
			if((j != tokens.size()-1) && j > 0)
				out << ',';
			flag = false;
			/*if(!fl)
				out << o << ',';*/
			o.clear();
			j++;
		}
		out << endl;
		buffer.clear();
	}
	
	input.close();
	out.close();
}
