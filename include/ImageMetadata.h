#pragma once
#include <string>
#include <map>
#include <memory>

using namespace std;

// Базовый класс для работы с метаданными изображения
class ImageMetadata
{
    protected:
        string filePath;    // Путь к файлу
    public:
        // Конструктор принимает путь к файлу
        // explicit предотвращаем неявные преобразования типов
        explicit ImageMetadata(const string& path);

        // Виртуальный дектруктор для правильного освобождения ресурсов
        virtual ~ImageMetadata() = default;

        // Геттер для пути к файлу 
        const string& getFilePath() const;

        // Проверка существования файла
        bool fileExists() const;

        // Проверка поддержки формата
        virtual bool isSupportedFormat() const;
};