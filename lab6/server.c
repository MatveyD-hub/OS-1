#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "knot.h"

volatile sig_atomic_t flag = 0;

void block_func(int sig)
{   int i;
    printf("Write what you want:\n0|Block/Unblock server\n1|Close server\nYour choice:");
    scanf("%d", &i);
    if(!i){
      if(flag){
        printf("Server blocked!\n");
        flag = 0;
      }else{
        printf("Server unblocked!\n");
        flag = 1;
      }
      signal(SIGINT, block_func);
      return;
    }else{
      printf("Close server!\n");
      exit(0);
    }
}
typedef struct _DB {
    int id;
    struct _DB* l;
    struct _DB* r;
} db;

db* db_create() {
	db* a = malloc(sizeof(db));
	if (!a) {
		fprintf(stderr, "ERROR: no memory\n");
        exit(-1);
    }
    a->id = -1;
    a->r = a->l = NULL;
    return a;
}

char* db_find(db* t, int a, char* path) {
	int i;
	if (t == NULL) {
        return NULL;
    }
    else {
    	if (t->id == a) {
        	char vr[5];
        	sprintf(vr,"%d",t->id);
        	path = strcat(path,vr);
        	path[strlen(path)] = '\0';
        	return path; 	
        }
        else {
        	char vr[5];
        	sprintf(vr,"%d",t->id);
        	char* c1 = malloc(sizeof(255));
        	strcpy(c1,path);
        	i = 0;
        	int j = strlen(c1);
        	while (i < strlen(vr)) {
        		c1[j] = vr[i];
        		i++;
        		j++;
        	}
        	c1 = strcat(c1," ");
        	char* c2 = malloc(sizeof(255));
        	strcpy(c2,path);
        	c1 = db_find(t->l, a, c1);
        	c2 = db_find(t->r, a, c2);
        	if (c1 == NULL) {
        		free(c1);
        		return c2;
        	}
        	else {
        		free(c2);
        		return c1;
        	}
        }
    }
}

db* db_find_k(db* t, int a) {
	if (t == NULL) {
        return NULL;
    }
    else {
        if (t->id == a) {
        	return t;
        }
        else {
        	db* b = t;
            while (b->r != NULL) {
                if (b->r->id == a) {
                    return b->r;
                }
                else {
                    b = b->r;
                }
            }
            db* c1 = db_find_k(t->l, a); 
            db* c2 = db_find_k(t->r, a);
         	if (c1 != NULL) {
         		return c1;
         	} 
         	else {
         		return c2;
         	}
        }
    }
}

void db_add(db* t, int id, int p) {
	db* b = db_find_k(t, p);
	db* a = malloc(sizeof(db));
	if (!a) {
		fprintf(stderr, "ERROR: no memory\n");
        exit(-1);
    }
    a->id = id;
    a->r = a->l = NULL;
    if (b->l == NULL) {
    	b->l = a;
    }
    else {
    	b = b->l;
    	while (b->r != NULL) {
    		b = b->r;
    	}
    	b->r = a;
    }
}

void del_br(db* t) {
	if (t != NULL) {
		if (t->l != NULL ) {
		del_br(t->l);
		}
		if (t->r != NULL ) {
		del_br(t->r);
		}
		free(t);
	}
}
void db_del_k(db* t, int p) {
	db* b = t;
	while (b->r != NULL) {
		if (b->r->id == p) {
			del_br(b->r->l);
			b->r = b->r->r;
			break;
		}
		b = b->r;
	}
		if (t->l->id == p) {
			del_br(b->l->l);
			b->l = b->l->r;
		}
		else {
			if (t->l != NULL) {
				db_del_k(t->l, p);
			}
		}
}

void print_tree (db* t, int i) {
    if (t != NULL) {
        for (int j = 0; j < i; j++) {
            printf(" ");
        }
        printf("%d\n", t->id);
        print_tree(t->l, i+1);
        print_tree(t->r, i);
    }
}

int main(int argc, char * argv[]) //ввод имя файла с командами
{
	int fp;
	int u;
	char* th = malloc(255);
	if ((fp = open(argv[1], O_RDONLY)) < 0) {
		printf("Cannot open file.\n");
		exit(1);
	}
	knot* k = malloc(sizeof(knot));
	k->id = -1;
	db* work = db_create();
	char info[20],c;
	int n,i,j, p, va;
	while (1) {
		//signal(SIGINT, block_func);
		//if (!flag) {
			i = 0;
			while(i < strlen(th)) {
				th[i] = ' ';
				i++;
			}
			char* action = malloc(10);
			i = 0;
			do {
				if((n = read(fp,&c, 1)) > 0) {
				info[i] = c;
				i++;
				}
				else {
					printf("No more commands: %d\n",n);
					exit(0);
				}
			} while (c != '\n');
			info[i - 1] = '\0';
			printf("\nread new command: %s\n", info);
			i = 0;
			while (info[i] != ' ') {
				action[i] = info[i];
				i++;
			}
			i++;
			// типы команд:
			// create id [parent]
			// remove id
			// exec id name value
			// heartbit time
			if (strcmp(action,"exec") == 0) {
				j = 0;
				while (info[i] != ' ') {
					j = j * 10 + info[i] - '0';
					i++;
				}
				i++;
				char* path = db_find(work, j, "");
				if (path != NULL) {
					char* name = malloc(10);
					while (info[i] != ' ') {
						name[i] = info[i];
						i++;
					}
					p = 0;
					while (info[i] != ' ') {
						p = p * 10 + info[i] - '0';
						i++;
					}
					i++;
					send(k->r_fl, "exec", j, path, name, p);
					free(name);
				} 
				else {
					printf("Error: Not found\n");
				}
				i = 0;
				while (i < 20) {
					info[i] = ' ';
				}
			}
			else if (strcmp(action,"remove") == 0) {
				j = 0;
				while (info[i] != '\n') {
					j = j * 10 + info[i] - '0';
					i++;
				}
				i++;
				if (j == -29) {
					printf("Ok:%d:removed\n", k->id);
					knot_destroy(k);
					close(fp);
					exit(0);
				}
				char* path = db_find(work, j, "");
				if (path != NULL) {
					u = 0;
					while (path[u] != ' ') {
						u++;
					}
					u++;
					i = u;
					while(i < strlen(path)) {
						th[i - u - 1] = path[i];
						i++;
					}
					send(k->r_fl, "remove", j, path, "", -1);
					db_del_k(work, j);
					print_tree (work, 0);
				} 
				else {
					printf("Error: Not found\n");
				}
			}
			else if (strcmp(action,"create") == 0) {
				j = 0;
				while (info[i] != ' ' && info[i] != '\0') {
					j = j * 10 + info[i] - '0';
					i++;
				}
				i++;
				if (db_find(work, j, "") != NULL) {
					printf("Error: Already exists\n");
				} else {
					p = 0;
					while (info[i] != '\0') {
						p = p * 10 + info[i] - '0';
						i++;
					}
					i++;
				if (p == -29) {
					db_add(work, j, -1);
					printf("Tree: \n");
					print_tree(work, 0);
					knot_add(k, j);
				}
				 else {
				 	//нормально составить путь
					char* path = db_find(work, p,"");
					if (path != NULL) {
						printf("path: %s\n", path);
						db_add(work, j, p);
						u = 0;
						while (path[u] != ' ') {
							u++;
						}
						i = u + 1;
						while(i < strlen(path)) {
							th[i - u - 1] = path[i];
							i++;
						}
						send(k->r_fl, "create", j, th, "", -1);
						print_tree (work, 0);
					} 
					else {
						printf("Error: Parent not found\n");
					}
			}
				}
				i = 0;
				while (i < 20) {
					info[i] = ' ';
					i++;
				}
			}
			free(action);
		//}
	}
}
