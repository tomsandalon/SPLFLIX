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
	delete(s);
	s2.start();
	return 0;
}
