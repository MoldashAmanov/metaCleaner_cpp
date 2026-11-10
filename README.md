# metaCleaner_cpp

Приложение на C++ для работы с метаданными изображений (EXIF, IPTC, XMP).

## Возможности

- ✅ Чтение всех метаданных изображения
- ✅ Редактирование даты, описания, автора
- ✅ Удаление отдельных тегов или всех метаданных
- ✅ Удаление GPS данных для приватности
- ✅ Поддержка JPEG и TIFF форматов

## Технологии

- C++17
- Exiv2 library
- CMake
- STL (std::filesystem, std::map, std::unique_ptr)

## Архитектура

Проект использует принципы ООП:
- **ImageMetadata** — базовый класс
- **MetadataReader** — чтение метаданных
- **MetadataEditor** — редактирование
- **MetadataDeleter** — удаление

## Сборка

```bash
mkdir build && cd build
cmake ..
make
./metadata_manager
