#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
	BaseAction();
	ActionStatus getStatus() const;
	virtual void act(Session& sess)=0;
	virtual std::string toString() const=0;
    ///-------------Edited Start-----------------
    //Destructor
    virtual ~BaseAction();
    //Copy Constructor
    BaseAction(const BaseAction &other);
    //Move Constructor
    BaseAction(BaseAction &&other);
    //Copy Assignment
    BaseAction& operator=(const BaseAction &other);
    //Move Assignment
    BaseAction& operator=(BaseAction &&other);
    // BaseAction Clone
    virtual BaseAction* clone() ;
    ///-------------Edited End-----------------
protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;
private:
	std::string errorMsg;
	ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    ///-------------Edited Start-----------------
    // CreateUser Clone
    virtual BaseAction* clone() ;
    ///-------------Edited End-----------------
};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    ///-------------Edited Start-----------------
    // ChangeActiveUser Clone
    virtual BaseAction* clone() ;
    ///-------------Edited End-----------------
};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
	///-------------Edited Start-----------------
    // DeleteUser Clone
    virtual BaseAction* clone() ;
    ///-------------Edited End-----------------
};


class DuplicateUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
    ///-------------Edited Start-----------------
    // DuplicateUser Clone
    virtual BaseAction* clone() ;
    ///-------------Edited End-----------------
};

class PrintContentList : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    ///-------------Edited Start-----------------
    // PrintContentList Clone
    virtual BaseAction* clone() ;
    ///-------------Edited End-----------------
};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    ///-------------Edited Start-----------------
    // PrintWatchHistory Clone
    virtual BaseAction* clone() ;
    ///-------------Edited End-----------------
};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    ///-------------Edited Start-----------------
    // Watch Clone
    virtual BaseAction* clone();
    ///-------------Edited End-----------------
};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    ///-------------Edited Start-----------------
    // PrintActionsLog Clone
    virtual BaseAction* clone();
    ///-------------Edited End-----------------
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    ///-------------Edited Start-----------------
    // Exit Clone
    virtual BaseAction* clone();
    ///-------------Edited End-----------------
};
#endif
