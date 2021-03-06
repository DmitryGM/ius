/******************************************************************************
(С) ООО "ЛМТ", Санкт-Петербург, Россия, 2002.
http://lmt.cs.ifmo.ru, tel.: +7-812-233-3096,
mailto: lmt@d1.ifmo.ru

Данный файл является свободно распространяемым примером на
языке Си (компилятор Keil C фирмы KEIL ELEKTRONIK GmbH) и
предназначен для иллюстрации работы с функциональным расширителем
центрального процессора стенда на базе ПЛИС Altera MAX3064 (3128).
Данный программный модуль не подвергался тщательному
тестированию и может содержать ошибки. Авторы не несут 
ответственности за потерю информации и иные возможные вредные
последствия использования данного программного модуля в составе
программных проектов.
Данный файл может подвергаться любым изменениям, но не может
распространяться в коммерческих целях и в составе коммерческих
программных продуктов.


Файл: max.c
Версия: 1.0.0
Автор: LAN
Описание: Набор демонстрационного программного обеспечения для стенда
SDK-1.1. Работа с ПЛИС MAX3064 (3128).

Изменения:
-------------------------------------------------------------------------------
N Дата Версия Автор Описание 
-------------------------------------------------------------------------------
1 10.04.02 1.0.0 LAN Создан
******************************************************************************/

#include <aduc812.h>

#define MAXBASE 0x8 //Номер страницы внешней памяти (xdata), куда отображаются
//регистры расширителя.

/**----------------------------------------------------------------------------
WriteMax
-------------------------------------------------------------------------------
Запись в нужный регистр ПЛИС ALTERA MAX3064(3128)

Вход: regnum - адрес (номер) регистра
val - записываемое значение
Выход: нет
Результат: нет
Описание: Производится запись в регистр (порт) ПЛИС ALTERA MAX3064(3128) 
путем переключения адресуемой страницы памяти на страницу, где 
расположены (куда отображаются) порты ввода-вывода ПЛИС.

Пример:
-----------------------------------------------------------------------------*/
void WriteMax(unsigned char __xdata *regnum, unsigned char val)
{
    unsigned char oldDPP = DPP;

    DPP = MAXBASE;
    *regnum = val;
    DPP = oldDPP;
}

/**----------------------------------------------------------------------------
ReadMax
-------------------------------------------------------------------------------
Чтение из нужного регистра ПЛИС ALTERA MAX3064(3128)

Вход: regnum - адрес (номер) регистра
Выход: нет
Результат: прочитанное из регистра значение
Описание: Чтение из порта ПЛИС ALTERA MAX3064(3128) 
путем переключения адресуемой страницы памяти на страницу, где 
расположены (куда отображаются) порты ввода-вывода ПЛИС
Пример:
-----------------------------------------------------------------------------*/
unsigned char ReadMax(unsigned char __xdata *regnum)
{
    unsigned char oldDPP = DPP;
    unsigned char val = 0;

    DPP = MAXBASE;
    val = *regnum;
    DPP = oldDPP;
    return val;
}
