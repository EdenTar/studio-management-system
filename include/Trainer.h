//
// Created by ynal6 on 22/11/2021.
//

#ifndef SPLASSIGNMENT1_TRAINER_H
#define SPLASSIGNMENT1_TRAINER_H


#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    explicit Trainer(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int>& workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary() const;
    bool isOpen() const;
    //rule of 5:
    virtual ~Trainer();//destructor
    Trainer(const Trainer &other);//copy constructor
    Trainer& operator=(const Trainer &other);//assignment operator
    Trainer(Trainer&& other); //move constructor
    Trainer& operator=(Trainer&& other); //move assignment operator
    Trainer* copy();
    void IsOrdered(bool);
    bool IsOrdered() const;

private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    bool isOrdered;
    int salary;

};


#endif //SPLASSIGNMENT1_TRAINER_H
