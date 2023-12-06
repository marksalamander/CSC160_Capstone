#ifndef RENTAL_H
#define RENTAL_H

#include <string>

class rental {
public:
    void blank_line(const char* file_name);
    void delete_line(const char* file_name, int n);

    void menu();

    void displayInfo();
    void admin();
    void user();

    void removeCar(const char *file_name);
    void addCar();
    std::string newCar();
    void returnCar();

    void setCar(std::string car);
    std::string getCar();

    void setLineNum(int num);
    int getLineNum();

    void total();
    void inputInfo();

private:
    std::string m_car;
    int m_lineNum;
};

#endif
