#include "MetadataDeleter.h"
#include "MetadataEditor.h"
#include "MetadataReader.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Очистка буфера
void clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Меню чтения метаданных
void readMenu()
{
    string filePath;
    cout << "/\nВведите путь к изображению: ";
    getline(cin, filePath);

    try 
    {
        MetadataReader reader(filePath);

        cout << "\nЧто вы хотите сделать?\n";
        cout << "1. Показать все метаданные\n";
        cout << "2. Найти конкретный тег\n";
        cout << "Ваш выбор: ";

        int choice;
        cin >> choice;
        clearInputBuffer();

        if (choice == 1)
        {
            reader.printMetadata();
        }
        else if (choice == 2)
        {
            cout << "Введите имя тега (например, Exif.Image.DateTime): ";
            string tagName;
            getline(cin, tagName);

            string value = reader.readMetadata(tagName);
            cout << tagName << " = " << value << "\n";
        }
    }
    catch (exception& e)
    {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

// Меню редактирования метаданных
void editMenu()
{
    string filePath;
    cout << "\nВведите путь к изображению: ";
    getline(cin, filePath);

    try 
    {
        MetadataEditor editor(filePath);

        cout << "\nЧто вы хотете изменить?\n";
        cout << "1. Дату и время\n";
        cout << "2. Описание\n";
        cout << "3. Автора\n";
        cout << "4. Произовольный тег\n";
        cout << "Ваш выбор: ";
        
        int choice;
        cin  >> choice;
        clearInputBuffer();

        bool success = false;

        switch (choice)
        {
            case 1:
            {
                cout << "Введите дату (формат YYYY:MM:DD HH:MM:SS): ";
                string dateTime;
                getline(cin, dateTime);
                success = editor.setDateTime(dateTime);
                break;
            }
            case 2:
            {
                cout << "Введите описание: ";
                string description;
                getline(cin, description);
                success = editor.setDescription(description);
                break;
            }
            case 3:
            {
                cout << "Введите автора: ";
                string author;
                getline(cin, author);
                success = editor.setAuthor(author);
                break;
            }
            case 4:
            {
                cout << "Введите имя тега: ";
                string key;
                getline(cin, key);
                cout << "Введите значение: ";
                string value;
                getline(cin, value);
                success = editor.setExifTag(key, value);
                break;
            }
            default:
            {
                cout << "Неверный выбор\n";
                return;
            }
        }

        if (success)
        {
            cout << "\nСохранить изменения? (y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y')
            {
                editor.saveChanges();
            }
        }
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

// Меню удаления метаданных
void deleteMenu()
{
    string filePath;
    cout << "\nВведите путь к изображению: ";
    getline(cin, filePath);

    try 
    {
        MetadataDeleter deleter(filePath);
        cout << "\nЧто вы хотите удалить?\n";
        cout << "1. Конкретный тег\n";
        cout << "2. Все EXIF данные\n";
        cout << "3. Все IPTC данные\n";
        cout << "4. Все метаданные\n";
        cout << "5. GPS данные\n";
        cout << "Ваш выбор: ";
        
        int choice;
        cin >> choice;
        clearInputBuffer();

        bool success = false;

        switch (choice)
        {
            case 1:
            {
                cout << "Введите имя тега для удаления: ";
                string tagName;
                getline(cin, tagName);
                success = deleter.deleteExifTag(tagName);
                break;
            }
            case 2:
            {
                success = deleter.deleteAllExif();
                break;
            }
            case 3:
            {
                success = deleter.deleteAllIptc();
                break;
            }
            case 4:
            {
                cout << "Внимание: это удалит ВСЕ метаданные!\n";
                cout << "Продолжить? (y/n): ";
                char confirm;
                cin >> confirm;
                clearInputBuffer();
                if (confirm == 'y' || confirm == 'Y')
                {
                    success = deleter.deleteAllMetadata();
                }
                break;
            }
            case 5:
            {
                success = deleter.deleteGPSData();
                break;
            }
            default:
            {
                cout << "Неверный выбор\n";
                return;
            }
        }

        if (success)
        {
            cout << "Сохранить изменения? (y/n): ";
            char confirm;
            cin >> confirm;
            clearInputBuffer();

            if (confirm == 'y' || confirm == 'Y')
            {
                deleter.saveChanges();
            }
        }
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

// Главное меню
void mainMenu()
{
    while (true)
    {
        cout << "\n=================================\n";
        cout << "  Менеджер метаданных изображений\n";
        cout << "=================================\n";
        cout << "1. Читать метаданные\n";
        cout << "2. Редактировать метаданные\n";
        cout << "3. Удалить метаданные\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";

        int choice;
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
            case 1:
            {
                readMenu();
                break;
            }
            case 2:
            {
                editMenu();
                break;
            }
            case 3:
            {
                deleteMenu();
                break;
            }
            case 0:
            {
                cout << "До свидания!\n";
                return;
            }
            default:
            {
                cout << "Неверный выбор. Попробуйте снова\n";
            }
        }
    }
}

int main()
{
    try 
    {
        mainMenu();
    }
    catch (const exception& e)
    {
        cerr << "Критическая ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
