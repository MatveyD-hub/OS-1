//  preprocessor for language C++
// Полей-Добронравова Амелия

#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <map>
#include <string>

enum Lexeme { //препроцессорные лексемы
    CONST, NAME, IDENTIFIER, KEYWORD, OPERATOR, NUMBER, SEPARATOR
};

class Define { //хранит все пары для замены в тексте файла
    std::map<char*, char*> d;
public:
    void define_insert(char* k1, char* k2) {
        char* p1 = new char[sizeof(k1)];
        for (int j = 0; j <= sizeof(k1); j++) {
            p1[j] = k1[j];
        }
        char* p2 = new char[sizeof(k2)];
        for (int j = 0; j <= sizeof(k2); j++) {
            p2[j] = k2[j];
        }
        d[p1] = p2;
    }
    bool define_check(char* k1) {
        std::map<char*, char*> :: iterator it = d.begin();
        for (; it != d.end(); it++) {  // выводим их
            if (!strcmp(it->first, k1)) {
                return true;
            }
        }
        return false;
    }
    void define_delete(char* k1) {
        std::map<char*, char*> :: iterator it = d.begin();
        for (; it != d.end(); it++) {  // выводим их
            if (!strcmp(it->first, k1)) {
                break;
            }
        }
        if (it != d.end()) {
            delete [] it->first;
            delete [] it->second;
            d.erase(it);
        }
    }
    void define_change(char* k1, char* k2) {
        std::map<char*, char*> :: iterator it = d.find(k1);
        if (it != d.end()) {
            it->second = k2;
        }
        else {
            std::cout << "No pair to change\n";
        }
    }
    std::string define_second (char* k1) {
        std::map<char*, char*> :: iterator it = d.find(k1);
        if (it != d.end()) {
            return it->second;
        }
        else {
            std::cout << "No pair to find second\n";
            return "";
        }
    }
    void define_cout () {
        std::map<char*, char*> :: iterator it = d.begin();
        for (int k = 0; it != d.end(); it++, k++) {  // выводим их
            std::cout << k << ") Ключ " << it->first << ", значение " << it->second << std::endl;
        }
        std::cout << "\n";
    }
};

class token {
private:
    std::string t;
    Lexeme type;
    int row;
    int column;
public:
    
};

int main(int argc, const char * argv[]) { //ввод имя файла
    Define d;
    int fp,fl, state = -1, i = 0, i1 = 0;
    ssize_t n;
    char c, b;
    char com[8];
    com[0] = '\0';
    char dop[30];
    char dop1[30];
    dop[0] = '\0';
    dop1[0] = '\0';
    if ((fp = open(argv[1], O_RDWR)) < 0) {
        printf("Cannot open file.\n");
        exit(1);
    }
    if ((fl = creat("lprework.txt", 0666)) == -1) { //буфер для редактирования
        printf("Cannot create file.\n");
        exit(1);
    }
    /* список состояний:
     -3 после define + комбинация
     -2 пропускаю пробелы до директивы
     -1 обработка не символов для препроцессора
     0 обработка директивы
     1 удаляем комментарии пока встречаемый символ не \0
     2 удаляем комментарии пока не встретится * /
     3 подозрение на коментарий
     4 подозрение на закрытие комментария
     5 после директивы
     
     */
    while (1) {
        if((n = read(fp,&c, 1)) > 0) {
            switch (state)
            {
                case -1:
                    if (c == '#') {
                        state = -2;
                        i = 0;
                    }
                    else if (c == '/') {
                        state = 3;
                    }
                    else {
                        if (write(fl, &c, n) != n)
                            printf("Error in writing in.\n");
                    }
                    break;
                case -2:
                    if (c == '\0') {
                        state = -1;
                    }
                    else if (c == ' ') {
                        
                    }
                    else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                        com[i] = c;
                        i++;
                        state = 0;
                    }
                    else {
                        state = 1;
                    }
                    break;
                case 0:
                    if (i == 8) {
                        printf("Error in directive\n");
                        i = 0;
                        state = 1;
                    }
                    if (c == ' ') {
                        com[i] = '\0';
                        i = 0;
                        std::cout << "КОМАНДА " << com << "|\n";
                        state = 5;
                    }
                    else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                        com[i] = c;
                        i++;
                    }
                    else {
                        printf("Error in directive\n");
                        state = 1; //ожидание конца строки
                    }
                    break;
                case 1:
                    if (c == '\n') {
                        if (write(fl, &c, n) != n)
                           printf("Error in writing in.\n");
                        state = -1;
                    }
                    break;
                case -3: //after define + combination
                    if (c == '\n') {
                        state = -1;
                        if (!strcmp(com,"define")) {
                            if (!d.define_check(dop1)) {
                                d.define_insert(dop1, dop);
                            }
                            d.define_cout();
                            for (int j = 0; j <= i1;j++) {
                                dop1[j] = '\0';
                            }
                            for (int j = 0; j <= i;j++) {
                                dop[j] = '\0';
                            }
                            i = 0;
                            i1 = 0;
                            for (int j = 0; j <= 7;j++) {
                                com[j] = '\0';
                            }
                        }
                        else if (!strcmp(com,"undef")) {
                            if (d.define_check(dop)) {
                                d.define_delete(dop);
                            }
                            d.define_cout();
                            for (int j = 0; j <= i;j++) {
                                dop[j] = '\0';
                            }
                            i = 0;
                            for (int j = 0; j <= 6;j++) {
                                com[j] = '\0';
                            }
                        }
                    }
                    else if (c == ' ') {
                        
                    }
                    else {
                        state = 1;
                        if (!strcmp(com,"define")) {
                            for (int j = 0; j <= i1;j++) {
                                dop1[j] = '\0';
                            }
                            for (int j = 0; j <= i;j++) {
                                dop[j] = '\0';
                            }
                            i = 0;
                            i1 = 0;
                            for (int j = 0; j <= 7;j++) {
                                com[j] = '\0';
                            }
                        }
                        else if (!strcmp(com,"undef")) {
                            for (int j = 0; j <= i;j++) {
                                dop[j] = '\0';
                            }
                            i = 0;
                            for (int j = 0; j <= 6;j++) {
                                com[j] = '\0';
                            }
                        }
                    }
                    break;
                case 2:
                    if (c == '*') {
                        state = 4;
                    }
                    break;
                case 3:
                    if (c == '/') {
                        state = 1;
                    }
                    else if (c == '*') {
                        state = 2;
                    }
                    else {
                        b = '/';
                        if (write(fl, &b, n) != n)
                            printf("Error in writing in.\n");
                        if (write(fl, &c, n) != n)
                            printf("Error in writing in.\n");
                        state = -1;
                    }
                    break;
                case 4:
                    if (c == '/') {
                        state = -1;
                    }
                    else {
                        state = 2;
                    }
                    break;
                case 5: //after directive
                    if (c == '\n') {
                        state = -1;
                    }
                    else if (c == ' ') {
                        
                    }
                    else {
                        i = 0;
                        dop[i] = c;
                        i++;
                        state = 6;
                    }
                    break;
                case 6: //read lexeme, what next?
                    if (c == '\n' || c == ' ') {
                        dop[i] = '\0';
                        if (!strcmp(com,"define")) { //без пробелов в именах лексем!!!!
                            if (dop1[0] == '\0') {
                                if (c == '\n') {
                                    dop[0] = '\0';
                                    state = -1;
                                }
                                else {
                                    for (int j = 0; j <= i;j++) {
                                        dop1[j] = dop[j];
                                    }
                                    for (int j = 0; j <= i;j++) {
                                        dop[j] = '\0';
                                    }
                                    i1 = i;
                                    i = 0;
                                    state = 5;
                                }
                            }
                            else {
                                if (c == '\n') {
                                    state = -1;
                                    if (!d.define_check(dop1)) {
                                        d.define_insert(dop1, dop);
                                    }
                                    d.define_cout();
                                    for (int j = 0; j <= i1;j++) {
                                        dop1[j] = '\0';
                                    }
                                    for (int j = 0; j <= i;j++) {
                                        dop[j] = '\0';
                                    }
                                    for (int j = 0; j <= 7;j++) {
                                        com[j] = '\0';
                                    }
                                    i = 0;
                                    i1 = 0;
                                }
                                else {
                                    state = -3;
                                }
                            }
                        }
                        else if (!strcmp(com,"undef")) {
                            if (c == '\n') {
                                if (d.define_check(dop)) {
                                    d.define_delete(dop);
                                }
                                d.define_cout();
                                for (int j = 0; j <= i;j++) {
                                    dop[j] = '\0';
                                }
                                i = 0;
                                for (int j = 0; j <= 6;j++) {
                                    com[j] = '\0';
                                }
                                state = -1;
                            }
                            else {
                                state = -3;
                            }
                        }
                        else if (!strcmp(com,"include")) {
                            
                        }
                        else if (!strcmp(com,"if")) {
                            
                        }
                        else if (!strcmp(com,"ifdef")) {
                            
                        }
                        else if (!strcmp(com,"ifndef")) {
                            
                        }
                        else if (!strcmp(com,"if")) {
                            
                        }
                        else if (!strcmp(com,"else")) {
                            
                        }
                        else if (!strcmp(com,"elif")) {
                            
                        }
                        else if (!strcmp(com,"endif")) {
                            
                        }
                        else if (!strcmp(com,"line")) {
                            
                        }
                        else if (!strcmp(com,"error")) {
                            
                        }
                        else if (!strcmp(com,"pragma")) {
                            
                        }
                    }
                    else {
                        if (i == 30) {
                            printf("Lexem is bigger than 30\n");
                            for (int j = 0; j < i;j++) {
                                dop[j] = '\0';
                            }
                            state = 1;
                        }
                        else {
                            dop[i] = c;
                            i++;
                        }
                    }
                    break;
                case 7:
                    
                    break;
            };
        }
        else {
            close(fp);
            break;
        }
    }
    return 0;
}
