#include <stdint.h>
#include <stdio.h>
#include <locale.h>


int16_t asm_calculate(int16_t x, int8_t y, int8_t z) {
    int16_t v;

    if(y == 1) {
        return 5;
    }

    __asm {
        // Очищаем регистры
        xor ax, ax;
        xor bx, bx;

        // Вычисление числителя
        mov al, z;   // Так как z - байт, загружаем его в малый регистр
        cbw;         // Расширяем регистр до 16 бит, так как дальше будем работать со словами
        add ax, 2;   // Прибавляем к регистру 2
        neg ax;      // Инвертируем число (то есть ax := -ax)

        mov bx, x;   // Помещаем слово x в регистр bx
        imul bx;     // Умножаем регистр ax на bx. Получаем на выходе двойное слово

        add ax, 3;   // Прибавляем к ax 3
        sbb dx, 0;   // Вычитаем флаг переноса из dx, так как пара ax:dx участвует в делении
        mov bx, ax;  // Помещаем значения из ax в bx
    
        // Вычисление знаменателя
        xor ax, ax;  // Очищаем регистр ax
        mov al, y;   // Помещаем байт y в al
        cbw;         // Расширяем al до двойного слова
        sub ax, 1;   // Вычитаем из ax 1

        // Деление
        xchg ax, bx;  // Меняем местами ax и bx
        cwd;          // Расширяем до двойного слова
        idiv bx;      // Делим ax:dx на bx
        add ax, 5     // прибавляем к ax 5

        mov v, ax     // Помещаем в v ax
    };
    return v;
}


int16_t c_calculate(int16_t x, int8_t y, int8_t z) {
    int16_t num, div;

    if(y == 1) {
        return 5;
    }

    num = 3 - (z + 2) * x;
    div = y - 1;
    return 5 + num / div;
}


void run_test(int16_t x, int8_t y, int8_t z, int16_t v) {
    int8_t v_c, v_asm;

    printf("Ввод:\n\tx := %hd\n\ty := %hd\n\tz := %hd\n", x, y, z);

    printf("Правильный результат:\n\tv:= %hd\n", v);

    v_c = c_calculate(x, y, z);
    printf("Вычисления в Си:\n\tДесяитичная: %hd\n\tШестнадцатеричная: 0x%04x\n\n", v_c, v_c);

    v_asm = asm_calculate(x, y, z);
    printf("Вычисления в Ассемблере:\n\tДесятичная: %hd\n\tШестнадцатеричная: 0x%04x\n\n", v_asm, v_asm);
    if(v_asm != v) {
        printf("Тест не пройден!\n\n");
    }
    else {
        printf("Тест пройден!\n\n");
    }

}


int main() {
    int8_t y, z;
    int16_t x, v;
    char ans;

    setlocale(LC_ALL, "Russian");

    printf("Тест #1\n");
    run_test(2, 4, -8, 0xA);

    printf("Тест #2\n");
    run_test(0x7BE, -0x7E, 0x7E, 0x7D2);

    printf("Продолжить ручное тестирование? (y/n):\t");
    scanf(" %c", &ans);

    while(ans != 'n') {
        printf("\nВведите x, y, z, v через пробел\n");
        scanf("%hd %hhd %hhd %hd", &x, &y, &z, &v);
        run_test(x, y, z, v);

        printf("Продолжить тестирование? (y/n):\t ");
        scanf(" %c", &ans);
    }

    return 0;
}