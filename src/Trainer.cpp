//
// Created by ynal6 on 09/11/2021.
//
//#include <algorithm>
#include <vector>
#include "../include/Trainer.h"


using namespace std;

Trainer::Trainer(int t_capacity):capacity(t_capacity) {
    open= false;
    salary=0;
    isOrdered=false;
}

int Trainer::getCapacity() const {
    return capacity;

}
void  Trainer::addCustomer(Customer *customer) {
    if(customer!= nullptr)
        customersList.push_back(customer);
}

void  Trainer::removeCustomer(int id) {

    if(!customersList.empty()) {
        int index = 0;
        for (Customer *c: customersList) {
            if (c->getId() == id) {
                customersList.erase(customersList.begin() + index);
                break;
            } else
                index++;
        }

        index = 0;
        for (const OrderPair& o: orderList) {
            if (o.first == id) {
                orderList.erase(orderList.begin() + index);
                salary -= o.second.getPrice();
            }
            index++;
        }
        if (customersList.empty())
            closeTrainer();
    }

}

Customer* Trainer::getCustomer(int id){
    for(Customer* c: customersList)
    {
        if(c->getId()==id) {
            return c;
        }
    }
    return nullptr;
}
std::vector<Customer*>& Trainer::getCustomers(){
    return customersList;
}
std::vector<OrderPair>& Trainer::getOrders(){
    return orderList;
}
void Trainer::order(const int customer_id, const std::vector<int>& workout_ids, const std::vector<Workout>& workout_options){
    if(getCustomer(customer_id)!= nullptr ) {
        for (int w_id: workout_ids) {
            for (const Workout& w: workout_options) {
                if (w.getId() == w_id) {
                    orderList.emplace_back(customer_id, w);
                    salary+=w.getPrice();
                }
            }
        }
    }
}
void Trainer::openTrainer(){
    this->open= true;
}
void Trainer::closeTrainer(){
    isOrdered=false;
    for (Customer* c:customersList)
        delete(c);
    customersList.clear();
    orderList.clear();
    this->open= false;
}
int Trainer::getSalary() const{
    return  salary;
}
bool Trainer::isOpen() const{
    return open;
}

Trainer::~Trainer() {
    for(Customer* c:customersList)
        delete c;
}

Trainer::Trainer(const Trainer &other) {
    capacity=other.capacity;
    open=other.open;
    for(Customer* c:other.customersList)
        this->customersList.push_back(c->copy());
    for(const OrderPair& p:other.orderList)
        this->orderList.push_back(p);
    if (other.isOrdered)
        isOrdered=true;
    else
        isOrdered = false;
    salary = other.salary;
}

Trainer& Trainer::operator=(const Trainer &other) {
    if (this != &other) {
        capacity = other.capacity;
        open = other.open;
        for(Customer* c:customersList) {
                delete c;
        }
        orderList.clear();
        for(Customer* c:other.customersList)
            this->customersList.push_back(c->copy());
        for(const OrderPair& p:other.orderList)
            this->orderList.push_back(p);
    }
    return *this;

}

Trainer::Trainer(Trainer &&other) {
    capacity = other.capacity;
    open = other.open;
    customersList=other.customersList;
    orderList=other.orderList;
    salary = other.salary;
    isOrdered = other.isOrdered;
    for(auto & i : other.customersList)
        i= nullptr;
}

Trainer &Trainer::operator=(Trainer &&other) {
    capacity = other.capacity;
    open = other.open;
    for(Customer* c:customersList) {
            delete c;
    }
    orderList.clear();
    customersList=other.customersList;
    orderList=other.orderList;
    for(auto & i : other.customersList)
        i= nullptr;
    return *this;
}

Trainer *Trainer::copy() {
    return new Trainer(*(this));
}

void Trainer::IsOrdered(bool val) {
    isOrdered=val;
}

bool Trainer::IsOrdered() const {
    return isOrdered;
}
