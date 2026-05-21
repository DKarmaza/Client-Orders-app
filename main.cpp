#include <iostream>
#include <limits>
#include <string>
#include <cctype> // бібліотека для функції std::isdigit (перевірка на цифри)
#include "Database.h"
#include <windows.h> //бібліотека для налаштування консолі Windows

//функція для перевірки формату телефону
bool isValidPhoneNumber(const std::string& phone) {
    // перевірка формату: +380XXXXXXXXX (13 символів)
    if (phone.length() == 13 && phone.substr(0, 4) == "+380") {
        for (size_t i = 1; i < phone.length(); ++i) {
            if (!std::isdigit(phone[i])) return false; // Якщо є хоч одна буква - помилка
        }
        return true;
    } 
    // перевірка формату: 0XXXXXXXXX (10 символів)
    else if (phone.length() == 10 && phone[0] == '0') {
        for (size_t i = 0; i < phone.length(); ++i) {
            if (!std::isdigit(phone[i])) return false; // якщо є хоч одна буква - помилка
        }
        return true;
    }
    return false; // якщо не підійшов жоден формат
}

// доп. функція для безпечного введення цілих чисел
int getSafeInt() {
    int value;
    while (!(std::cin >> value) || value < 0) {
        std::cin.clear(); // видаляемо прапорець помилки
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ігнорування некоректного вводу
        std::cout << "[Помилка] Будь ласка, введіть ціле число (0 або більше): ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буферу після числа
    return value;
}

// доп. функція для безпечного введення дійсних чисел
double getSafeDouble() {
    double value;
    while (!(std::cin >> value) || value < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[Помилка] Будь ласка, введіть додатне число або 0 (використовуйте крапку для дробів): ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::string chooseStatus() {
    int choice = 0;
    std::string statuses[] = {
        "Нове", 
        "В обробці", 
        "В дорозі", 
        "Надіслано покупцю", 
        "Отримано покупцем"
    };
    
    while (true) {
        std::cout << "\n--- Оберіть статус замовлення ---\n";
        for (int i = 0; i < 5; ++i) {
            std::cout << i + 1 << ". " << statuses[i] << "\n";
        }
        std::cout << "Ваш вибір (1-5): ";
        
        choice = getSafeInt();
        
        if (choice >= 1 && choice <= 5) {
            return statuses[choice - 1]; // Повертаємо текст статусу
        } else {
            std::cout << "[Помилка] Невірний вибір. Введіть число від 1 до 5.\n";
        }
    }
}

void printMenu() {
    std::cout << "\n=========================================\n";
    std::cout << "      БАЗА ДАНИХ ЗАМОВЛЕНЬ КЛІЄНТІВ      \n";
    std::cout << "=========================================\n";
    std::cout << "1. Відкрити (завантажити) базу з файлу\n";
    std::cout << "2. Зберегти базу у файл\n";
    std::cout << "3. Відобразити всі замовлення\n";
    std::cout << "4. Додати нове замовлення\n";
    std::cout << "5. Редагувати замовлення\n";
    std::cout << "6. Видалити замовлення\n";
    std::cout << "7. Підсумкова інформація\n";
    std::cout << "8. Про програму\n";
    std::cout << "0. Вийти\n";
    std::cout << "=========================================\n";
    std::cout << "Оберіть дію: ";
}

int main() {
    // кодування Windows 1251 для консолі Windows для того щоб кирилиця була однобайтовую та виводилася
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    system("mode con cols=130 lines=30");
    
    Database db("orders_data.txt");
    int choice;                     

    // спроба автоматично завантажити дані при старті
    try {
        db.loadFromFile();
        std::cout << "[Інфо] Дані успішно завантажені при запуску програми.\n";
    } catch (const std::exception& e) {
        std::cerr << "[Попередження] Не вдалося завантажити файл: " << e.what() << "\n";
    }

    do {
        printMenu();
        choice = getSafeInt();

        switch (choice) {
            case 1:
                try {
                    db.loadFromFile();
                    std::cout << "[Успіх] Дані завантажено з файлу.\n";
                } catch (const std::exception& e) {
                    std::cerr << "[Помилка] " << e.what() << "\n";
                }
                break;

            case 2:
                try {
                    db.saveToFile();
                    std::cout << "[Успіх] Дані успішно збережено у файл.\n";
                } catch (const std::exception& e) {
                    std::cerr << "[Помилка] " << e.what() << "\n";
                }
                break;

            case 3:
                db.displayAll();
                break;

            case 4: {
                Order newOrder;
                std::cout << "\n--- ДОДАВАННЯ ЗАМОВЛЕННЯ ---\n";
                
                std::cout << "ID замовлення: ";
                newOrder.id = getSafeInt();
                
                if (db.orderExists(newOrder.id)) {
                    std::cout << "[Помилка] Замовлення з таким ID вже існує!\n";
                    break;
                }

                std::cout << "ПІБ клієнта: ";
                std::getline(std::cin, newOrder.customerName);
                
                //запит телефону з перевіркою формату
                std::cout << "Номер телефону (+380... або 0...): ";
                while (true) {
                    std::getline(std::cin, newOrder.phoneNumber);
                    if (isValidPhoneNumber(newOrder.phoneNumber)) {
                        break;
                    }
                    std::cout << "[Помилка] Тільки цифри! Формат +380XXXXXXXXX або 0XXXXXXXXX: ";
                }

                std::cout << "Назва товару: ";
                std::getline(std::cin, newOrder.productName);
                
                std::cout << "Кількість одиниць: ";
                newOrder.quantity = getSafeInt();
                
                std::cout << "Ціна за одиницю (грн): ";
                newOrder.unitPrice = getSafeDouble();
                // виклик меню статусів
                newOrder.status = chooseStatus();

                db.addOrder(newOrder);

                // блок зберігання та автозбереження нових данних у файл
                try {
                    db.saveToFile(); 
                    std::cout << "[Успіх] Замовлення додано та надійно збережено.\n";
                } catch (const std::exception& e) {
                    std::cerr << "[Помилка збереження] " << e.what() << "\n";
                }
                break;
            }

           case 5: {
                std::cout << "Введіть ID замовлення для редагування: ";
                int editId = getSafeInt();
                
                // перевірка, чи є таке замовлення
                if (!db.orderExists(editId)) {
                    std::cout << "[Помилка] Замовлення з таким ID не знайдено.\n";
                    break;
                }

                // отримання копії старого замовлення з бази
                Order updatedOrder = db.getOrder(editId); 
                std::string input; // змінна для тимчасового збереження вводу

                std::cout << "\n--- РЕДАГУВАННЯ ЗАМОВЛЕННЯ ---\n";
                std::cout << "Підказка: Залиште поле порожнім (натисніть Enter), щоб не змінювати його.\n\n";

                // 2. редагвання ПІБ
                std::cout << "ПІБ клієнта [" << updatedOrder.customerName << "]: ";
                std::getline(std::cin, input);
                // якщо рядок не порожній - оновлюємо,а якщо порожній - залишається старе
                if (!input.empty()) updatedOrder.customerName = input;

                // оновлене редагування телефону з перевіркою формату (із можливістю пропуску)
                std::cout << "Номер телефону [" << updatedOrder.phoneNumber << "]: ";
                while (true) {
                    std::getline(std::cin, input);
                    if (input.empty()) {
                        break; // залишаємо старий номер
                    }
                    if (isValidPhoneNumber(input)) {
                        updatedOrder.phoneNumber = input;
                        break;
                    }
                    std::cout << "[Помилка] Тільки цифри! Формат +380XXXXXXXXX або 0XXXXXXXXX (або Enter для пропуску): ";
                }

                // редагуваннятовару
                std::cout << "Назва товару [" << updatedOrder.productName << "]: ";
                std::getline(std::cin, input);
                if (!input.empty()) updatedOrder.productName = input;
                
                // редагування кількості 
                std::cout << "Кількість одиниць [" << updatedOrder.quantity << "]: ";
                std::getline(std::cin, input);
                if (!input.empty()) {
                    try { 
                        int val = std::stoi(input);
                        // перевірка на від'ємні (але 0 дозволено)
                        if (val >= 0) updatedOrder.quantity = val; 
                        else std::cout << "[Помилка] Число не може бути від'ємним.\n";
                    } catch (...) { 
                        std::cout << "[Помилка] Некоректне число, залишено старе значення (" << updatedOrder.quantity << ").\n"; 
                    }
                }
                
                // 6. редагування ціни 
                std::cout << "Ціна за одиницю [" << updatedOrder.unitPrice << "]: ";
                std::getline(std::cin, input);
                if (!input.empty()) {
                    try { 
                        double val = std::stod(input);
                        if (val >= 0) updatedOrder.unitPrice = val; 
                        else std::cout << "[Помилка] Ціна не може бути від'ємною.\n";
                    } catch (...) { 
                        std::cout << "[Помилка] Некоректне число, залишено старе значення (" << updatedOrder.unitPrice << ").\n"; 
                    }
                }
                
                // редагування статус
                std::cout << "Поточний статус: [" << updatedOrder.status << "]\n";
                std::cout << "Бажаєте змінити статус? (1 - Так, 0 - Ні): ";
                int changeStatus = getSafeInt();
                if (changeStatus == 1) {
                    updatedOrder.status = chooseStatus(); // виклик меню статусів
                }

                //оновлення даних в базі
                if (db.updateOrder(editId, updatedOrder)) {
                    try {
                        db.saveToFile(); //  автозбереження
                        std::cout << "[Успіх] Замовлення оновлено та збережено.\n";
                    } catch (const std::exception& e) {
                        std::cerr << "[Помилка збереження] " << e.what() << "\n";
                    }
                }
                break;
            }

            case 6: {
                std::cout << "Введіть ID замовлення для видалення: ";
                int deleteId = getSafeInt();
                if (db.deleteOrder(deleteId)) {
                    std::cout << "[Успіх] Замовлення видалено.\n";
                    try {
                        db.saveToFile(); // автозбереження після видалення
                    } catch (const std::exception& e) {
                        std::cerr << "[Помилка автозбереження] " << e.what() << "\n";
                    }
                } else {
                    std::cout << "[Помилка] Замовлення з таким ID не знайдено.\n";
                }
                break;
            }

            case 7:
                db.displaySummary();
                break;

            case 8:
                std::cout << "\n--- ПРО ПРОГРАМУ ---\n";
                std::cout << "Курсовий проєкт з дисциплін 'Програмування' та 'Алгоритми та структури даних'.\n";
                std::cout << "Тема: База даних замовлень клієнтів.\n";
                std::cout << "Розробник: Кармаза Данило Валерійович\n";
                std::cout << "--------------------\n";
                break;

            case 0:
                try {
                    db.saveToFile(); // збереження даних перед виходом
                    std::cout << "Дані збережено. Вихід з програми...\n";
                } catch (const std::exception& e) {
                    std::cerr << "[Помилка при збереженні] " << e.what() << "\n";
                }
                break;

            default:
                std::cout << "[Помилка] Невідома команда. Спробуйте ще раз.\n";
        }
    } while (choice != 0);

    return 0;
}