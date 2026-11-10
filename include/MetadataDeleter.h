#pragma once
#include "ImageMetadata.h"
#include <exiv2/exiv2.hpp>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Класс для удаления метаданных
class MetadataDeleter : public ImageMetadata
{
    public:
        explicit MetadataDeleter(const string& path);
        bool isSupportedFormat() const override;

        // Удаляем конкретный EXIF тег
        bool deleteExifTag(const string& key);

        // Удаляем все EXIF данные
        bool deleteAllExif();

        // Удаляем все IPTC данные
        bool deleteAllIptc();

        // Удаляем все метаданные (EXIF + IPTC + XMP)
        bool deleteAllMetadata();

        // Удаляем GPS данные
        bool deleteGPSData();

        // Сохраняем изменения
        bool saveChanges();

    private:
        unique_ptr<Exiv2::Image> image;
        bool loadImage();
};