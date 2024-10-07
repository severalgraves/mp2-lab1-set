// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <algorithm>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw ("negative lenght");
    BitLen = len;
    if (len % (sizeof(TELEM)*8) == 0)
    {
        MemLen = len / (sizeof(TELEM) * 8);
    }
    else
    {
        MemLen = len / (sizeof(TELEM) * 8) + 1;
    }
    pMem = new TELEM[MemLen]{ 0 };
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    if (pMem != nullptr) {
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    else { 
        throw bad_alloc();
    }
}

TBitField::~TBitField()
{
    BitLen = 0;
    MemLen = 0;
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((n < 0) || (n >= BitLen))
        throw ("Error input");
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n < 0) || (n >= BitLen))
        throw ("Error input");
    const int j = n % (sizeof(TELEM) * 8);
    const TELEM mask = (1 << j);
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n >= BitLen))
    {
        throw ("Error input");
    }
    const int i = GetMemIndex(n);
    const int p = GetMemMask(n);
    pMem[i] = (pMem[i] | p);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen))
    {
        throw ("Error input");
    }
    const int i = GetMemIndex(n);
    const int p = GetMemMask(n);
    pMem[i] = (pMem[i] & (~p));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if ((n < 0) || (n >= BitLen))
  {
      throw ("Error input");
  }

  const int i = GetMemIndex(n);
  const int p = GetMemMask(n);
  return (pMem[i] & p) ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf)
    {
        if (bf.MemLen != MemLen) 
        {
            MemLen = bf.MemLen;
            delete[] pMem;
            pMem = new TELEM[MemLen];
            if (pMem == nullptr) 
            {
                throw bad_alloc();
            }
        }
        BitLen = bf.BitLen;
        for (int i = 0; i < MemLen; i++)
        {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    int tmp = 1;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            tmp = 0;
            break;
        }
    }
    return tmp;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxLen = max(BitLen, bf.BitLen);
    TBitField result(maxLen);

    for (int i = 0; i < result.MemLen; i++)
    {
        TELEM a = (i < MemLen) ? pMem[i] : 0;
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;
        result.pMem[i] = a | b;
    }

    return result;
    
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField A(max(BitLen, bf.BitLen));
    for (int i = 0; i < min(MemLen, bf.MemLen); i++)
        A.pMem[i] = pMem[i] & bf.pMem[i];
    return A;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(*this);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~(pMem[i]);
    TELEM d = ((TELEM)1 << (BitLen % (sizeof(TELEM) * 8))) - (TELEM)1;
    res.pMem[MemLen - 1] &= d;
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string in;
    istr >> in;
    if (in.size() > bf.BitLen)
        throw out_of_range("out of range");
    else
        for (int i = 0; i < in.size(); i++)
            if (in[i] != '0')
                bf.SetBit(i);
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        cout << bf.GetBit(i);
    return ostr;
}
