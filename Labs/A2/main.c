#include <stdint.h>
#include <stdio.h>
#include <locale.h>


void asm_calculate(int8_t x, int16_t y, int16_t z) {
    int8_t v;
    __asm {
        // Вычисление числителя
        mov al, z    // ax := x
        cbw          // Расширить al до 16-ти битного регистра
        add ax 2     // ax := ax + 2
        imul x       // ax = x * ax
        neg ax       // ax := -ax
        add ax, 3    // ax := ax + 3
        mov bx, ax   // bx := ax
        // Вычисление знаменателя
        xor ax, ax   // ax := XOR(ax, ax)
        mov al, y    // al := y
        cbw          // Расширить al до 16-ти битного регистра
        sub ax, 1    // ax := ax - 1
        // Деление
        xchng ax, bx // ax <=> bx
        idiv bx      // ax := ax / bx (Знаковое деление)
        add ax, 5    // ax := ax  + 5
        mov v, ax    // v := ax
    };
    return v;
}


int8_t c_calculate(int8_t x, int16_t y, int16_t z) {
    return 5 + ( 3 - (z + 2) * x ) / (y - 1);
}


void run_test(int8_t x, int16_t y, int16_t z, int8_t v) {
    int8_t v_tmp;

    printf("Ввод:\n\tx:= %hd\ty:= %hd\tx:=%hd\n", x, y, z);

    printf("Правильный результат:\n\tv:= %hd\n", v);

    v_tmp = c_calculate(x, y, z)
    printf("Вычисления в Си:\n\tДесяитичная: %hd\tШестнадцатеричная: 0x%04x\n\n", v_tmp, v_tmp);

    v_tmp = asm_calculate(x, y, z)
    printf("Вычисления в Ассемблере:\n\tДесятичная: %hd\tШестнадцатеричная: 0x%04x\n\n", v_tmp, v_tmp);
}


int main() {
    int8_t x, int16_t y, int16_t z;
    char ans;

    setlocale(LC_ALL, "Russian");

    printf("Тест #1\n");
    run_test(2, 4, -8, 0xA);

    printf("Тест #2\n");
    run_test(0x7BE, -0x7E, 0x7E, 0x7D2);

    printf("Все тесты пройдены!\n");

    printf("Продолжить ручное тестирование? (д/н):\t");
    scanf("%s", &ans);

    while(ans == 'д') {
        printf("\nВведите x, y, z через пробел\n");
        scanf("%hhd %hhd %hhd", &x, &y, &z);
        run_test(x, y, z);

        printf("Продолжить тестирование? (д/н):\t ");
        scanf("%s", &ans);
    }

    return 0;
}