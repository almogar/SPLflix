#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>    // std::sort

#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Watchable.h"
using namespace std;

//---------------------Class User----------------------
//Constructor
User::User(const std::string& name): history(0,nullptr),name(name), watched(0,0),
        Genres(0,{0,""}),avg(0){}
//Destructor
User::~User() {}
//Copy Constructor
User::User(const User &other):  history(other.history), name(other.name),watched(other.watched),
                Genres(other.Genres),avg(other.avg){  /// $$$  NewName suppose to be valid
///copy of vector by default
}
//Move Constructor
User::User(User &&other): history(other.history),name(other.name), watched(other.watched),
                         Genres(other.Genres),avg(other.avg) {}
//Copy Assignment
User &User::operator=(const User &other) { return *this; }
//Move Assignment
User &User::operator=(User &&other) { return *this; }
//User Clone
User *User::clone() {
    return nullptr;
}

//getName
std::string User::getName() const{                                 return name;   }
//getHistory
std::vector<Watchable*> User::get_history() const {                return history;    }
//getHistory_i
Watchable* User::get_history_i(int i) const {
    if((i>=0) & (i<(int)this->get_history().size()))
        return this->history[i];
    std::cout << "Error - no such index exists.";
    return nullptr;
}
//set history i
void User::set_history_i(int i, Watchable * newwatch) {
    this->history[i] = newwatch;
}
//setName
void User::setName(std::string &new_name) {                        this->name=new_name;   }
//create Watched vector
void User::CreateWatched(Session &sess) {
    this->watched = std::vector<int>((int)sess.getContent().size(), 0);                 //initiate bool aray
}
//getWatched
std::vector<int> User::getWatched (){                             return this->watched;  }
//setWatched
void User::setWatched_i (int i){                                   this->watched[i]=1; }
//setAvg
void User::setAvg(int length_val) {
    avg = ( avg*(this->get_history().size()-1)+ length_val) / (this->get_history().size());
}
//getAvg
double User::getAvg() const {                                      return this->avg;       }
//getGenres
std::vector<std::pair <int,std::string> > &User::getGenres() {      return this->Genres;     }
//addGenre
void User::addGenre(Watchable *currWatch) {
    for(int i=0; i<(int)currWatch->getTags().size(); i++){
        bool found=false;
        for(int k=0; k<((int)this->getGenres().size()); k++) {
            if (currWatch->getTags()[i].compare(this->Genres[k].second) == 0) {
                this->Genres[k].first++;
                found = true;
            }
        }
        if(!found) {                   // no kind of this tag yet...
            std::pair<int,std::string> newPair(1, currWatch->getTags()[i]);
            this->getGenres().push_back(newPair);
        }
    }
}
//pushWatchHistory
void User::pushWatchHistory(Watchable *current){
    this->history.push_back(current);
}

//---------------------Class LengthRecUser----------------------
//Constructor - this is the default anyway
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name) {}
//Copy Constructor - this is the default anyway
LengthRecommenderUser::LengthRecommenderUser(const User &other) : User(other) {}
//getRecommendation
Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    // those 2 will be operate manuely at Watchable.cpp->getNextWatchable
    this->setWatched_i( this->get_history_i(  (int)this->get_history().size()-1  )->getId() -1  );  //marked as watched
    this->setAvg(this->get_history_i( (int)this->get_history().size()-1 )->getLength());     //computed as avg

    int bst_dif = 2147483647;                                                            // max val of integer
    Watchable* best_opt = nullptr;
    for(int i=0; i<(int)s.getContent().size(); i++){
        if( this->getWatched()[i]==0) {                                                    // content not been watched yet
            if (abs(s.getContent()[i]->getLength() - this->getAvg()) < (bst_dif)) {
                bst_dif = abs(s.getContent()[i]->getLength() - this->getAvg());
                best_opt = s.getContent()[i];
            }
        }
    }
    return (best_opt);
}
//LengthRecommenderUser Clone
User *LengthRecommenderUser::clone() {
    return (new LengthRecommenderUser(*this));
}

//---------------------Class RerunRecUser----------------------
//Constructor
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name), rerun_next_index(0) {}
//CopyConstructor
RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser &other) : User(other) , rerun_next_index(other.rerun_next_index){}
//getRecommendation
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    // ******* rerun_next_index will always < history.size()
    rerun_next_index++;
    return (s.getActiveUser()->get_history_i(  this->rerun_next_index-1  ));
}
// !!!  if seen no episodes - always will recommend the last watchable

//RerunRecommenderUser Clone
User *RerunRecommenderUser::clone() {
    return (new RerunRecommenderUser(*this));
}


//---------------------Class GenreRecUser----------------------
//Constructor
GenreRecommenderUser::GenreRecommenderUser( const std::string &name):User(name) {}
//Copy Constructor
GenreRecommenderUser::GenreRecommenderUser(const User &other): User(other) {}
//getRecommendation
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    // those 2 will be operate manuely at Watchable.cpp->getNextWatchable
    this->setWatched_i(this->get_history_i((int) this->get_history().size() - 1)->getId() - 1);  //marked as watched
    this->addGenre(this->get_history_i((int) this->get_history().size() - 1));                //counted tags
    //sort from max to min - in int , and min to max at string
    std::sort(this->getGenres().begin(),this->getGenres().end(),
            []( const std::pair<int,std::string> &a, std::pair<int,std::string> &b )
            {return ( (a.first > b.first) || ((a.first==b.first) & (a.second[0] < b.second[0]) ));});

    for(int i=0; i<(int)this->getGenres().size(); i++) {
        for (int j = 0; j < (int) s.getContent().size(); j++) {                         //checks all content
            if (this->getWatched()[j]==0) {                                             // content not been watched yet
                for (int k = 0; k < (int) s.getContent()[j]->getTags().size(); k++) {   //check all tags of 1 watchable
                    if(s.getContent()[j]->getTags()[k].compare((this->getGenres()[i]).second) == 0){
                        return s.getContent()[j];
                    }
                }
            }
        }
    }
    return (nullptr); /// if null pointer work correctly

}

//GenreRecommenderUser Clone
User *GenreRecommenderUser::clone() {
    return (new GenreRecommenderUser(*this));
}


