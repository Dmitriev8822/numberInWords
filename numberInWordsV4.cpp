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

constexpr int MIN_INPUT_VALUE = -1; // минимальное значение для ввода
constexpr int MAX_INPUT_VALUE = 999999999; // максимальное значение для ввода

std::string numberToWords(unsigned int number);
static int countDigits(int number);
int intPow(int base, int exp);
static std::string rankToText(int rankValue, int rank);
void clearInputBuffer();
int getNumber();
std::string getValueOfRank(int num, int rank);

const int GETTERRANKVALUE = intPow(10, 3);

int main()
{
    setlocale(LC_ALL, "Russian");

    int number{ 0 };
    std::cout << "Программа \"Число прописью\".\nДля выхода из программы введите -1.\n";

    while (true) {
        std::cout << "Введите число (-1 для выхода): ";
        int number{ 0 };
        number = getNumber();

        if (number == -1)
            break;
        else if (number == -2)
            continue;
        else
            std::cout << numberToWords(number) << "\n"; 
    }

    std::cout << "Программа завершена.\n";

    return 0;
}

int getNumber() {
    long long number;
    if (!(std::cin >> number)) {
        clearInputBuffer();
        std::cout << "Некорректный ввод. Ожидается целое число.\n";
        return -2;
    }

    if (number >= MIN_INPUT_VALUE && number <= MAX_INPUT_VALUE) {
        return static_cast<int>(number);
    }

    std::cout << "Число выходит за диапазон допустимых значений.\n";
    return -2;
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

    for (int posInRank{ 2 }; rankValue; posInRank--)
    {
        digit = rankValue / intPow(10, posInRank);
        const std::string* arrUse = arrWords[rank][posInRank];
        if ((posInRank == 1) && (digit == 1))
        {
            arrUse = arrTensDialect;
            digit = rankValue % 10;
            rankValue = 0;
        }
        result += arrUse[digit];
        rankValue %= intPow(10, posInRank);
    }
    
    return result;
}

std::string getValueOfRank(int num, int rank)
{
    if (num / 10 == 1)
        return arrValuesOfRanks[rank][0];

    return arrValuesOfRanks[rank][num % 10];
}

// Функция преобразования целочисленного значения в пропись
// На вход функция принимает значение в диапазоне от 0 до 1,000,000,000
std::string numberToWords(unsigned int number)
{
    if (!number)
        return "ноль рублей";

    std::string result{ "" };
    std::string currencyValue = addСurrencyValue(number);
    
    for (int rank{ (countDigits(number) - 1) / 3 }; number; rank--)
    {
        int rankValue = number / intPow(GETTERRANKVALUE, rank);
        number %= intPow(GETTERRANKVALUE, rank);
        result += rankToText(rankValue, rank) + getValueOfRank(rankValue % intPow(10, 2), rank);
    }
    result += currencyValue;
    return result;
}