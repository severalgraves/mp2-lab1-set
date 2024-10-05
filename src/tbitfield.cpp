// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw ("negative lenght");
    BitLen = len;
    if (len % 32 == 0)
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
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    BitLen = 0;
    MemLen = 0;
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((n < 0) || (n > BitLen))
        throw ("Error input");
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n < 0) || (n > BitLen))
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
    if ((n < 0) || (n > BitLen))
    {
        throw ("Error input");
    }
    const int i = GetMemIndex(n);
    const int p = GetMemMask(n);
    pMem[i] = (pMem[i] | p);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n > BitLen))
    {
        throw ("Error input");
    }
    const int i = GetMemIndex(n);
    const int p = GetMemMask(n);
    pMem[i] = (pMem[i] & (~p));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if ((n < 0) || (n > BitLen))
  {
      throw ("Error input");
  }

  const int i = GetMemIndex(n);
  const int p = GetMemMask(n);
  return (pMem[i] & p);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen == bf.BitLen) {
      for (int i = 0; i < MemLen; i++)
          if (pMem[i] != pMem[i])
          {
              return 0;
          };
      return 1;
  }
  else return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return ~(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int i;
    int len = BitLen;
    if (len < bf.BitLen)
        len = bf.BitLen;

    TBitField tbf(len);
    for (i = 0; i < MemLen; i++)
    {
        tbf.pMem[i] = pMem[i];
    }
    for (i = 0; i < bf.MemLen; i++)
    {
        tbf.pMem[i] |= bf.pMem[i];
    }

    return tbf;
    
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
    int tmp = 0;

    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> tmp;
        bf.SetBit(tmp);
    }

    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        cout << bf.GetBit(i);
    return ostr;
}
