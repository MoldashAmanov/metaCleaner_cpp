#include "MetadataDeleter.h"
#include <exiv2/exiv2.hpp>
#include <iostream>
#include <algorithm>


MetadataDeleter::MetadataDeleter(const string& path) : ImageMetadata(path)
{
    if (!isSupportedFormat())
    {
        throw invalid_argument("Неподдерживаемый формат файла");
    }
}

bool MetadataDeleter::isSupportedFormat() const
{
    string path = getFilePath();
    string ext = path.substr(path.find_last_of("."));
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    return (ext == ".jpg" || ext == ".jpeg" || ext == ".tiff" || ext == ".tif");
}

bool MetadataDeleter::loadImage()
{
    try 
    {
        image = Exiv2::ImageFactory::open(getFilePath());
        image->readMetadata();
        return true;
    } 
    catch (Exiv2::Error& e)
    {
        cerr << "Ошибка загрузки: " << e.what() << "\n";
        return false;
    }
}

// Удаляем конкретный тег
bool MetadataDeleter::deleteExifTag(const string& key)
{
    if (!loadImage())
    {
        return false;
    }
    
    try 
    {
        Exiv2::ExifData& exifData = image->exifData();

        // Находим и удаляем тег
        auto pos = exifData.findKey(Exiv2::ExifKey(key));
        if (pos != exifData.end())
        {
            exifData.erase(pos);
            cout << "Удален тег: " << key << "\n";
            return true;
        }

        cout << "Тег не найден: " << key << "\n";
        return false;
    }
    catch (Exiv2::Error& e)
    {
        cerr << "Ошибка удаления: " << e.what() << "\n";
        return false;
    }
}

// Удаляем все EXIF данные
bool MetadataDeleter::deleteAllExif()
{
    if (!loadImage())
    {
        return false;
    }

    try 
    {
        Exiv2::ExifData& exifData = image->exifData();
        exifData.clear();
        cout << "Все EXIF данные удалены\n";
        return true;
    }
    catch (Exiv2::Error& e)
    {
        cerr << "Ошибка: " << e.what() << "\n";
        return false;
    }
}

// Удаляем все IPTC данные
bool MetadataDeleter::deleteAllIptc()
{
    if (!loadImage())
    {
        return false;
    }

    try 
    {
        Exiv2::IptcData& iptcData = image->iptcData();
        iptcData.clear();
        cout << "Все IPTC данные удалены\n";
        return true;
    }
    catch (Exiv2::Error& e)
    {
        cerr << "Ошибка: " << e.what() << "\n";
        return false;
    }
}

// Удаляем все метаданные
bool MetadataDeleter::deleteAllMetadata()
{
    if (!loadImage())
    {
        return false;
    }

    try 
    {
        // Очищаем все типы метаданных
        image->exifData().clear();
        image->iptcData().clear();
        image->xmpData().clear();

        cout << "Все метаданные удалены\n";
        return true;
    }
    catch (Exiv2::Error& e)
    {
        cerr << "Ошибка: " << e.what() << "\n";
        return false;
    }
}

// Удаляем GPS данные
bool MetadataDeleter::deleteGPSData()
{
    if (!loadImage())
    {
        return false;
    }

    try 
    {
        Exiv2::ExifData& exifData = image->exifData();

        // Список GPS тегов для удаления
        vector<string> gpsTags = {
            "Exif.GPSInfo.GPSLatitude",
            "Exif.GPSInfo.GPSLongitude",
            "Exif.GPSInfo.GPSAltitude",
            "Exif.GPSInfo.GPSTimeStamp",
            "Exif.GPSInfo.GPSDateStamp"
        };

        int deletedCount = 0;
        for (const auto& tag : gpsTags)
        {
            auto pos = exifData.findKey(Exiv2::ExifKey(tag));
            if (pos != exifData.end())
            {
                exifData.erase(pos);
                deletedCount++;
            }
        }

        cout << "Удалено GPS тегов: " << deletedCount << "\n";
        return true;
    }
    catch (Exiv2::Error& e)
    {
        cerr << "Ошибка: " << e.what() << "\n";
        return false;
    }
}

bool MetadataDeleter::saveChanges()
{
    if (!image)
    {
        cerr << "Изображение не загружено\n";
        return false;
    }

    try 
    {
        image->writeMetadata();
        cout << "Изменения сохранены\n";
        return true;
    }
    catch (Exiv2::Error& e)
    {
        cerr << "Ошибка сохранения: " << e.what() << "\n";
        return false;
    }
}