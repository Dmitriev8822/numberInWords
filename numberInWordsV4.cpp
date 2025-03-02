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
    {"", "", ""},
    {"тысяча ", "тысячи ", "тысяч "},
    {"миллион ", "миллиона ", "миллионов "}
};

static const int arrIndexValueOfRanks[10]{ 2, 0, 1, 1, 1, 2, 2, 2, 2, 2 };

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
int getNum(unsigned int& number);

int main()
{
    setlocale(LC_ALL, "Russian");

    int number{ 0 };
    std::cout << "Программа \"Число прописью\".\nДля выхода из программы введите -1.\n";

    while (number != -1)
    {
        std::cout << "Введите число: ";
        std::cin >> number;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Были получены некорректные данные." << '\n';
            continue;
        }

        if (number >= MIN_INPUT_VALUE && number <= MAX_INPUT_VALUE)
            std::cout << "Число прописью: " << numberToWords(number) << '\n';
        else
            std::cout << "Число выходит за диапазон допустимых значений для ввода." << '\n';
        
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
        result += arrValuesOfRanks[rank][2];
        num /= 100;
        i = 0;
    }
    else
    {
        result += arrValuesOfRanks[rank][arrIndexValueOfRanks[num % 10]];
        i = 2;
    }

    for (; i >= 0; --i) // (; num; --i)
    {
        result = arrWords[rank][i][num % 10] + result;
        num /= 10;
    }

    return result;
}

int getNum(unsigned int& number)
{
    int result = 0;
    int multiplier = 1;

    for (int i = 0; i < 3; ++i) {
        int digit = number % 10;
        result += digit * multiplier;
        number /= 10;
        multiplier *= 10;
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
    int num{ 0 };
    num = getNum(number);
    
    result += addСurrencyValue(num);
    for (int rank{0}; rank < 3; ++rank)
    {
        result = rankToText(num, rank) + result;
        num = getNum(number);
    } 

    return result;
}