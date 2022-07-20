//
// Created by ynal6 on 22/11/2021.
//

#ifndef SPLASSIGNMENT1_STUDIO_H
#define SPLASSIGNMENT1_STUDIO_H


#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "fstream"
#include "sstream"
#include "Action.h"

class Studio{
public:
    Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<std::string> getInputs();
    //rule of 5:
    virtual ~Studio();//destructor
    Studio(const Studio &other);//copy constructor
    Studio& operator=( const Studio &other);//assignment operator
    Studio( Studio&& other); //move constructor
    Studio& operator=(Studio&& other); //move assignment operator

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    void split( std::string const &str, char delim, std::vector <std::string> &out );
    int stringToInt(std::string s);
    int customerSerialNum;
    std::vector<std::string> inputs;
};


#endif //SPLASSIGNMENT1_STUDIO_H
