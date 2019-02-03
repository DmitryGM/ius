
#include <aduc812.h>
#include <datatypes.h>

//////////////////////// SetVector //////////////////////////
// Функция, устанавливающая вектор прерывания в
// пользовательской таблице прерываний.
// Вход:
// Vector – адрес обработчика прерывания,
// Address – вектор пользовательской таблицы прерываний.
// Выход: нет.
// Результат: нет.
//////////////////////////////////////////////////////////////

void SetVector(unsigned char __xdata * Address, void * Vector)
{
    unsigned char __xdata * TmpVector;
    *Address = 0x02;
    // address to jump
    TmpVector = (unsigned char __xdata *) (Address + 1); // make TmpVector be pointing to next byte after written command 'ljmp'
    *TmpVector = (unsigned char) ((unsigned short)Vector >> 8); // this byte now contains 8 highest bits of ISR address
    ++TmpVector; // move to next byte
    *TmpVector = (unsigned char) Vector; // write next 8 bits of address
}