//
// Created by ynal6 on 22/11/2021.
//

#ifndef SPLASSIGNMENT1_WORKOUT_H
#define SPLASSIGNMENT1_WORKOUT_H


#include <string>

enum WorkoutType{
    ANAEROBIC, MIXED, CARDIO
};

class Workout{
public:
    Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type);
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    WorkoutType getType() const;
    Workout& operator=( const Workout &other);//assignment operator
private:
    const int id;
    const std::string name;
    const int price;
    const WorkoutType type;
};


#endif //SPLASSIGNMENT1_WORKOUT_H
