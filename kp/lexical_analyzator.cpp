//  lexical_analyzator for language C++
// Полей-Добронравова Амелия

#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

enum Lexeme {
    IDENTIFIER, KEYWORD, OPERATOR, LITERAL, SEPARATOR
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
    int fp;
    ssize_t n;
    char c;
    char com[20];
    com[0] = '\0';
    if ((fp = open(argv[1], O_RDWR)) < 0) {
        printf("Cannot open file.\n");
        exit(1);
    }
    while (1) {
        if((n = read(fp,&c, 1)) > 0) {
            
        }
        else {
            close(fp);
            break;
        }
    }
    return 0;
}
