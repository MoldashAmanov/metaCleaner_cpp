#include "ImageMetadata.h"
#include <filesystem>
#include <stdexcept>

// std::filesystem библиотека для работы с файловой системой
namespace fs = std::filesystem;

// Конструктор: сохраняем путь к файлу
ImageMetadata::ImageMetadata(const string& path) : filePath(path)
{
    // Проверяем, что файл существует при создании объекта
    if (!fileExists())
    {
        throw invalid_argument("Файл не найден: " + path);
    }
}

// Возвращаем путь к файлу
const string& ImageMetadata::getFilePath() const
{
    return filePath;
}

// Проверяем существование файла с помощью filesystem
bool ImageMetadata::fileExists() const
{
    return fs::exists(filePath) && fs::is_regular_file(filePath);
}

// По умолчанию считаем формат неподдерживаемым
// Наследники переопределят это метод
bool ImageMetadata::isSupportedFormat() const
{
    return false;
}