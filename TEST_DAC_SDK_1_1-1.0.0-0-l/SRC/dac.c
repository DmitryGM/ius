/****************************************************************************

dac.c - простейший драйвер ЦАП
для учебного стенда SDK-1.1

(C) dac.c, Ключев А.О. 2007 г.

Это свободная программа; вы можете повторно распространять ее и/или
модифицировать ее в соответствии с Универсальной Общественной
Лицензией GNU, опубликованной Фондом Свободного ПО; либо версии 2,
либо (по вашему выбору) любой более поздней версии.

Эта программа распространяется в надежде, что она будет полезной,
но БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ; даже без подразумеваемых гарантий
КОММЕРЧЕСКОЙ ЦЕННОСТИ или ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ. Для
получения подробных сведений смотрите Универсальную Общественную
Лицензию GNU.

Вы должны были получить копию Универсальной Общественной Лицензии
GNU вместе с этой программой; если нет, напишите по адресу: Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA

----------------------------------------------------------------------------
Россия, Санкт-Петербург, кафедра вычислительной техники СПбГУИТМО
e-mail: kluchev@d1.ifmo.ru

****************************************************************************/
#include <aduc812.h>
#include <dac.h>

/**----------------------------------------------------------------------------
init_dac
-------------------------------------------------------------------------------
Инициализация ЦАП.

Вход: mode - режим (_8BIT/_12BIT)
Выход: нет
Результат: нет
Описание: Прекращает работу ЦАП, устанавливает режим (8-битный/12-битный),
обнуляет регистры данных DACxH,DACxL. Затем включает оба канала.

-----------------------------------------------------------------------------*/
void init_dac(bit mode)
{
    unsigned char wait;

    DACCON &= ~0x03; //Выключить оба канала

    if( mode == _8BIT )
    {
        DACCON |= 0x80;
    }
    else
    {
        DACCON &= ~0x80;
    }

    DAC0H = DAC0L = 0;
    DAC1H = DAC1L = 0;

    DACCON |= 0x60; //Диапазон напряжения 0..Vdd (0..5В)
    DACCON |= 0x1C; //Напряжение на выходе изменяется при записи в регистры
//DACxL

    DACCON |= 0x03; //Включить оба канала

    for(wait = 0; wait < 30; ++wait); //Задержка на время > 65мс (см.
//описание функционирования ЦАП и АЦП
//в документации на ADuC812).

}

/**----------------------------------------------------------------------------
switch_dac
-------------------------------------------------------------------------------
Включение/выключение канала ЦАП.

Вход: channel - номер канала (DAC0/DAC1)
state - включить (ON) или выключить (OFF)
Выход: нет
Результат: нет
Описание:

-----------------------------------------------------------------------------*/
void switch_dac( bit channel, bit state )
{
    unsigned char mask;

    if( channel == DAC0 )
    {
        mask = 0x01;
    }
    else
    {
        mask = 0x02;
    }

    if( state == ON )
    {
        DACCON |= mask;

    }
    else
    {
        DACCON &= ~mask;
    }
}

/**----------------------------------------------------------------------------
set_voltage
-------------------------------------------------------------------------------
Подача нужного напряжения на один из каналов ЦАП.

Вход: v - значение напряжения в вольтах
channel - номер канала (DAC0/DAC1)
Выход: нет
Результат: нет
Описание: Подает напряжение на выбранный канал ЦАП. Предполагается, что канал
включен.

-----------------------------------------------------------------------------*/
#define VDD 5.0

void set_voltage( float v, bit channel )
{
    unsigned short max_val;
    unsigned short val;

    if( DACCON & 0x80 ) //8-битный режим ЦАП
    {
        max_val = 0xFF;
    }
    else //12-битный режим ЦАП
    {
        max_val = 0xFFF;
    }

    val = ( unsigned short )( v * max_val / VDD ); //Максимально возможное
//напряжение - VDD
// val = 0x99;
    val &= max_val; //Значение не больше максимально возможного

    if( channel == DAC0 )
    {
        DAC0H = val >> 8;
        DAC0L = val;
    }
    else
    {
        DAC1H = val >> 8;
        DAC1L = val;
    }
}