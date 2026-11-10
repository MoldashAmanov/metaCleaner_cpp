#pragma once
#include "ImageMetadata.h"
#include <exiv2/exiv2.hpp>
#include <string>
#include <memory>

using namespace std;

// Класс для редактирования метаданных
class MetadataEditor : public ImageMetadata
{
    public:
        explicit MetadataEditor(const string& path);
        bool isSupportedFormat() const override;

        // Устанавливаем значение EXIF тега
        bool setExifTag(const string& key, const string& value);

        // Устанавливаем дату съемки
        bool setDateTime(const string& dateTime);

        // Устанавливаем описание изображения
        bool setDescription(const string& description);

        // Устанавливаем информацию об авторе
        bool setAuthor(const string& author);

        // Сохраняем изменения в файл
        bool saveChanges();

    private:
        // Указатель на изображение для работы, обеспечиваем автоматическое
        // освобождение памяти
        unique_ptr<Exiv2::Image> image;

        // Загружаем изображение в память
        bool loadImage();
};