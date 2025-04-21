#include <iostream>
#include <string>
#include <limits> // для std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#include <locale.h> // для setlocale(LC_ALL, "Russian");

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
    {arrUnits, arrTens, arrHundreds},
    {arrUnitsDialect, arrTens, arrHundreds},
    {arrUnits, arrTens, arrHundreds}
};

constexpr int MIN_INPUT_VALUE = 0; // минимальное значение для ввода
constexpr int MAX_INPUT_VALUE = 999999999; // максимальное значение для ввода

std::string numberToWords(unsigned int number);
static int countDigits(int number);
int intPow(int base, int exp);
static std::string rankToText(int rankValue, int rank);
void clearInputBuffer();

int main()
{
    setlocale(LC_ALL, "Russian");

    int number{ 0 };
    std::cout << "Программа \"Число прописью\".\nДля выхода из программы введите -1.\n";

    while (true) {
        std::cout << "Введите число (-1 для выхода): ";
        int number;

        try { // ввод данных вынести в отдельную функцию
            if (!(std::cin >> number)) 
            {
                throw std::runtime_error("Некорректный ввод. Ожидается целое число.");
            }

            if (number == -1) 
            {
                break;
            }

            if (number < MIN_INPUT_VALUE || number > MAX_INPUT_VALUE) 
            {
                throw std::out_of_range("Число выходит за диапазон допустимых значений.");
            }

            std::cout << "Число прописью: " << numberToWords(number) << '\n';
        }
        catch (const std::exception& e)
        {
            clearInputBuffer();
            std::cerr << "Ошибка: " << e.what() << '\n';
        }
    }

    std::cout << "Программа завершена.\n";


    return 0;
}

void clearInputBuffer() 
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int countDigits(int number)
{
    int digitCounter{ 0 };
    while (number)
    {
        number /= 10;
        digitCounter++;
    }
    
    return digitCounter;
}

static int intPow(int base, int exp)
{
    int result = 1;
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

static std::string addСurrencyValue(int number)
{
    if (number / 10 % 10 == 1)
        return arrValuesOfСurrency[0];

    return arrValuesOfСurrency[number % 10];
}

static std::string rankToText(int rankValue, int rank)
{
    std::string result = "";
    int digit{ 0 };

    for (int posInRank{ 2 - (3 - countDigits(rankValue)) }; rankValue; posInRank--) // упростить 025
    {
        digit = rankValue / intPow(10, posInRank);
        const std::string* arrUse = arrWords[rank][posInRank];
        if ((posInRank == 1) && (digit == 1)) // изменить на  posInRank == 1
        {
            result += arrTensDialect[rankValue % 10] + arrValuesOfRanks[rank][0]; // arrUse
            return result;
        }
        result += arrUse[digit];
        rankValue %= intPow(10, posInRank);
    }

    result += arrValuesOfRanks[rank][digit]; // вынести в отделную функцию
    return result;
}

// Функция преобразования целочисленного значения в пропись
// На вход функция принимает значение в диапазоне от 0 до 1,000,000,000
std::string numberToWords(unsigned int number)
{
    if (!number)
        return "ноль рублей";

    std::string result{ "" };
    std::string currencyValue = addСurrencyValue(number);
    
    int rankValue{ 0 }; // внести в цикл
    for (int rank{ (countDigits(number) - 1) / 3 }; number; rank--)
    {
        rankValue = number / intPow(intPow(10, 3), rank); // constexpr intPow(10, 3)
        number %= intPow(intPow(10, 3), rank);
        result += rankToText(rankValue, rank);
    }
    result += currencyValue;
    return result;
}