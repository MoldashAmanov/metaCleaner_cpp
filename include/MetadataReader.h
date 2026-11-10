#pragma once
#include "ImageMetadata.h"
#include <map>
#include <string>

using namespace std;

// Класс для чтения метаданных
class MetadataReader : public ImageMetadata
{
    public:
        // Конструктор наследует базовый конструктор
        // explicit предотвращаем неявные преобразования типов
        explicit MetadataReader(const string& path);

        // Переопределяем проверку формата
        bool isSupportedFormat() const override;

        // Читаем метаданных в map
        map<string, string> readAllMetadata();

        // Читаем конкретное поле метаданных
        string readMetadata(const string& key);

        // Выводим метаданные в консоль
        void printMetadata();
        
    private:
        // Вспомогательный метод для извлечения EXIF данных
        map<string, string> extractExifData();
};