#include <iostream>
#include <fstream>

#include "../include/json.hpp"
#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Watchable.h"

using namespace std;

int main(int argc, char** argv){
	if(argc!=2)
	{
		cout << "usage splflix input_file" << endl;
		return 0;
	}
	Session s(argv[1]);
    s.start();
    
/*    Session* s = new Session(argv[1]);
    s->start();
    Session s2 = *s;
    delete(s);
    s2.start();  */

 /*   Session s(argv[1]);
    s.start();
    Session s2(argv[1]);
    s2=s;
    s2.start(); */
   /* Session* s1 = new Session(argv[1]); //const
    s1->start();
    Session* s2 = new Session(argv[1]);
    s2->start();
    delete(s1);
    *s2 = std::move(*s1); //move assignment

    s2->start();
    Session* s3(new Session(argv[1])); //move const
    s3->start();
    *s2 = *s3; //operator=
    s2->start();
    delete (s3);
    Session* s4 = new Session(argv[1]);
    s4->start();
    Session* s5 = s4;
    *s5 = std::move(*s4); //self assignment
    s5->start();
    *s2 = * s4; //operator=
    delete(s4);
    s2->start();
    delete(s2); */
    return 0;
   // return 0;
}
