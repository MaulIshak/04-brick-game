#ifndef STRING_H
#define STRING_H

typedef struct String {
    int len, cap;
    char *buff;
} String;

void String_Push(String *str);
void String_ToCString(String *str);


#endif // STRING_H