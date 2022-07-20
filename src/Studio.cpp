//
// Created by ynal6 on 09/11/2021.
//


#include <cstring>
#include "../include/Studio.h"
#include<algorithm>
using namespace std;
Studio::Studio() {
open= true;
customerSerialNum=0;
}
Studio::Studio(const std::string &configFilePath) {
    customerSerialNum=0;
    open= true;
    std::ifstream file(configFilePath);
    string line;
    int numOfLine=1;
    int numOfTrainers=0;

        while (numOfLine<8 && getline(file, line)) {
            remove(line.begin(), line.end(), ' ');
            if (line[0] == '#') {
                numOfLine++;
                continue;
            } else if (line.length() == 0) {
                numOfLine++;
                continue;
            }

            if (numOfLine == 2) {
                numOfTrainers = line[0] - '0';
                numOfLine++;
            } else if (numOfLine == 5) {
                vector<std::string> c_splitInput;
                const char FirstSeparator = ',';
                split(line, FirstSeparator, c_splitInput);
                for (const string& s: c_splitInput) {
                    trainers.push_back(new Trainer(stringToInt(s)));
                }
                numOfLine++;
            }
        }

        if(numOfLine==8) {
        int workoutCount = 0;
        while (getline(file, line) && line!="\0") {
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            vector<std::string> w_splitInput;
            const char FirstSeparator = ',';
            split(line, FirstSeparator, w_splitInput);
            string name = w_splitInput[0];
            string type = w_splitInput[1];
            WorkoutType w_type;
            if (type == "Anaerobic")
                w_type = ANAEROBIC;
            else if (type == "Mixed")
                w_type = MIXED;
            else
                w_type = CARDIO;
            int price = stringToInt(w_splitInput[2]);
            workout_options.emplace_back(workoutCount, name, price, w_type);
            workoutCount++;
        }
        numOfLine++;
    }

}

void Studio::start() {
    cout << "Studio is now open!\n";
    OpenTrainer* openTrainer;
    Order* order;
    MoveCustomer* moveCustomer;
    Close* close;
    CloseAll* closeAll;
    PrintWorkoutOptions* printWorkoutOptions;
    PrintTrainerStatus* printTrainerStatus;
    PrintActionsLog* printActionsLog;
    BackupStudio* backupStudio;
    RestoreStudio* restoreStudio;

    while (open) {
        string input;
        getline(cin,input);
        this->inputs.push_back(input);
        vector<std::string> splitInput;
        const char FirstSeparator = ' ';
        split(input,FirstSeparator,splitInput);

        if (splitInput.size()>2)
        {
        if (splitInput[0] == "open") {

            vector<Customer *> customers;

            for (const string& s: splitInput) {
                if (s.find(',') != std::string::npos) {

                    const char secSeparator = ',';
                    vector<std::string> splitPair;
                    split(s, secSeparator, splitPair);
                    if (splitPair[1] == "swt")
                        customers.push_back(new SweatyCustomer(splitPair[0], customerSerialNum));
                    else if (splitPair[1] == "chp")
                        customers.push_back(new CheapCustomer(splitPair[0], customerSerialNum));
                    else if (splitPair[1] == "mcl")
                        customers.push_back(new HeavyMuscleCustomer(splitPair[0], customerSerialNum));
                    else if (splitPair[1] == "fbd")
                        customers.push_back(new FullBodyCustomer(splitPair[0], customerSerialNum));

                    customerSerialNum++;
                }
            }
            openTrainer = new OpenTrainer(stringToInt(splitInput[1]), customers);
            openTrainer->act(*this);
            actionsLog.push_back(openTrainer);
        }
        else if(splitInput[0] == "move") {
                moveCustomer=new MoveCustomer(stringToInt(splitInput[1]),stringToInt(splitInput[2]),stringToInt(splitInput[3]));
                moveCustomer->act(*this);
                actionsLog.push_back(moveCustomer);
                cout<<moveCustomer->toString();
            }
        }
        else if(splitInput[0]=="order") {
            order=new Order(stringToInt(splitInput[1]));
            order->act(*this);
            actionsLog.push_back(order);
            cout<<order->toString();
        }
        else if(splitInput[0]=="close") {
            close=new Close(stringToInt(splitInput[1]));
            close->act(*this);
            actionsLog.push_back(close);
            cout<<close->toString();
        }
        else if(splitInput[0]=="closeall") {
            closeAll=new CloseAll();
            closeAll->act(*this);
            actionsLog.push_back(closeAll);
            open= false;
            cout<< closeAll->toString();
        }
        else if(splitInput[0]=="workout_options") {
            printWorkoutOptions= new PrintWorkoutOptions();
            printWorkoutOptions->act(*this);
            actionsLog.push_back(printWorkoutOptions);
        }
        else if(splitInput[0]=="status") {
            printTrainerStatus=new PrintTrainerStatus(stringToInt(splitInput[1]));
            printTrainerStatus->act(*this);
            actionsLog.push_back(printTrainerStatus);
        }
        else if(splitInput[0]=="log") {
            printActionsLog= new PrintActionsLog();
            printActionsLog->act(*this);
            actionsLog.push_back(printActionsLog);
        }
        else if(splitInput[0]=="backup") {
            backupStudio=new BackupStudio();
            actionsLog.push_back(backupStudio);
            backupStudio->act(*this);

        }
        else if(splitInput[0]=="restore") {
            restoreStudio=new RestoreStudio();
            actionsLog.push_back(restoreStudio);
            restoreStudio->act(*this);

        }
    }
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {
    if(tid>=trainers.size() || tid<0)
        return nullptr;
    return trainers[tid];
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}

void Studio::split(const string &str, char delim, vector<std::string> &out) {
    std::stringstream s(str);
    std::string currString;
    while (std:: getline (s, currString, delim) )
    {
        out.push_back(currString);
    }
}

int Studio::stringToInt(std::string s) {
    stringstream stream(s);
    int val ;
    stream >> val;
    return val;
}

std::vector<std::string> Studio::getInputs() {
    return inputs;
}

Studio::~Studio() {
    for(Trainer* t:trainers) {
        delete t;
    }
    for(BaseAction* a:actionsLog) {
        delete a;
    }
}

Studio::Studio(const Studio &other) {
    for(Trainer* t:other.trainers)
        trainers.push_back(t->copy());
    for(BaseAction* a:other.actionsLog)
        actionsLog.push_back(a->copy());
    for(const Workout& w:other.workout_options)
        workout_options.push_back(w);
    for(const string& s:other.inputs)
        inputs.push_back(s);
    customerSerialNum=other.customerSerialNum;
    open=other.open;
}

Studio &Studio::operator=(const Studio &other) {

    if(this!=&other)
    {

        for(Trainer* t:trainers) {
            delete t;
        }

        for(BaseAction* a:actionsLog) {
            delete a;
        }
        /*for(int i=0;i<trainers.size();i++)
        {
            if(trainers[i]) {
                delete (trainers[i]);
                trainers[i]= nullptr;
            }
        }*/
        //std::vector<Customer*> customers= trainers[3]->getCustomers();
       // std::vector<Customer*> customers2= trainers[2]->getCustomers();
       /* for(int i=0;i<actionsLog.size();i++)
        {
            if(actionsLog[i]) {
                delete (actionsLog[i]);
                trainers[i]= nullptr;
            }
        }*/
        actionsLog.clear();
        trainers.clear();
        workout_options.clear();
        inputs.clear();
        for(Trainer* t:other.trainers)
            trainers.push_back(t->copy());
        for(BaseAction* a:other.actionsLog)
            actionsLog.push_back(a->copy());
        for(const Workout& w:other.workout_options)
            workout_options.push_back(w);
        for(const string& s:other.inputs)
            inputs.push_back(s);
        customerSerialNum=other.customerSerialNum;
        open=other.open;
    }
    return *this;
}

Studio::Studio( Studio &&other) {
    trainers=other.trainers;
    actionsLog=other.actionsLog;
    workout_options=other.workout_options;
    for(auto & trainer : other.trainers)
        trainer= nullptr;
    for(auto & i : other.actionsLog)
        i= nullptr;
}

Studio &Studio::operator=(Studio &&other) {
    for(Trainer* t:trainers) {
        delete t;
    }
    for(BaseAction* a:actionsLog) {
        delete a;
    }
    workout_options.clear();
    trainers=other.trainers;
    actionsLog=other.actionsLog;
    workout_options=other.workout_options;
    for(auto & trainer : other.trainers)
        trainer= nullptr;
    for(auto & i : other.actionsLog)
        i= nullptr;
    return *this;
}










