#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/json.hpp"
#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Watchable.h"
using namespace std;
using json = nlohmann::json;

//---------------------Class Session----------------------
//Constructor
Session::Session(const std::string &configFilePath): content (0, nullptr), actionsLog(0, nullptr),
    userMap(), activeUser(nullptr), running(false), line("") {
    std::ifstream i(configFilePath);
    json j;
    i >>j;
    /// import json parameters for watchables
    for(int k=0; k<(int)j["movies"].size(); k++){
        this->content.push_back(new Movie((int)this->content.size()+1,j["movies"][k]["name"],
                j["movies"][k]["length"],j["movies"][k]["tags"]));  ///WATCH FOR ERRORS WITH USING JSON
    }
    for(int k=0; k<(int)j["tv_series"].size(); k++) {
        for (int s = 1; s <= (int)j["tv_series"][k]["seasons"].size(); s++) {
            for (int e = 1; e <= j["tv_series"][k]["seasons"][s - 1]; e++) {
                Episode* EP = new Episode((int)this->content.size() + 1, j["tv_series"][k]["name"],
                        j["tv_series"][k]["episode_length"], s, e,j["tv_series"][k]["tags"]);
                this->content.push_back(EP);
                if((s==(int)j["tv_series"][k]["seasons"].size()) &  (e==j["tv_series"][k]["seasons"][s - 1]) )
                    EP->setNextEpisodeId(0);                        //LAST EPISODE AT TV SERIES
                else
                    EP->setNextEpisodeId((int)this->content.size());
            }
        }
    }
    /// Add config 1 ?????
    //Create new user default and it's the active user
    std::string cmd;
    cmd = "createuser default len";
    Session::nextCommand(cmd);
    cmd = "changeuser default";
    Session::nextCommand(cmd);
}
//Destructor
Session::~Session() {
    for(int i=0; i<(int)this->content.size(); i++){                 //DELETE
        delete this->content[i];
    }
    this->content.clear();
    for(int i=0; i<(int)this->actionsLog.size(); i++){               //DELETE
        delete this->actionsLog[i];
    }
    this->actionsLog.clear();
    for (auto local_it = this->userMap.begin(); local_it != this->userMap.end(); ++local_it)
        delete local_it->second;
    //this->userMap.erase(this->userMap.begin(),this->userMap.end());  //DELETE
    this->userMap.clear();
    //delete this->activeUser;
    this->activeUser=nullptr;
}
//Copy Constructor
Session::Session(Session &other): content(),actionsLog(),userMap(),activeUser(),
        running(other.running),line(other.line){
    for(int i=0; i<(int)other.content.size(); i++){                 //Insert
        Watchable* tmp = other.content[i]->clone();
        this->content.push_back( tmp );
        tmp=nullptr;
    }
    for(int i=0; i<(int)other.actionsLog.size(); i++){                 //Insert
        BaseAction* tmp = other.actionsLog[i]->clone();
        this->actionsLog.push_back(  tmp );
        tmp=nullptr;
    }
    for ( unsigned i = 0; i < other.userMap.bucket_count(); ++i) {
        for (auto local_it = other.userMap.begin(i); local_it != other.userMap.end(i); ++local_it) {
            User *new_usr = local_it->second->clone();
            std::string cpy_name = local_it->first;
            this->userMap.insert({cpy_name, new_usr});        //adding user to userMap
            for(int k=0; k<(int)new_usr->get_history().size(); k++){
                long tmpid = new_usr->get_history_i(k)->getId();
                new_usr->set_history_i(k, this->content[tmpid-1]);
            }
            if(other.activeUser == local_it->second) {
                this->setActiveUser(new_usr);    // Active user
            }
            new_usr=nullptr;

        }
    }
}
//Move Constructor
Session::Session(Session &&other): content(), actionsLog(), userMap(),
        activeUser(), running(), line(){

        running = other.running;
        line = other.line;
        for (int i = 0; i < (int)other.content.size(); i++) {                 //Insert
            this->content.push_back(other.content[i]);
            other.content[i] = nullptr;  //delete other one
            /*Watchable* cpy = Watchable(other.content[i]);
            this->content.push_back(other.content[i]);*/
        }

        //maybe error
        for (int i = 0; i < (int)other.content.size(); i++) {  //delete other one
            delete other.content[i];
        }
        //
        other.content.clear();

        for (int i = 0; i < (int)other.actionsLog.size(); i++) {                 //Insert
            this->actionsLog.push_back(other.actionsLog[i]);
            other.actionsLog[i] = nullptr;    //delete other one
        }

        for (int i = 0; i < (int)other.actionsLog.size(); i++) {
            delete other.actionsLog[i];     //delete other one
        }
        other.actionsLog.clear();

        for (unsigned i = 0; i < other.userMap.bucket_count(); ++i) {
            for (auto local_it = other.userMap.begin(i); local_it != other.userMap.end(i); ++local_it) {
                User *new_usr = local_it->second;
                std::string cpy_name = local_it->first;
                this->userMap.insert({cpy_name, new_usr});        //adding user to userMap
                if (other.activeUser == local_it->second)
                    this->setActiveUser(new_usr);    // Active user
                local_it->second = nullptr;
            }
        }
        other.userMap.clear();
        other.activeUser = nullptr;
        //other.userMap.erase(other.userMap.begin(), other.userMap.end());  //DELETE
}
//Copy Assignment
Session &Session::operator=(Session &other){
    if(this!=&other) {
        this->running = other.running;
        this->line = other.line;
        for (int i = 0; i < (int)this->content.size(); i++) {                 //delete
            delete this->content[i];
        }
        this->content.clear();
        for (int i = 0; i < (int)other.content.size(); i++) {                 //Insert
            Watchable* tmp = other.content[i]->clone();
            this->content.push_back(tmp);                       //CLONE
            tmp = nullptr;
        }

        for (int i = 0; i < (int)this->actionsLog.size(); i++) {               //delete
            delete this->actionsLog[i];
        }
        this->actionsLog.clear();
        for (int i = 0; i < (int)other.actionsLog.size(); i++) {                 //Insert
            BaseAction* tmp = other.actionsLog[i]->clone();
            this->actionsLog.push_back(tmp);              //CLONE
            tmp = nullptr;
        }

        for (unsigned i = 0; i < this->userMap.bucket_count(); ++i) { //delete
            for (auto local_it = this->userMap.begin(i); local_it != this->userMap.end(i); ++local_it) {
                delete local_it->second;
            }
        }
        this->userMap.clear();
        //this->userMap.erase(this->userMap.begin(), this->userMap.end());  //DELETE THIS MAP
        for (unsigned i = 0; i < other.userMap.bucket_count(); ++i) {                       // INSERT THIS MAP
            for (auto local_it = other.userMap.begin(i); local_it != other.userMap.end(i); ++local_it) {
                User *new_usr = local_it->second->clone();                                  //CLONE
                std::string cpy_name = local_it->first;
                this->userMap.insert({cpy_name, new_usr});        //adding user to userMap
                for(int k=0; k<(int)new_usr->get_history().size(); k++){
                    long tmpid = new_usr->get_history_i(k)->getId();
                    new_usr->set_history_i(k, this->content[tmpid-1]);
                }
                if(other.activeUser == local_it->second) {
                    this->setActiveUser(new_usr);    // Active user
                }
                new_usr=nullptr;
            }
        }
    }
        return (*this);
}
//Move Assignment
Session &Session::operator=(Session &&other) {
    if(this!=&other) {
        this->running = other.running;
        this->line = other.line;
        for (int i = 0; i < (int) this->content.size(); i++) {                 //delete
            delete this->content[i];
        }
        this->content.clear();
        for (int i = 0; i < (int) other.content.size(); i++) {                 //Insert
            this->content.push_back(other.content[i]);
            other.content[i] = nullptr;  //delete other one
        }
        for (int i = 0; i < (int) other.content.size(); i++) {  //delete other one
            delete other.content[i];
        }
        other.content.clear();


        for (int i = 0; i < (int) this->actionsLog.size(); i++) {               //delete
            delete this->actionsLog[i];
        }
        this->actionsLog.clear();
        for (int i = 0; i < (int) other.actionsLog.size(); i++) {                 //Insert
            this->actionsLog.push_back(other.actionsLog[i]);
            other.actionsLog[i] = nullptr;    //delete other one
        }
        for (int i = 0; i < (int) other.actionsLog.size(); i++) {
            delete other.actionsLog[i];     //delete other one
        }
        other.actionsLog.clear();


        for (unsigned i = 0; i < this->userMap.bucket_count(); ++i) { //delete THIS
            for (auto local_it = this->userMap.begin(i); local_it != this->userMap.end(i); ++local_it) {
                delete local_it->second;
            }
        }
        this->userMap.clear();
        //other.userMap.erase(other.userMap.begin(),other.userMap.end());  //DELETE THIS
        for (unsigned i = 0; i < other.userMap.bucket_count(); ++i) {
            for (auto local_it = other.userMap.begin(i); local_it != other.userMap.end(i); ++local_it) {
                User *new_usr = local_it->second;
                std::string cpy_name = local_it->first;
                this->userMap.insert({cpy_name, new_usr});        //adding user to userMap
                if (other.activeUser == local_it->second)
                    this->setActiveUser(new_usr);    // Active user
                local_it->second = nullptr;
            }
        }
        other.userMap.clear();
        other.activeUser = nullptr;
        //other.userMap.erase(other.userMap.begin(),other.userMap.end());  //DELETE Other
    }
        return (*this);
}

//Start
void Session::start() {
    std::cout <<"SPFLIX is now on!\n";
    Session::setRunStat(true); // program is running

    ///MAIN LOOP
    while (this->getRunStat()){
        if( this->line.compare("")!=0){
            nextCommand(this->line);
        }
        else {
            std::string cmd;
            std::getline(std::cin, cmd);
            nextCommand(cmd);
        }
    }
}

//set_Run_stat
void Session::setRunStat(bool run){                             this->running=run;         }
//get_Run_stat
bool Session::getRunStat(){                                     return (this->running);    }
//addAction
void Session::addAction(BaseAction* currAction) {               this->actionsLog.push_back(currAction);      }
//getContent
std::vector<Watchable*> Session::getContent() {                 return this->content;       }
//getSesLine
std::string Session::getSesLine() {                             return this->line;          }
//setSesLine
void Session::setSesLine(std::string &str) {                    this->line=str;             }
//getUserMap
std::unordered_map<std::string,User*>& Session::getUserMap() {    return this->userMap;       }
//setActiveUser
void Session::setActiveUser(User *name)  {                      this->activeUser=name;      }
//getActiveUser
User* Session::getActiveUser() {                                return this->activeUser;    }
//getActionLog
std::vector<BaseAction*> Session::getActionLog()  {              return this->actionsLog;    }
//nextCommand
void Session::nextCommand(std::string &currLine) {

    //code for word by word from &currLine
    istringstream editedLine(currLine);
    std::string word;
    editedLine >> word;
    //end of word by word

    //check for valid action -> save currline to line -> call spesific action
    if(word.compare("createuser")==0){
        line =currLine.substr(11,currLine.length()-1);  //2 words
        BaseAction* p = new CreateUser();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("changeuser")==0){
        editedLine>>line;                                       //1 word
        BaseAction* p = new ChangeActiveUser();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("deleteuser")==0){
        editedLine>>line;
        BaseAction* p = new DeleteUser();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("dupuser")==0){
        line =currLine.substr(8,currLine.length()-1);
        BaseAction* p = new DuplicateUser();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("content")==0){
                                                                //no words
        BaseAction* p = new PrintContentList();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("watchhist")==0){
                                                             //no words
        BaseAction* p = new PrintWatchHistory();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("watch")==0){
        editedLine>>line;
        BaseAction* p = new Watch();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("log")==0){
        BaseAction* p = new PrintActionsLog();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("exit")==0){
        BaseAction* p = new Exit();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else{   cout<<"Error - no such command!\n";    }
}

/*  createuser
*  changeuser
*  deleteuser
*  dupuser
*  content
*  watchhist
 *  watch
 *  log
 *  exit
*/