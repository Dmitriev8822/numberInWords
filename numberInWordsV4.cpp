#include <iostream>
#include <string>
#include <limits> // для std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#include <locale.h> // для setlocale(LC_ALL, "Russian");
#include "test.cpp"

static const std::string arrHundreds[10]{ "", "сто ", "двести ", "триста ", "четыреста ", "пятьсот ", "шестьсот ", "семьсот ", "восемьсот ", "девятьсот " };

static const std::string arrTens[10]{ "", "", "двадцать ", "тридцать ", "сорок ", "пятьдесят ", "шестьдесят ", "семьдесят ", "восемьдесят ", "девяносто " };

static const std::string arrTensDialect[10]{ "десять ", "одиннадцать ", "двенадцать ", "тринадцать ", "четырнадцать ", "пятнадцать ", "шестнадцать ", "семнадцать ", "восемнадцать ", "девятнадцать " };

static const std::string arrUnits[10]{ "", "один ", "два ", "три ", "четыре ", "пять ", "шесть ", "семь ", "восемь ", "девять " };

static const std::string arrUnitsDialect[10]{ "", "одна ", "две ", "три ", "четыре ", "пять ", "шесть ", "семь ", "восемь ", "девять " };

static const std::string arrValuesOfСurrency[10]{ "рублей", "рубль", "рубля", "рубля", "рубля", "рублей", "рублей", "рублей", "рублей", "рублей" };

static const std::string arrValuesOfRanks[3][10]
{
    {"", "", "", "", "", "", "", "", "", ""},
    {"тысяч ", "тысяча ", "тысячи ", "тысячи ", "тысячи ", "тысяч ", "тысяч ", "тысяч ", "тысяч ", "тысяч "},
    {"миллионов ", "миллион ", "миллиона ", "миллиона ", "миллиона ", "миллионов ", "миллионов ", "миллионов ", "миллионов ", "миллионов "}
};

 static const std::string* arrWords[3][3]
{
    {arrHundreds, arrTens, arrUnits},
    {arrHundreds, arrTens, arrUnitsDialect},
    {arrHundreds, arrTens, arrUnits}
};

constexpr int MAX_RANKS = 3; // размер числа
constexpr int MIN_INPUT_VALUE = 0; // минимальное значение для ввода
constexpr int MAX_INPUT_VALUE = 999999999; // максимальное значение для ввода

static std::string digitsToText(int* ptr, int index);
std::string numberToWords(unsigned int number);
int getNumber();
int reverseNumber(int number);
void testNumberToWords();
int* splitNumber(int number);

int main()
{
    // Устанавливаем локализацию на русский язык
    setlocale(LC_ALL, "Russian");

    std::cout << "Число прописью: " << numberToWords(123123) << '\n';

    int choice;
    std::cout << "Выберите один из следующих вариантов:\n1. Использовать программу;\n2. Тестировать программу." << std::endl;
    std::cin >> choice;
    
    if (choice - 1)
        testNumberToWords();

    std::cout << "Программа \"Число прописью\".\nДля выхода из программы введите -1.\n";

    // Основной цикл программы
    while (true)
    {
        int x = getNumber(); // Получаем корректное целочисленное значение

        if (x == -1) // Если введено -1, программа завершает работу
            break;

        // Преобразуем число в пропись и выводим результат
        std::cout << "Число прописью: " << numberToWords(x) << '\n';
    }

    std::cout << "Программа завершена.\n";

    return 0;
}

static std::string addСurrencyValue(int num)
{
    if (num / 10 % 10 == 1)
        return arrValuesOfСurrency[0];

    return arrValuesOfСurrency[num % 10];
}

// Функция преобразования трехзначного числа в пропись
// number — это само число, а digit указывает на разряд (0 — рубли, 1 — тысячи, 2 — миллионы)
static std::string rankToText(int num, int rank)
{
    if (num == 0)
        return "";

    std::string result = "";
    int i;

    int middleDigit = num / 10 % 10;
    if (middleDigit == 1)
    {
        result = arrTensDialect[num % 10];
        result += arrValuesOfRanks[rank][0];
        num /= 100;
        i = 0;
    }
    else
    {
        result += arrValuesOfRanks[rank][num % 10];
        i = 2;
    }

    for (; i >= 0; --i)
    {
        result = arrWords[rank][i][num % 10] + result;
        num /= 10;
    }

    return result;
}

// Функция преобразования целочисленного значения в пропись
// На вход функция принимает значение в диапазоне от 0 до 1,000,000,000
std::string numberToWords(unsigned int number)
{
    if (!number)
        return "ноль рублей";

    std::string result{ "" };
    std::string arrUse{ "" };
    
    result += addСurrencyValue(number % 100);
    for (int rank{0}; number; ++rank)
    {
        result = rankToText(number % 1000, rank) + result;
        number /= 1000; 
    } 

    return result;
}


int getNumber()
{
    int number{ 0 };

    while (true)
    {
        try
        {
            std::cout << "Введите число: ";
            std::cin >> number;

            // Проверка на диапазон значений
            if (number >= MIN_INPUT_VALUE && number <= MAX_INPUT_VALUE) // минимальное значение для ввода
            {
                // Если ввод успешен, выйти из цикла
                if (std::cin.good()) 
                {
                    break; // Ввод корректен
                }
                else 
                {
                    throw std::invalid_argument("Ошибка ввода!");
                }
            }
            else
            {
                throw std::out_of_range("Диапазон допустимых значений от 0 до 1,000,000,000.");
            }
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << e.what() << '\n';
            // Очистить оставшийся ввод
            while (std::cin.get() != '\n'); // Игнорируем оставшийся ввод
        }
        catch (const std::out_of_range& e)
        {
            std::cout << e.what() << '\n';
        }
    }

    return number;
}