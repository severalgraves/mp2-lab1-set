// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <sstream>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength())
{

}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (BitField.GetBit(Elem) == 0)
        return 0;
    else return 1;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw ("Элемент находится за пределами набора");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem > 0 || Elem < MaxPower)
    {
        BitField.ClrBit(Elem);
    }
    else
    {
        throw std::out_of_range("Element out of range!");
    }
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s)
    {
        BitField = s.BitField;
        MaxPower = s.MaxPower;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (BitField == s.BitField)
        return 1;
    else return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    if (BitField != s.BitField)
        return 1;
    else return 0;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet U(BitField | s.BitField);
    return U;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet U(*this);
    U.InsElem(Elem);
    return U;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet U(*this);
    U.DelElem(Elem);
    return U;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet U(BitField & s.BitField);
    return U;
}

TSet TSet::operator~(void) // дополнение
{
    return TSet(~BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
