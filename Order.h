#ifndef ORDER_H
#define ORDER_H

#include <string>

struct Order {
    int id;
    std::string customerName;
    std::string phoneNumber;
    std::string productName;
    int quantity;
    double unitPrice;
    std::string status;

    // метод автоматичного розрахунку вартості замовлення
    double getTotalPrice() const {
        return quantity * unitPrice;
    }
};

#endif