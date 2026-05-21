#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include "Order.h"

/**
 * @brief Клас для керування базою даних замовлень.
 * Відповідає за основний функціонал програми: CRUD-операції, 
 * роботу з файлами та підрахунок підсумків.
 */
class Database {
private:
    std::vector<Order> orders;  ///< Контейнер STL для зберігання даних у пам'яті
    std::string filename;       ///< Шлях до файлу для збереження даних

public:
    /**
     * @brief Конструктор бази даних.
     * @param file Ім'я файлу, з яким буде працювати база.
     */
    Database(const std::string& file);

    /**
     * @brief Завантажує дані з текстового файлу в пам'ять.
     * @throws std::runtime_error Якщо виникла помилка читання.
     */
    void loadFromFile();

    /**
     * @brief Записує всі поточні дані з пам'яті у файл.
     * @throws std::runtime_error Якщо файл не вдалося відкрити.
     */
    void saveToFile() const;

    /**
     * @brief Додає нове замовлення до бази.
     * @param newOrder Об'єкт нового замовлення.
     */
    void addOrder(const Order& newOrder);

    /**
     * @brief Перевіряє, чи існує замовлення з вказаним ID.
     * @param id Ідентифікатор для перевірки.
     * @return true, якщо існує, інакше false.
     */
    bool orderExists(int id) const;

    /**
     * @brief Отримує копію замовлення за його ID.
     * @param id Ідентифікатор замовлення.
     * @return Об'єкт Order із знайденими даними (або порожній, якщо не знайдено).
     */
    Order getOrder(int id) const;

    /**
     * @brief Виводить всі замовлення у вигляді таблиці.
     */
    void displayAll() const;

    /**
     * @brief Оновлює дані існуючого замовлення за його ID.
     * @param id Ідентифікатор замовлення для редагування.
     * @param updatedOrder Нові дані замовлення.
     * @return true, якщо оновлено успішно, false - якщо ID не знайдено.
     */
    bool updateOrder(int id, const Order& updatedOrder);

    /**
     * @brief Видаляє замовлення з бази за його ID.
     * @param id Ідентифікатор замовлення для видалення.
     * @return true, якщо видалено успішно, false - якщо ID не знайдено.
     */
    bool deleteOrder(int id);

    /**
     * @brief Виводить підсумкову статистику (кількість, загальна сума тощо).
     */
    void displaySummary() const;
};

#endif // DATABASE_H