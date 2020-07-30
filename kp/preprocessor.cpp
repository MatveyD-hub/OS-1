//  preprocessor for language C++
// Полей-Добронравова Амелия

#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <map>

enum Lexeme { //препроцессорные лексемы
    CONST, NAME, IDENTIFIER, KEYWORD, OPERATOR, NUMBER, SEPARATOR
};

class define {
    std::map<std::string, std::string> d;
public:
    void define_insert(std::string k1, std::string k2) {
        d[k1] = k2;
    }
    bool define_check(std::string k1) {
        std::map<std::string, std::string> :: iterator it = d.find(k1);
        if (it == d.end()) {
            return true; //если уже есть в словаре, вернуть true
        }
        else {
            return false;
        }
    }
    void define_delete(std::string k1) {
        std::map<std::string, std::string> :: iterator it = d.find(k1);
        if (it != d.end()) {
            d.erase(it);
        }
    }
    void define_change(std::string k1, std::string k2) {
        std::map<std::string, std::string> :: iterator it = d.find(k1);
        if (it != d.end()) {
            it->second = k2;
        }
        else {
            std::cout << "No pair to change\n";
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
    int fp,fl, state = -1, i = 0;
    ssize_t n;
    char c, b;
    char com[8];
    com[0] = '\0';
    
    if ((fp = open(argv[1], O_RDWR)) < 0) {
        printf("Cannot open file.\n");
        exit(1);
    }
    if ((fl = creat("lprework.txt", 0666)) == -1) { //буфер для редактирования
        printf("Cannot create file.\n");
        exit(1);
    }
    /* список состояний:
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
                case 5:
                    if (c == '\0') {
                        state = -1;
                    }
                    else if (c == ' ') {
                        
                    }
                    else {
                        
                    }
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
