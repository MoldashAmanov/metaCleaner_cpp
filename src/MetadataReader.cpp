#include "MetadataReader.h"
#include <exiv2/exiv2.hpp>
#include <iostream>
#include <iomanip>
#include <algorithm>

MetadataReader::MetadataReader(const string& path) : ImageMetadata(path)
{
    // Дополнительная проверка формата при создании
    if (!isSupportedFormat())
    {
        throw invalid_argument(
            "Неподдерживаемый формат файла. Используйте JPEG или TIFF"
        );
    }
}

// Проверяем расширение файла
bool MetadataReader::isSupportedFormat() const
{
    string path = getFilePath();
    string ext = path.substr(path.find_last_of("."));

    // Приводим к нижнему регистру для сравнения
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    return (ext == ".jpg" || ext == ".jpeg" || ext == ".tiff" || ext == ".tif");
}

// Читаем все метаданные
map<string, string> MetadataReader::readAllMetadata()
{
    return extractExifData();
}

// Читаем конкретное поле
string MetadataReader::readMetadata(const string& key)
{
    auto allData = readAllMetadata();

    // Ищем ключ в map
    auto it = allData.find(key);
    if (it != allData.end())
    {
        return it->second;
    }

    return "Не найдено";
}

// Извлекаем EXIF данные с помощью Exiv2
map<string, string> MetadataReader::extractExifData()
{
    map<string, string> metadata;

    try
    {
        // Открываем изображение с помощью Exiv2
        auto image = Exiv2::ImageFactory::open(getFilePath());

        // Читаем метаданные из файла и загружаем в память
        image->readMetadata();

        // Получаем доступ к EXIF данным и возвращаем ссылку на EXIF данные (exifData())
        Exiv2::ExifData& exifData = image->exifData();

        // Проверяем, есть ли EXIF данные
        if (exifData.empty())
        {
        metadata["Error"] = "EXIF данные не найдены";
        return metadata;
        }

        // Итерируемся по всем EXIF тегам
        for (const auto& exifItem : exifData)
        {
        // Ключ тега (пример: "Exif.Image.DateTime")
        string key = exifItem.key();

        // Значение тега в строковом формате
        string value = exifItem.value().toString();
        metadata[key] = value;
        }

        // Читаем IPTC данные (авторские права, описание)
        Exiv2::IptcData& iptcData = image->iptcData();
        for (const auto& iptcItem : iptcData)
        {
            string key = "IPTC." + iptcItem.key();
            metadata[key] = iptcItem.value().toString();
        }
    }
    catch(Exiv2::Error& e)
    {
        metadata["Error"] = string("Ошибка Exiv2: ") + e.what();
    }
    return metadata;
}

// Вывод метаданных в консоль
void MetadataReader::printMetadata()
{
    auto metadata = readAllMetadata();
    cout << "\n=== Метаданные файла: " << getFilePath() << "===\n\n";

    if (metadata.empty())
    {
        cout << "Метаданные не найдены\n";
        return;
    }

    // Находим максимальную длину ключа для выравнивания
    size_t maxKeyLength = 0;
    for (const auto& pair : metadata)
    {
        maxKeyLength = max(maxKeyLength, pair.first.length());
    }

    // Выводим каждую пару ключ-значение
    for (const auto& pair : metadata)
    {
        cout << left << setw(maxKeyLength + 2) << pair.first << ": " << pair.second << "\n";
    }

    cout << "\nВсего полей: " << metadata.size() << "\n";
}