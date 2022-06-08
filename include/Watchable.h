#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;


//---------------------Class Watchable----------------------
class Watchable{
public:
    //Constuctor
    Watchable(long id, int length, const std::vector<std::string>& tags);
    //Destructor
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    ///-------------Edited Start-----------------
       //Copy Constructor
       Watchable(const Watchable &other);
       //Move Constructor
       Watchable(Watchable &&other);
       //Copy Assignment
       Watchable& operator=(const Watchable &other);
       //Move Assignment
       Watchable& operator=(Watchable &&other);

    //getId
    long getId() const;
    //getLength
    int getLength() const;
    //getTags
    const std::vector <std::string>& getTags() const;
    //get Name for "watch"
    virtual std::string only_name() const =0;
    //Watchable Clone
    virtual Watchable* clone();
    ///-------------Edited End-----------------
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};


//---------------------Class Movie----------------------
class Movie : public Watchable{
public:
    //Constructor
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);

    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    ///-------------Edited Start-----------------
    //Destructor
    virtual ~Movie();
    //getName
    const std::string& getName() const;
    //get Name for "watch"
    virtual std::string only_name() const;
    //Movie Clone
    virtual Watchable* clone();
    ///-------------Edited End-------------------
private:
    std::string name;
};


//---------------------Class Episode----------------------
class Episode: public Watchable{
public:
    //Constructor
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);

    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    ///-------------Edited Start-----------------
    //Destructor
    virtual ~Episode();
    //getSeriesName
    const std::string& getSeriesName() const;
    //getSeason
    int getSeason() const;
    //getEpisode
    int getEpisode() const;
    //setNextEpisodeId
    void setNextEpisodeId(long nxt_id);
    //get Name for "watch"
    virtual std::string only_name() const;
    //Episode Clone
    virtual Watchable* clone();
    ///-------------Edited End-------------------
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
