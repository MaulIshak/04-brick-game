/**
 Utilitas Array list.

 Karena sifat dari bahasa C tidak memiliki generic, berbagai fungsionalitas di bahasa lain harus di emulasi menggunakan makro.
 
 Semua argument dari fungsi (sebenarnya makro) pertama adalah sebuah struct dengan properti sebagai berikut (kita memanggilnya protokol) :
 ```c
 struct ArrayList {
    int cap;
    int len;
    T *items;
 }
 ```
 dimana T adalah tipe data bebas.
 
 konsep ArrayList mirip dengan konsep slice di golang dan vector di rust.
 Lihat video untuk lebih jelas: https://youtu.be/xFMXIgvlgcY?si=4STwkWfJiA1WGi7r&t=588

 Arraylist mewajibkan penggunaan malloc untuk mendapatkan memori pada properti items. Cara lain tidak akan di support.
 */

#pragma once
#include <stdlib.h>
#ifndef ARRAY_LIST_H

/*
Append sebuah item kepada sebuah array list

Jika kapasitas sudah penuh, memori akan di realokasi dengan jumlah 2x dari kapasitas awal
*/
#define da_append(da, item) \
do { \
    if((da)->len >= (da)->cap) {\
        (da)->cap = (da)->cap * 2;\
        (da)->items = realloc((da)->items, sizeof(*(da)->items) * (da)->cap); \
    }\
    (da)->items[(da)->len++] = (item); \
} while (0) \


#endif // ARRAY_LIST_H