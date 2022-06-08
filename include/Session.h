#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    ///-------------Edited Start-----------------
    //Copy Constructor
    Session(Session &other);
    //Move Constructor
    Session(Session &&other);
    //Copy Assignment
    Session& operator=(Session &other);
    //Move Assignment
    Session& operator=(Session &&other);

    bool getRunStat();                              //get running value
    void setRunStat(bool run);                              //change running value
    void addAction(BaseAction* currAction);         //add last action to actionLog
    std::vector<Watchable*> getContent();           //return vector of content
    std::string getSesLine();                       //get session.line
    void setSesLine(std::string& str);              //set session.line
    std::unordered_map<std::string,User*> &getUserMap();     //get user map
    void setActiveUser(User* name);                 //set active user
    User* getActiveUser();                          //get active user
    std::vector<BaseAction*> getActionLog();        //get action log
    void nextCommand(std::string& currLine);        //command getter from console
    ///-------------Edited End-----------------
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap; //watch the site for more info
    User* activeUser;
    ///-------------Edited Start-----------------
    bool running;                                  //program flow indicator
    std::string line;                              //line of command
         // Created function to delete Line after each use

    ///-------------Edited End-----------------
};

#endif
