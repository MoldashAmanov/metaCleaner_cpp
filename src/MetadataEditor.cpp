#include "MetadataEditor.h"
#include <exiv2/exiv2.hpp>
#include <iostream>
#include <algorithm>

MetadataEditor::MetadataEditor(const string& path) : ImageMetadata(path)
{
    if (!isSupportedFormat())
    {
        throw invalid_argument("Неподдерживаемый формат файла");
    }
}

bool MetadataEditor::isSupportedFormat() const
{
    string path = getFilePath();
    string ext = path.substr(path.find_last_of("."));
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    return (ext == ".jpt" || ext == ".jpeg" || ext == ".tiff" || ext == ".tif");
}

// Загружаем изображения для редактирования
bool MetadataEditor::loadImage()
{
    try 
    {
        image = Exiv2::ImageFactory::open(getFilePath());
        image->readMetadata();
        return true;
    }
    catch (Exiv2::Error& e)
    {
        cerr << "Ошибка загрузки изображения: " << e.what() << "\n";
        return false;
    }
}

// Устанавливаем произвольный EXIF тег
bool MetadataEditor::setExifTag(const string& key, const string& value)
{
    if (!loadImage())
    {
        return false;
    }
    
    try
    {
        Exiv2::ExifData& exifData = image->exifData();

        // Устанавливаем значение тега
        exifData[key] = value;

        cout << "Установлен тег " << key << " = " << value << "\n";
        return true;
    }
    catch (Exiv2::Error& e)
    {
        cerr << "Ошибка установки тега: " << e.what() << "\n";
        return false;
    }
}

// Устанавливаем дату и время
bool MetadataEditor::setDateTime(const string& dateTime)
{
    // Формат даты для EXIF: "YYYY:MM:DD HH:MM:SS"
    return setExifTag("Exif.Image.DateTime", dateTime);
}

// Устанавливаем описание
bool MetadataEditor::setDescription(const string& description)
{
    // Описание хранится в IPTC данных
    if (!loadImage())
    {
        return false;
    }

    try 
    {
        Exiv2::IptcData& iptcData = image->iptcData();

        // Создаем или обновляем тег описания
        Exiv2::IptcKey key("Iptc.Application2.Caption");
        // ...create создает значение нужного типа
        Exiv2::Value::UniquePtr value = Exiv2::Value::create(Exiv2::string);
        value->read(description);

        iptcData.add(key, value.get());

        cout << "Установлено описание: " << description << "\n";
        return true;
    }
    catch (Exiv2::Error& e)
    {
        cerr << "Ошибка установки описания: " << e.what() << "\n";
        return false;
    }
}

// Устанавливаем информацию об авторе
bool MetadataEditor::setAuthor(const string& author)
{
    return setExifTag("Exif.Image.Artist", author);
}

// Сохраняем все изменения
bool MetadataEditor::saveChanges()
{
    if (!image)
    {
        cerr << "Изображение не загружено\n";
        return false;
    }

    try 
    {
        // Записываем метаданные обратно в файл
        image->writeMetadata();
        cout << "Изменения сохранены успешно\n";
        return true;
    }
    catch (Exiv2::Error& e)
    {
        cerr << "Ошибка сохранения: " << e.what() << "\n";
        return false;
    }
}