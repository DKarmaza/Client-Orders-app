#include "Database.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

Database::Database(const std::string& file) : filename(file) {}

void Database::loadFromFile() {
    std::ifstream file(filename);
    if (!file.is_open()) return; 

    orders.clear(); 
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string item;
        Order order;

        try {
            std::getline(ss, item, '|'); order.id = std::stoi(item);
            std::getline(ss, order.customerName, '|');
            std::getline(ss, order.phoneNumber, '|');
            std::getline(ss, order.productName, '|');
            std::getline(ss, item, '|'); order.quantity = std::stoi(item);
            std::getline(ss, item, '|'); order.unitPrice = std::stod(item);
            std::getline(ss, order.status, '|');

            orders.push_back(order);
        } catch (...) {

        }
    }
    file.close();
}

void Database::saveToFile() const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не вдалося відкрити файл для збереження даних!");
    }

    for (const auto& order : orders) {
        file << order.id << "|"
             << order.customerName << "|"
             << order.phoneNumber << "|"
             << order.productName << "|"
             << order.quantity << "|"
             << order.unitPrice << "|"
             << order.status << "\n";
    }
    file.close();
}

void Database::addOrder(const Order& newOrder) {
    orders.push_back(newOrder);
}

bool Database::orderExists(int id) const {
    auto it = std::find_if(orders.begin(), orders.end(), 
                           [id](const Order& o) { return o.id == id; });
    return it != orders.end();
}

Order Database::getOrder(int id) const {
    auto it = std::find_if(orders.begin(), orders.end(), 
                           [id](const Order& o) { return o.id == id; });
    if (it != orders.end()) {
        return *it;
    }
    return Order{}; 
}

void Database::displayAll() const {
    if (orders.empty()) {
        std::cout << "База даних порожня.\n";
        return;
    }

    std::cout << std::string(120, '-') << "\n";
    std::cout << std::left 
              << std::setw(5)  << "ID" 
              << std::setw(35) << "Клієнт" 
              << std::setw(15) << "Телефон" 
              << std::setw(20) << "Товар" 
              << std::setw(10) << "К-ть" 
              << std::setw(12) << "Ціна/од." 
              << std::setw(12) << "Сума" 
              << std::setw(15) << "Статус" << "\n";
    std::cout << std::string(120, '-') << "\n";

    for (const auto& order : orders) {
        std::cout << std::left 
                  << std::setw(5)  << order.id 
                  << std::setw(35) << order.customerName 
                  << std::setw(15) << order.phoneNumber 
                  << std::setw(20) << order.productName 
                  << std::setw(10) << order.quantity 
                  << std::setw(12) << std::fixed << std::setprecision(2) << order.unitPrice 
                  << std::setw(12) << std::fixed << std::setprecision(2) << order.getTotalPrice() 
                  << std::setw(15) << order.status << "\n";
    }
    std::cout << std::string(120, '-') << "\n";
}

bool Database::updateOrder(int id, const Order& updatedOrder) {
    auto it = std::find_if(orders.begin(), orders.end(), 
                           [id](const Order& o) { return o.id == id; });
    
    if (it != orders.end()) {
        *it = updatedOrder; 
        return true;
    }
    return false;
}

bool Database::deleteOrder(int id) {
    auto it = std::find_if(orders.begin(), orders.end(), 
                           [id](const Order& o) { return o.id == id; });
    
    if (it != orders.end()) {
        orders.erase(it); 
        return true;
    }
    return false;
}

void Database::displaySummary() const {
    if (orders.empty()) {
        std::cout << "Немає даних для підбиття підсумків.\n";
        return;
    }

    int totalQuantity = 0;
    double totalRevenue = 0.0;

    for (const auto& order : orders) {
        totalQuantity += order.quantity;
        totalRevenue += order.getTotalPrice();
    }

    std::cout << "\n--- ПІДСУМКОВА ІНФОРМАЦІЯ ---\n";
    std::cout << "Загальна кількість замовлень: " << orders.size() << "\n";
    std::cout << "Загалом замовлено товарів (одиниць): " << totalQuantity << "\n";
    std::cout << "Загальна вартість усіх замовлень: " << std::fixed << std::setprecision(2) << totalRevenue << " грн\n";
    std::cout << "-----------------------------\n";
}