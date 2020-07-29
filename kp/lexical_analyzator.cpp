//  lexical_analyzator for language C++
// Полей-Добронравова Амелия

#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

enum Lexeme { //препроцессорные лексемы
    CONST, NAME, IDENTIFIER, KEYWORD, OPERATOR, NUMBER, SEPARATOR
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
    int fp,fl, state = -1;
    ssize_t n;
    char c;
    char com[20];
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
     -1 обработка не символов для препроцессора
     0 обработка директивы
     1 удаляем комментарии пока встречаемый символ не \0
     2 удаляем комментарии пока не встретится * /
     3 подозрение на коментарий
     4 подозрение на закрытие комментария
     
     */
    while (1) {
        if((n = read(fp,&c, 1)) > 0) {
            switch (state)
            {
                case -1:
                    if (c == '#') {
                        state = 0;
                    }
                    else if (c == '/') {
                        state = 4;
                    }
                    if (write(fl, &c, n) != n)
                        printf("Error in writing in.\n");
                    break;
                case 0:
                    
                    break;
                case 1:
                    if (c == '\0') {
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
                    break;
                case 4:
                    if (c == '/') {
                        state = -1;
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
