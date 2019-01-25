#include <my_timer.h>
#include <vect.h>


//////////////////////// T0_ISR //////////////////////////////
// Обработчик прерывания от таймера 0.
// Вход: нет.
// Выход: нет.
// Результат: нет.
//////////////////////////////////////////////////////////////
void T0_ISR( void ) __interrupt ( 1 )
{
    WriteLED( 0x55 ); // Зажигание светодиодов (через один)
}



void init_timer()
{
    // Инициализация таймера 0

    // Задаем частоту таймера
    TH0 = 0xFF;
    TL0 = 0xF0;

    TMOD = 0x01;
    // Selected timer operation (input from internal system clock)
    // 16-Bit Timer/Counter. TH0 and TL0 are cascaded; there is no prescaler.

    TCON = 0x10; // Включаем таймер 0

    // Установка вектора в пользовательской таблице
    SetVector( 0x200B, (void *)T0_ISR );

    // Разрешение прерываний от таймера 0
    ET0 = 1; EA = 1;
}


