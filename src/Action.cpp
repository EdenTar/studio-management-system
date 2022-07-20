//
// Created by ynal6 on 22/11/2021.
//
#include "../include/Action.h"

#include <utility>
#include "Studio.h"
extern Studio* backup;

BaseAction::BaseAction() = default;

void BaseAction::complete() {
    status=COMPLETED;
}

void BaseAction::error(std::string errormsg) {
    status=ERROR;
    this->errorMsg=std::move(errormsg);
}

std::string BaseAction::getErrorMsg() const {
    return "Error: "+errorMsg;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

std::string BaseAction::getErrorMessage() const {
    return getErrorMsg();
}

BaseAction::~BaseAction() = default;


OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id) {
    customers = customersList;
}

void OpenTrainer::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer== nullptr || trainer->isOpen()) {
        error("Trainer does not exist or is already open.");
        std::cout<<getErrorMessage()<<std::endl;
        return;
    }

    trainer->openTrainer();
    int capacity = 0;
    for (Customer *customer: customers) {
        if (capacity < trainer->getCapacity()) {
            trainer->addCustomer(customer->copy());
            capacity++;
        }
    }
    complete();
}

std::string OpenTrainer::toString() const {
    return "open Trainer";
}

BaseAction *OpenTrainer::copy() {
    std::vector<Customer *> customersCopy = {};
    for (Customer* c : customers){
        customersCopy.push_back(c->copy());
    }
    auto* action=new OpenTrainer((*this).trainerId,customersCopy);
    if(this->getStatus()==ERROR)
        action->error(this->getErrorMessage());
    else
        action->complete();
    return action;
}

int OpenTrainer::getTrainerId() const {
    return trainerId;
}

OpenTrainer::~OpenTrainer() {
    for(Customer* c:customers) {
        delete c;
    }
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {

}

void MoveCustomer::act(Studio &studio) {
    int srcTrainerId = srcTrainer, dstTrainerId = dstTrainer;
    Trainer *SrcTrainer = studio.getTrainer(srcTrainerId);
    Trainer *DstTrainer = studio.getTrainer(dstTrainerId);

    if (SrcTrainer== nullptr || DstTrainer == nullptr || SrcTrainer->getCustomer(id)== nullptr ||DstTrainer->getCapacity()== DstTrainer->getCustomers().size())
    {
        error("Cannot move customer");
        std::cout<<getErrorMessage()<<std::endl;
        return;
    }

    if (!DstTrainer->isOpen()) {
        error("session of dst trainer is close");
        std::cout<<getErrorMessage()<<std::endl;
        return;
    }

    Customer *customer = SrcTrainer->getCustomer(id);
    std::vector<OrderPair>& customerOrders = SrcTrainer->getOrders();

    std::vector<int> workout_ids = {};
    std::vector<Workout> workout_orders = {};

    for (const OrderPair& orderPair : customerOrders)
    {
        if (orderPair.first == id) {
            workout_orders.push_back(orderPair.second);
            workout_ids.push_back(orderPair.second.getId());
        }
    }
    SrcTrainer->removeCustomer(id);
    if (SrcTrainer->getCustomers().empty()) {
        SrcTrainer->closeTrainer();
    }

    DstTrainer->addCustomer(customer);
    if (!workout_orders.empty())
        DstTrainer->order(id, workout_ids, workout_orders);
    complete();
}

std::string MoveCustomer::toString() const {
    return {};
}

BaseAction *MoveCustomer::copy() {
    auto* action=new MoveCustomer(this->srcTrainer, this->dstTrainer, this->id);
    if(this->getStatus()==ERROR)
        action->error(this->getErrorMessage());
    else
        action->complete();
    return action;
}


CloseAll::CloseAll() = default;

void CloseAll::act(Studio &studio) {
    for (int i=0 ; i<studio.getNumOfTrainers() ; i++) {
        if (studio.getTrainer(i)->isOpen()) {
            auto *close = new Close(i);
            close->act(studio);
            std::cout<<close->toString();
            delete close;
        }
    }
    complete();
}

std::string CloseAll::toString() const {
    return "";
}

BaseAction *CloseAll::copy() {
    auto* action=new CloseAll();
    if(this->getStatus()==ERROR)
        action->error(this->getErrorMessage());
    else
        action->complete();
    return action;
}

PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id){

}

void PrintTrainerStatus::act(Studio &studio) {
    if (studio.getNumOfTrainers()>trainerId) {
        if (studio.getTrainer(trainerId)->isOpen()) {
            std::cout << "Trainer " << trainerId << " status: open" << std::endl;
            Trainer *trainer = studio.getTrainer(trainerId);
            if (trainer->getOrders().empty())
                return;
            std::cout << "Customers:" << std::endl;
            for (Customer *customer: trainer->getCustomers())
                std::cout << customer->getId() << " " << customer->getName() << std::endl;

            std::cout << "Orders:" << std::endl;
            for (const std::pair<int, Workout> &workout: trainer->getOrders())
                std::cout << workout.second.getName() << " " << workout.second.getPrice() << "NIS " << workout.first
                          << std::endl;

            std::cout << "Current Trainer's Salary: " << trainer->getSalary() << std::endl;;
        } else {
            std::vector<BaseAction *> actionLog = studio.getActionsLog();
            int tmp =0;
            for (BaseAction *a: studio.getActionsLog()) {
                if ((typeid(a)==typeid(OpenTrainer)) && (((OpenTrainer *) a)->getTrainerId() == trainerId)) {
                    if (tmp == 0)
                        std::cout << "Trainer " << trainerId << " status: closed. Current Trainer's Salary: "
                                  << studio.getTrainer(trainerId)->getSalary() << std::endl;
                    tmp++;
                }
            }
        }
        complete();
    }
}

BaseAction *PrintTrainerStatus::copy() {
    auto* action=new PrintTrainerStatus(this->trainerId);
    if(this->getStatus()==ERROR)
        action->error(this->getErrorMessage());
    else
        action->complete();
    return action;
}

std::string PrintTrainerStatus::toString() const {
    return {};
}

BackupStudio::BackupStudio() = default;

void BackupStudio::act(Studio &studio) {
    delete backup;
    backup = new Studio(studio);
    complete();
}

BaseAction *BackupStudio::copy() {
    auto* action=new BackupStudio();
    if(this->getStatus()==ERROR)
        action->error(this->getErrorMessage());
    else
        action->complete();
    return action;
}

std::string BackupStudio::toString() const {
    return {};
}

Order::Order(int id):trainerId(id) {

}

void Order::act(Studio &studio) {
    Trainer* trainer=studio.getTrainer(trainerId);
    this->studio = &studio;
    if(trainer!= nullptr && trainer->isOpen()) {
        if( !trainer->IsOrdered()) {
            std::vector<Customer *> customers = trainer->getCustomers();
            for (Customer *c: customers) {
                trainer->order(c->getId(), c->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
            }
            complete();
            trainer->IsOrdered(true);
        }
    }
    else {
        error("Trainer does not exist or is not open\n");
    }

}

std::string Order::toString() const {
    std::string output="";
    if (getStatus()!=ERROR) {
        Trainer *trainer = studio->getTrainer(trainerId);
        for (const OrderPair& o: trainer->getOrders())
            for (Customer *c: trainer->getCustomers())
                if (o.first == c->getId())
                    output += c->getName() + " is doing " + o.second.getName() + "\n";
        return output;
    }
    return getErrorMessage();
}

BaseAction *Order::copy() {
    auto* action=new Order(this->trainerId);
    if(this->getStatus()==ERROR)
        action->error(this->getErrorMessage());
    else
        action->complete();
    return action;
}

Close::Close(int id):trainerId(id) {}

void Close::act(Studio &studio) {
    Trainer* trainer=studio.getTrainer(trainerId);
    this->studio=&studio;
    if (trainer != nullptr && trainer->isOpen() ) {
        trainer->closeTrainer();
        complete();
    } else {
        error("Trainer does not exist or is not open\n");
        std::cout << getErrorMessage();
    }

}

std::string Close::toString() const {
    Trainer* trainer=studio->getTrainer(trainerId);
    std::string output="Trainer "+std::to_string(trainerId)+std::string(" closed. salary: ")+std::to_string(trainer->getSalary())+"\n";
    return output;
}

BaseAction *Close::copy() {
    auto* action=new Close(this->trainerId);
    if(this->getStatus()==ERROR)
        action->error(this->getErrorMessage());
    else
        action->complete();
    return action;
}

PrintWorkoutOptions::PrintWorkoutOptions() = default;

void PrintWorkoutOptions::act(Studio &studio) {
    for(const Workout& w: studio.getWorkoutOptions())
        std::cout<<w.getName()<<" ,"<<getStringType(w.getType())<<" ,"<<w.getPrice()<<std::endl;
    complete();
}

std::string PrintWorkoutOptions::toString() const {
    return {};
}

BaseAction *PrintWorkoutOptions::copy() {
    auto* action=new PrintWorkoutOptions();
    if(this->getStatus()==ERROR)
        action->error(this->getErrorMessage());
    else
        action->complete();
    return action;
}

std::string PrintWorkoutOptions::getStringType(int type) {
    if(type==0)
        return "Anaerobic";
    else if(type==1)
        return "Mixed";
    else
        return "Cardio";

}

PrintActionsLog::PrintActionsLog() = default;

void PrintActionsLog::act(Studio &studio) {
    int index=0;
    for(BaseAction* a:studio.getActionsLog()){
        if(a->getStatus()==0)
            std::cout<<studio.getInputs()[index]<<" "<<getStatusString(a->getStatus())<<std::endl;
        else
            std::cout<<studio.getInputs()[index]<<" "<<a->getErrorMessage()<<std::endl;
        index++;

    }
}


std::string PrintActionsLog::toString() const {
    return {};
}

BaseAction *PrintActionsLog::copy() {
    auto* action=new PrintActionsLog();
    if(this->getStatus()==ERROR)
        action->error(this->getErrorMessage());
    else
        action->complete();
    return action;
}

std::string PrintActionsLog::getStatusString(ActionStatus status) {
    if(status==COMPLETED)
        return "Completed";
    return "Error";
}


RestoreStudio::RestoreStudio() = default;

void RestoreStudio::act(Studio &studio) {
    if (backup== nullptr) {
        error("No backup available\n");
        std::cout<<getErrorMessage();
        return;
    }
    studio.operator=(*backup);
    complete();
}

std::string RestoreStudio::toString() const {
    return {};
}

BaseAction *RestoreStudio::copy() {
    auto* action=new RestoreStudio();
    if(this->getStatus()==ERROR)
        action->error(this->getErrorMessage());
    else
        action->complete();
    return action;
}
