//
// Created by ynal6 on 09/11/2021.
//

#include <algorithm>
#include <utility>
#include "../include/Customer.h"

Customer::Customer(std::string c_name, int c_id) : id(c_id), name(std::move(c_name)) {

}
std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

Customer::~Customer() = default;

SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(std::move(name), id) {

}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> final_workouts {};
    for(const auto& workout : workout_options)
    {
        if (workout.getType()==2)
            final_workouts.push_back(workout.getId());
    }
    return final_workouts;
}

std::string SweatyCustomer::toString() const {
    return std::to_string(getId())+" "+getName()+",swt";
}

Customer *SweatyCustomer::copy() {
    auto* customer=new SweatyCustomer(this->getName(), this->getId());
    return customer;
}

CheapCustomer::CheapCustomer(std::string name, int id) : Customer(std::move(name), id) {

}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> final_workouts {};
    int cheapestPrice=-1;
    for(const auto& workout : workout_options) {
        if (workout.getPrice()<cheapestPrice || cheapestPrice==-1) {
            final_workouts.clear();
            final_workouts.push_back(workout.getId());
            cheapestPrice = workout.getPrice();
        }
    }
    return final_workouts;
}

std::string CheapCustomer::toString() const {
    return std::to_string(getId())+" "+getName()+",chp";
}

Customer *CheapCustomer::copy() {
    auto* customer=new CheapCustomer(this->getName(), this->getId());
    return customer;
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(std::move(name), id) {

}

bool sortbyPrice(std::pair<int,int> &a, std::pair<int,int> &b)
{
    if (a.first==b.first)
        return (a.second<b.second);
    return (a.first > b.first);
}
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<std::pair<int, int>> price_id {};
    //save all anaerobic workouts using their price and id
    for(const auto& workout : workout_options) {
        if (workout.getType()==0)
            price_id.emplace_back(workout.getPrice(), workout.getId());
    }
    if (price_id.empty())
        return {};

    //sort by price (decreasing)
    std::sort(price_id.begin(), price_id.end(), sortbyPrice);

    //save the id according to the same order
    std::vector<int> final_workouts{};
    for (auto price_id_pair : price_id)
        final_workouts.push_back(price_id_pair.second);
    return final_workouts;
}

std::string HeavyMuscleCustomer::toString() const {
    return std::to_string(getId())+" "+getName()+",mcl";
}

Customer *HeavyMuscleCustomer::copy() {
    auto* customer=new HeavyMuscleCustomer(this->getName(), this->getId());
    return customer;
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(std::move(name), id) {

}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    int cheapest_cardio_id=-1,  expensive_mixtype_id=-1 ,  cheapest_anaerobic_id=-1;
    int cheapest_cardio_price=-1,  expensive_mixtype_price=-1 ,  cheapest_anaerobic_price=-1;

    for(const auto& workout : workout_options) {
        if ((workout.getType() == 2 )&
            (cheapest_cardio_price > workout.getPrice() || cheapest_cardio_id == -1 ||
             (cheapest_cardio_price==workout.getPrice() & cheapest_cardio_id>workout.getId()))) {
            cheapest_cardio_id = workout.getId();
            cheapest_cardio_price = workout.getPrice();
        } else if ((workout.getType() == 0) &
                   (cheapest_anaerobic_price > workout.getPrice() || cheapest_anaerobic_id == -1 ||
                    (cheapest_anaerobic_price==workout.getPrice() & cheapest_anaerobic_id>workout.getId()))) {
            cheapest_anaerobic_id = workout.getId();
            cheapest_anaerobic_price = workout.getPrice();
        } else if ((workout.getType() == 1) &
                   (expensive_mixtype_price < workout.getPrice() || expensive_mixtype_id == -1 ||
                    (expensive_mixtype_price==workout.getPrice() & expensive_mixtype_id>workout.getId()))) {
            expensive_mixtype_id = workout.getId();
            expensive_mixtype_price = workout.getPrice();
        }
    }

    std::vector<int> final_workouts{};
    if (!(cheapest_anaerobic_id==-1 || cheapest_cardio_id==-1 || expensive_mixtype_id ==-1)) {
        final_workouts.push_back(cheapest_cardio_id);
        final_workouts.push_back(expensive_mixtype_id);
        final_workouts.push_back(cheapest_anaerobic_id);
    }
    return final_workouts;
}

std::string FullBodyCustomer::toString() const {
    return std::to_string(getId())+" "+getName()+",fbd";
}

Customer *FullBodyCustomer::copy() {
    auto* customer=new FullBodyCustomer(this->getName(), this->getId());
    return customer;
}