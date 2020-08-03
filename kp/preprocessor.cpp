//  preprocessor for language C++
// Полей-Добронравова Амелия

#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <map>
#include <string>
#include <dirent.h>

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
    ~Define() {
        std::map<char*, char*> :: iterator it = d.begin();
        for (; it != d.end(); it++) {  // выводим их
            delete [] it->first;
            delete [] it->second;
        }
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
    int _LINE_ = 1;
    struct dirent* entry;
    int result;
    DIR *dir = NULL;
    char* _FILE_ = new char[sizeof(&argv[1])];
    char com[8] = {'\0'};
    com[0] = '\0';
    for (int j = 0; j <= strlen(argv[1]); j++) {
        _FILE_[j] = argv[1][j];
    }
    int fp,fl, state = -1, i = 0, i1 = 0;
    ssize_t n;
    char c = '\0', b = '\0';
    char dop[30] = {'\0'};
    char dop1[30] = {'\0'};
    dop[0] = '\0';
    dop1[0] = '\0';
    char flag = ' ', flag1 = '+'; //for include
    if ((fp = open(argv[1], O_RDONLY)) < 0) {
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
     6 чтение лексем после директивы
     7 после include <
     8 после include "
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
                        printf("Error in directive 1\n");
                        for (int j = 0; j <= i;j++) {
                            com[j] = '\0';
                        }
                        i = 0;
                        state = 1;
                    }
                    if (c == ' ') {
                        com[i] = '\0';
                        i = 0;
                        std::cout << "КОМАНДА " << com << "|\n";
                        state = 5;
                    }
                    else if (c == '\n') {
                        com[i] = '\0';
                        i = 0;
                        std::cout << "КОМАНДА " << com << "|\n";
                        if (!strcmp(com,"else")) {
                            if (flag1 == '-') {
                                flag1 = '+';
                            }
                            else {
                                flag1 = '-';
                            }
                            for (int j = 0; j <= 5;j++) {
                                com[j] = '\0';
                            }
                            state = -1;
                        }
                        else {
                            state = 5;
                        }
                    }
                    else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                        com[i] = c;
                        i++;
                    }
                    else {
                        printf("Error in directive 2\n");
                        for (int j = 0; j <= i;j++) {
                            com[j] = '\0';
                        }
                        i = 0;
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
                        if (!strcmp(com,"error")) {
                            std::cout << "\n";
                            close(fp);
                            close(fl);
                            break;
                        }
                        else if ((!strcmp(com,"line"))) {
                            for (int y = 0; y < i1; y++) {
                                if (dop1[y] < '0' || dop1[y] > '9') {
                                    flag = 'f';
                                    break;
                                }
                            }
                            if (flag == 'f') {
                                std::cout << "Error in #line: line\n\n";
                                flag = ' ';
                            }
                            else {
                                _LINE_ = 0;
                                for (int y = 0; y < i1; y++) {
                                    _LINE_ = _LINE_ * 10 + dop1[y] - '0';
                                }
                                std::cout << "CHANGED LINE: " << _LINE_ << "\n";
                                state = -1;
                                for (int y = 0; y <= i1; y++) {
                                    dop1[y] ='\0';
                                }
                                i1 = 0;
                        }
                        }
                        else if (!strcmp(com,"endif")) {
                            flag1 = '+';
                            for (int j = 0; j <= 6;j++) {
                                com[j] = '\0';
                            }
                            for (int p = 0; p <= i; p++) {
                                dop[p] = '\0';
                            }
                            i = 0;
                        }
                        state = -1;
                        for (int p = 0; p <= i; p++) {
                            dop[p] = '\0';
                        }
                        for (int p = 0; p <= i1; p++) {
                            dop1[p] = '\0';
                        }
                        i = 0;
                        i1 = 0;
                        for (int j = 0; j <= 8;j++) {
                            com[j] = '\0';
                        }
                        flag = ' ';
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
                        if (!strcmp(com,"define") && flag1 == '+') { //без пробелов в именах лексем!!!!
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
                        else if (!strcmp(com,"undef") && flag1 == '+') {
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
                        else if (!strcmp(com,"include") && flag1 == '+') {
                            if (flag == ' ') {
                                if (!strcmp(dop,"<") || !strcmp(dop,"\"")) {
                                    flag = dop[0];
                                    state = 5;
                                    dop[0] = '\0';
                                    i = 0;
                                }
                                else if (dop[0] == '<') {
                                    if (dop[i - 1] == '>') {
                                        //поиск <>
                                        std::cout << "ПОИСК " << dop << "|\n\n";
                                        for (int p = 0; p <= i; p++) {
                                            dop[p] = '\0';
                                        }
                                        i = 0;
                                        flag = ' ';
                                        state = -1;
                                    }
                                    else {
                                        flag = dop[0];
                                        state = 5;
                                        for (int p = 0; p < i; p++) {
                                            dop1[p] = dop[p + 1];
                                        }
                                        i1 = i;
                                        
                                        for (int p = 0; p <= i; p++) {
                                            dop[p] = '\0';
                                        }
                                        i = 0;
                                    }
                                }
                                else if (dop[0] == '"') {
                                    if (dop[i - 1] == '"') {
                                        //поиск ""
                                        std::cout << "ПОИСК " << dop << "|\n\n";
                                        for (int p = 0; p <= i; p++) {
                                            dop[p] = '\0';
                                        }
                                        i = 0;
                                        flag = ' ';
                                        state = -1;
                                    }
                                    else {
                                        flag = dop[0];
                                        state = 5;
                                        for (int p = 0; p < i; p++) {
                                            dop1[p] = dop[p + 1];
                                        }
                                        i1 = i;
                                        for (int p = 0; p <= i; p++) {
                                            dop[p] = '\0';
                                        }
                                        i = 0;
                                    }
                                }
                                else {
                                    for (int j = 0; j <= 8;j++) {
                                        com[j] = '\0';
                                    }
                                    if (c == '\n') {
                                        state = -1;
                                    }
                                    else {
                                        state = 1;
                                    }
                                }
                            }
                            else {
                                if (c == '\n' && dop[0] == '\0') {
                                    for (int j = 0; j <= 8;j++) {
                                        com[j] = '\0';
                                    }
                                    for (int p = 0; p <= i; p++) {
                                        dop[p] = '\0';
                                    }
                                    for (int p = 0; p <= i1; p++) {
                                        dop1[p] = '\0';
                                    }
                                    i = 0;
                                    i1 = 0;
                                    flag = ' ';
                                    state = -1;
                                }
                                else {
                                    if (dop[i-1] == flag || dop[0] == flag || (flag == '<' && dop[0] == '>')) {
                                        if (flag == '"') {
                                            //поиск "
                                            std::cout << "ПОИСК \"" << dop1 << dop << "|\n\n";
                                            for (int p = 0; p <= i; p++) {
                                                dop[p] = '\0';
                                            }
                                            for (int p = 0; p <= i1; p++) {
                                                dop1[p] = '\0';
                                            }
                                            i = 0;
                                            flag = ' ';
                                            state = -1;
                                        }
                                        else {
                                            //поиск <
                                            std::cout << "ПОИСК <" << dop1 << dop << "|\n\n";
                                            for (int p = 0; p <= i; p++) {
                                                dop[p] = '\0';
                                            }
                                            for (int p = 0; p <= i1; p++) {
                                                dop1[p] = '\0';
                                            }
                                            i = 0;
                                            flag = ' ';
                                            state = -1;
                                        }
                                    }
                                    else if (dop1[0] == '\0' && c != '\n') {
                                        for (int p = 0; p <= i; p++) {
                                            dop1[p] = dop[p];
                                        }
                                        i1 = i;
                                        for (int p = 0; p <= i; p++) {
                                            dop[p] = '\0';
                                        }
                                        i = 0;
                                        state = 5;
                                    }
                                    else {
                                        for (int j = 0; j <= 8;j++) {
                                            com[j] = '\0';
                                        }
                                        state = -1;
                                        for (int p = 0; p <= i; p++) {
                                            dop[p] = '\0';
                                        }
                                        for (int p = 0; p <= i1; p++) {
                                            dop1[p] = '\0';
                                        }
                                        i = 0; i1 = 0;
                                        flag = ' ';
                                    }
                                }
                            }
                        }
                        else if (!strcmp(com,"if") && flag1 == '+') {
                            
                        }
                        else if (!strcmp(com,"ifdef") && flag1 == '+') {
                            if (d.define_check(dop)) {
                                flag1 = '+';
                            }
                            else {
                                flag1 = '-';
                            }
                            state = -1;
                            for (int j = 0; j <= 6;j++) {
                                com[j] = '\0';
                            }
                            for (int p = 0; p <= i; p++) {
                                dop[p] = '\0';
                            }
                            i = 0;
                        }
                        else if (!strcmp(com,"ifndef") && flag1 == '+') {
                            if (!d.define_check(dop)) {
                                flag1 = '+';
                            }
                            else {
                                flag1 = '-';
                            }
                            state = -1;
                            for (int j = 0; j <= 6;j++) {
                                com[j] = '\0';
                            }
                            for (int p = 0; p <= i; p++) {
                                dop[p] = '\0';
                            }
                            i = 0;
                        }
                        else if (!strcmp(com,"else") && flag1 == '-') {
                            flag1 = '+';
                            for (int j = 0; j <= 6;j++) {
                                com[j] = '\0';
                            }
                            for (int p = 0; p <= i; p++) {
                                dop[p] = '\0';
                            }
                            i = 0;
                            state = -1;
                        }
                        else if (!strcmp(com,"elif") && flag1 == '-') {
                            
                        }
                        else if (!strcmp(com,"endif")) {
                            flag1 = '+';
                            for (int j = 0; j <= 6;j++) {
                                com[j] = '\0';
                            }
                            for (int p = 0; p <= i; p++) {
                                dop[p] = '\0';
                            }
                            i = 0;
                            state = -1;
                        }
                        else if (!strcmp(com,"line") && flag1 == '+') {
                            if (dop1[0] == '\0') {
                                if (c == '\n') {
                                    for (int y = 0; y < i; y++) {
                                        if (dop[y] < '0' || dop[y] > '9') {
                                            flag = 'f';
                                            break;
                                        }
                                    }
                                    if (flag == 'f') {
                                        std::cout << "Error in #line: line\n";
                                        flag = ' ';
                                        for (int y = 0; y <= 5; y++) {
                                            com[i] = '\0';
                                        }
                                        state = 1;
                                    }
                                    else {
                                        _LINE_ = 0;
                                        for (int y = 0; y < i; y++) {
                                            _LINE_ = _LINE_ * 10 + dop[y] - '0';
                                        }
                                        std::cout << "CHANGED LINE: " << _LINE_ << "\n";
                                        state = -1;
                                    }
                                    for (int y = 0; y <= i; y++) {
                                        dop[i] = '\0';
                                    }
                                    i = 0;
                                }
                                else {
                                    state = 5;
                                    for (int p = 0; p <= i; p++) {
                                        dop1[p] = dop[p];
                                    }
                                    i1 = i;
                                    for (int p = 0; p <= i; p++) {
                                        dop[p] = '\0';
                                    }
                                    i = 0;
                                }
                            }
                            else {
                                for (int y = 0; y < i1; y++) {
                                    if (dop1[y] < '0' || dop1[y] > '9') {
                                        flag = 'f';
                                        break;
                                    }
                                }
                                if (flag == 'f') {
                                    std::cout << "Error in #line: line\n";
                                    flag = ' ';
                                    for (int y = 0; y <= 5; y++) {
                                        com[i] = '\0';
                                    }
                                    for (int p = 0; p <= i1; p++) {
                                        dop1[p] = '\0';
                                    }
                                    i1 = 0;
                                    state = 1;
                                }
                                else {
                                    _LINE_ = 0;
                                    for (int y = 0; y < i1; y++) {
                                        _LINE_ = _LINE_ * 10 + dop1[y] - '0';
                                    }
                                    std::cout << "CHANGED LINE: " << _LINE_ << "\n";
                                    for (int p = 0; p <= i1; p++) {
                                        dop1[p] = '\0';
                                    }
                                    i1 = 0;
                                    dir = opendir(".");
                                    if( dir == NULL ) {
                                        printf( "Error opening dir \n");
                                    }
                                    entry = readdir( dir );
                                    while (entry != NULL) {
                                        if (!strcmp(entry->d_name, dop)) {
                                            flag = 'f';
                                            std::cout << "Error in #line: filename\n";
                                            break;
                                        }
                                        entry = readdir( dir );
                                    }
                                    closedir( dir );
                                    if (flag == ' ') {
                                        result= rename(_FILE_, dop);
                                        if ( result == 0 )
                                            puts ( "File successfully renamed" );
                                        else
                                            perror( "Error renaming file" );
                                        for (int y = 0; y <= i; y++) {
                                            _FILE_[y] = dop[y];
                                        }
                                    }
                                    flag = ' ';
                                    for (int j = 0; j <= 5;j++) {
                                        com[j] = '\0';
                                    }
                                    state = -1;
                                }
                            }
                        }
                        else if (!strcmp(com,"error") && flag1 == '+') {
                            if (flag == ' ') {
                                std::cout << "Fatal error: ";
                                flag = 'f';
                            }
                            std::cout << dop << " ";
                            if (c == '\n') {
                                std::cout << "\n";
                                close(fp);
                                close(fl);
                                break;
                            }
                            else {
                                state = 5;
                                for (int p = 0; p <= i; p++) {
                                    dop[p] = '\0';
                                }
                                i = 0;
                            }
                        }
                        else if (!strcmp(com,"pragma") && flag1 == '+') {
                            
                        }
                        else if (c == ' ') {
                            for (int j = 0; j <= 8;j++) {
                                com[j] = '\0';
                            }
                            for (int p = 0; p <= i; p++) {
                                dop[p] = '\0';
                            }
                            i = 0;
                            state = 1;
                        }
                        else if (c == '\n') {
                            for (int j = 0; j <= 8;j++) {
                                com[j] = '\0';
                            }
                            for (int p = 0; p <= i; p++) {
                                dop[p] = '\0';
                            }
                            i = 0;
                            state = -1;
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
            close(fl);
            break;
        }
    }
    //return 0;
    exit(0);
}