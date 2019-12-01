#include <iostream>
#include "../include/Session.h"
using namespace std;

int main(int argc, char** argv){
	if(argc!=2)
	{
		cout << "usage splflix input_file" << endl;
		return 0;
	}
	Session* s = new Session("config2.json");
	s->start();
	Session s2 = *s;


cout << "start"  << endl;
	cout << (s2.get_content()[0] -> toString(false)) << endl;
cout << "end"  << endl;;

	delete(s);

	cout << "start2"  << endl;
	cout << (s2.get_content()[0] -> toString(false)) << endl;
cout << "end2"  << endl;;

	s2.start();
	return 0;
}
