#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../include/json.hpp"
#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Watchable.h"
using namespace std;

//---------------------Class BaseAction----------------------
//Constructor
BaseAction::BaseAction(): errorMsg(""), status(PENDING) {}
//Destructor
BaseAction::~BaseAction() {}
//Copy Constructor
BaseAction::BaseAction(const BaseAction &other): errorMsg(other.errorMsg), status(other.status) {}
//Move Constructor
BaseAction::BaseAction(BaseAction &&other): errorMsg(other.errorMsg), status(other.status) {}
//Copy Assignment
BaseAction &BaseAction::operator=(const BaseAction &other) {    return *this;}
//Move Assignment
BaseAction &BaseAction::operator=(BaseAction &&other) {    return *this;}
// BaseAction Clone
BaseAction* BaseAction::clone() {
    return nullptr;
}

//getStatus
ActionStatus BaseAction::getStatus() const {            return this->status;        }
//complete
void BaseAction::complete() {                            this->status=COMPLETED;    }
//error
void BaseAction::error(const std::string &errorMsg) {    this->status=ERROR; this->errorMsg=errorMsg;   }
//getErrorMsg
std::string BaseAction::getErrorMsg() const{             return this->errorMsg;     }

//---------------------Class CreateUser----------------------
//createuser act
void CreateUser::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    //code for word by word
    istringstream edit(sess.getSesLine());
    std::string name;
    edit >>name;
    std::string alg;
    edit >> alg;
    //end word by word

    if(   (sess.getUserMap()).find(name)==sess.getUserMap().end()  ){  //check if NOT exists in the usermap
        if(alg.compare("len")==0){
            User* usr = new LengthRecommenderUser(name);
            sess.getUserMap().insert({name,usr});        //adding user to userMap
            usr->CreateWatched(sess);                 //create watched vector
        }
        else if(alg.compare("rer")==0){
            User* usr = new RerunRecommenderUser(name);
            sess.getUserMap().insert({name,usr});        //adding user to userMap
            usr->CreateWatched(sess);                 //create watched vector
        }
        else if(alg.compare("gen")==0){
            User* usr = new GenreRecommenderUser(name);
            sess.getUserMap().insert({name,usr});        //adding user to userMap
            usr->CreateWatched(sess);                 //create watched vector
        }
        else{
            error("invalid recommendation algorithm!");
        }
    }
    else{    error("user name is taken!");    }

    if(this->getStatus()==ERROR)
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else {
        this->complete();
        //cout<< "Message: user created named "+name+"\n";           //TODO: DELETE
    }
    //line initialization
    std::string str = "";
    sess.setSesLine(str);
}
//createuser toString()
std::string CreateUser::toString() const {
    std::string str;
    if (this->getStatus() == ERROR)
        str = "CreateUser ERROR: " + this->getErrorMsg() + "\n";
    else if (this->getStatus() == COMPLETED)
        str = "CreateUser COMPLETED\n";
    else
        str = "CreateUser PENDING\n";
    return (str);
}
// CreateUser Clone
BaseAction* CreateUser::clone() {  return (new CreateUser(*this)); }

//---------------------Class ChangeActiveUser----------------------
//changeactiveuser act
void ChangeActiveUser::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    std::string name = sess.getSesLine();
    if (  (sess.getUserMap()).find(name)!=sess.getUserMap().end()  ){   //check if exists in the usermap
        User* usr = sess.getUserMap()[name];
        sess.setActiveUser( usr ); //sess.setActiveUser(  sess.getUserMap().find(name)->second  ); //this is the User*
    }
    else{    error("there is no such user!");   }

    if(this->getStatus()==ERROR)
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else{
        this->complete();
        //cout<< "Message: active user changed to "+name+"\n";          //TODO: DELETE
    }

    //line initialization
    std::string str = "";
    sess.setSesLine(str);
}
//changeactiveuser toString()
std::string ChangeActiveUser::toString() const {
    std::string str;
    if (this->getStatus() == ERROR)
        str = "ChangeActiveUser ERROR: " + this->getErrorMsg() + "\n";
    else if (this->getStatus() == COMPLETED)
        str = "ChangeActiveUser COMPLETED\n";
    else
        str = "ChangeActiveUser PENDING\n";
    return (str);
}
// ChangeActiveUser Clone
BaseAction* ChangeActiveUser::clone() {  return (new ChangeActiveUser(*this)); }

//---------------------Class DeleteUser----------------------
//DeleteUser act
void DeleteUser::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    std::string name = sess.getSesLine();
    if ( (sess.getUserMap()).find(name)!=sess.getUserMap().end()) { //check if exists in the usermap
        //if(sess.getUserMap()[name] == sess.getActiveUser())   ///current user wont be deleted @@@@@@
        //    sess.setActiveUser( sess.getUserMap()["default"]  );// if is active - set default
        delete sess.getUserMap()[name];
        sess.getUserMap().erase(name);           //delete name from the users
    }
    else{    error("there is no such user!");   }

    if(this->getStatus()==ERROR)
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else{
        //cout<< "Message: deleted user: "+name+"\n";          //TODO: DELETE
        this->complete();
    }

    //line initialization
    std::string str = "";
    sess.setSesLine(str);
}
//DeleteUser toString()
std::string DeleteUser::toString() const {
    std::string str;
    if (this->getStatus() == ERROR)
        str = "DeleteUser ERROR: " + this->getErrorMsg() + "\n";
    else if (this->getStatus() == COMPLETED)
        str = "DeleteUser COMPLETED\n";
    else
        str = "DeleteUser PENDING\n";
    return (str);
}
// DeleteUser Clone
BaseAction* DeleteUser::clone() {  return (new DeleteUser(*this)); }

//---------------------Class DuplicateUser----------------------
//DuplicateUser act
void DuplicateUser::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    //code for word by word
    istringstream edit(sess.getSesLine());
    std::string orig_name;
    edit >>orig_name;
    std::string new_name;
    edit >> new_name;
    //end word by word
    if( (sess.getUserMap()).find(orig_name)!=sess.getUserMap().end()){  //check if orig EXISTS in the usermap
        if(   (sess.getUserMap()).find(new_name)==sess.getUserMap().end() ) {//check if new is NOT in the usermap
            //29.11 11:20
            User* usr = sess.getUserMap()[orig_name]->clone();
            usr->setName(new_name);
            sess.getUserMap().insert( {new_name , usr} );
            //cout<< "Message: duplicated user: "+new_name+" is a copy of "+orig_name+"\n";          //TODO: DELETE
            usr = nullptr;
        }
        else{
            error("new name is taken!");            ///new name not available
        }
    }
    else{    error("the original user doesn't exist!");    } ///original name not found

    if(this->getStatus()==ERROR)
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else
        this->complete();

    //line initialization
    std::string str = "";
    sess.setSesLine(str);
}
//DuplicateUser toString()
std::string DuplicateUser::toString() const {
    std::string str;
    if (this->getStatus() == ERROR)
        str = "DuplicateUser ERROR: " + this->getErrorMsg() + "\n";
    else if (this->getStatus() == COMPLETED)
        str = "DuplicateUser COMPLETED\n";
    else
        str = "DuplicateUser PENDING\n";
    return (str);
}
// DuplicateUser Clone
BaseAction* DuplicateUser::clone() {  return (new DuplicateUser(*this)); }

//---------------------Class PrintContentList----------------------
//PrintContentList act
void PrintContentList::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    for(int i=0; i<(int)sess.getContent().size(); i++){
        cout<<sess.getContent()[i]->toString()<<"\n";
    }
    /// when will error occure? ...
/*    if(this->getStatus()==ERROR)
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else
*/        this->complete();

    //line initialization
    std::string str = "";
    sess.setSesLine(str);
}
//PrintContentList toString()
std::string PrintContentList::toString() const {
    std::string str;
    if(this->getStatus()==ERROR)
        str="PrintContentList ERROR: "+this->getErrorMsg()+"\n";
    else if(this->getStatus()==COMPLETED)
        str="PrintContentList COMPLETED\n";
    else
        str="PrintContentList PENDING\n";
    return (str);
}
// PrintContentList Clone
BaseAction* PrintContentList::clone() {  return (new PrintContentList(*this)); }

//---------------------Class PrintWatchHistory----------------------
//PrintWatchHistory act
void PrintWatchHistory::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    cout<< "Watch history for "<<sess.getActiveUser()->getName()<<"\n";
    for(int i=0; i<(int)sess.getActiveUser()->get_history().size(); i++)
         cout<< i+1 <<". " << sess.getActiveUser()->get_history_i(i)->only_name() <<"\n";

    if(this->getStatus()==ERROR)    /// When will it be?
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else
        this->complete();

    //line initialization
    std::string str = "";
    sess.setSesLine(str);
}
//PrintWatchHistory toString()
std::string PrintWatchHistory::toString() const {
    std::string str;
    if (this->getStatus() == ERROR)
        str = "PrintWatchHistory ERROR: " + this->getErrorMsg() + "\n";
    else if (this->getStatus() == COMPLETED)
        str = "PrintWatchHistory COMPLETED\n";
    else
        str = "PrintWatchHistory PENDING\n";
    return (str);
}
// PrintWatchHistory Clone
BaseAction* PrintWatchHistory::clone() {  return (new PrintWatchHistory(*this)); }

//---------------------Class Watch----------------------
//Watch act
void Watch::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    int id = std::stoi(sess.getSesLine());       // Convert string to int
    if ((id < 1) | (id > (int)sess.getContent().size())) {
          error("id is illegal!");
          cout<< "Error - id is illegal!\n";
          //line initialization
          std::string str = "";
          sess.setSesLine(str);
    }
    else {
        cout << "Watching " << sess.getContent()[id-1]->only_name() << "\n";
        sess.getActiveUser()->pushWatchHistory(  sess.getContent()[id-1]  );   //adding the watched to history
        Watchable *next_watch = sess.getContent()[id-1]->getNextWatchable(sess);
        if(next_watch==nullptr) {
            cout << "We don't have any new recommendation for you.\n";
            std::string str = "";
            sess.setSesLine(str);
        }
        else {
            cout << "We recommend watching " << next_watch->only_name()
                 << ", continue watching? [y/n]\n";    //ask recommendation

            std::string ans;
            getline(std::cin, ans);
            //while (!((ans.compare("y") == 0) | (ans.compare("n") == 0))) {  //infinite loop problem
            //    cout << "Choose only y or n.\n";
            //    getline(std::cin, ans);
            //}
            if (ans.compare("y") == 0) {
                std::string tmp_str = "watch " + to_string(next_watch->getId());
                sess.setSesLine(tmp_str);
            } else {
                std::string str = "";
                sess.setSesLine(str);
            }
        }
        complete();
        next_watch=nullptr;
    }
}
//Watch toString()
std::string Watch::toString() const {
        std::string str;
        if (this->getStatus() == ERROR)
            str = "Watch ERROR: " + this->getErrorMsg() + "\n";
        else if (this->getStatus() == COMPLETED)
            str = "Watch COMPLETED\n";
        else
            str = "Watch PENDING\n";
        return (str);
}
// Watch Clone
BaseAction* Watch::clone() {  return (new Watch(*this)); }

//---------------------Class PrintActionsLog----------------------
//PrintActionsLog act
void PrintActionsLog::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    for (int i=0; i<(int)sess.getActionLog().size(); i++)
        cout<<sess.getActionLog()[i]->toString();
    complete();

    //line initialization
    std::string str = "";
    sess.setSesLine(str);
}
//PrintActionsLog toString()
std::string PrintActionsLog::toString() const {
    std::string str;
    if (this->getStatus() == ERROR)
        str = "PrintActionsLog ERROR: " + this->getErrorMsg() + "\n";
    else if (this->getStatus() == COMPLETED)
        str = "PrintActionsLog COMPLETED\n";
    else
        str = "PrintActionsLog PENDING\n";
    return (str);
}
// PrintActionsLog Clone
BaseAction* PrintActionsLog::clone() {  return (new PrintActionsLog(*this)); }

//---------------------Class Exit----------------------
//Exit act
void Exit::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    sess.setRunStat(false );
    complete();
    //line initialization
    std::string str = "";
    sess.setSesLine(str);
}
//Exit toString()
std::string Exit::toString() const {
        std::string str;
        if (this->getStatus() == ERROR)
            str = "Exit ERROR: " + this->getErrorMsg() + "\n";
        else if (this->getStatus() == COMPLETED)
            str = "Exit COMPLETED\n";
        else
            str = "Exit PENDING\n";
        return (str);
}
// Exit Clone
BaseAction* Exit::clone() {  return (new Exit(*this)); }








