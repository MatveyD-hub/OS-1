#include "message.h"

void send(void* r, char* action, int id, char* pat, ID nam, int value) {
	zmq_msg_t actionMes;
    zmq_msg_t idMes;
    zmq_msg_t pathMes;
    zmq_msg_t nameMes;
    zmq_msg_t valueMes;
    zmq_msg_t Dop;
    
    char str1[10];
    sprintf(str1, "%d", id);
    
    char str[10];
    sprintf(str, "%d", value);
    
    zmq_msg_init_data (&actionMes, action, sizeof(action), NULL, NULL);
	zmq_msg_init_data (&idMes, str1, sizeof(str1), NULL, NULL);
	zmq_msg_init_data (&pathMes, pat, sizeof(pat), NULL, NULL);
	zmq_msg_init_data (&nameMes, nam,sizeof(nam), NULL, NULL);
	zmq_msg_init_data (&valueMes, str,sizeof(str), NULL, NULL);
	zmq_msg_init_data (&Dop, "dop", sizeof("dop"), NULL, NULL);
	
	printf("Пытаюсь отправить сообщение, action:%s id:%d pat:%s|, pathMes %s|, nameMes %s|\n",action, id, pat,(char*)zmq_msg_data(&pathMes), (char*)zmq_msg_data(&nameMes));
	//ZMQ_SNDMORE
	zmq_msg_send(&actionMes, r, 0);
	sleep(1);
	zmq_msg_send(&pathMes, r, 0);
	sleep(1);
	zmq_msg_send(&idMes, r, 0);
	sleep(1);
	zmq_msg_send(&nameMes, r, 0);
	sleep(1);
	zmq_msg_send(&valueMes, r, 0);
	sleep(1);
	zmq_msg_send(&Dop, r, 0);
	sleep(1);
	
	zmq_msg_close(&actionMes);
	zmq_msg_close(&idMes);
	zmq_msg_close(&pathMes);
	zmq_msg_close(&nameMes);
	zmq_msg_close(&valueMes);
	zmq_msg_close(&Dop);
}

message* rec(void* r) {
	message* m = malloc(sizeof(message));
	printf("Пытаюсь получить \n");
    zmq_msg_t actionMes;
    zmq_msg_t idMes;
    zmq_msg_t pathMes;
    zmq_msg_t nameMes;
    zmq_msg_t valueMes;
    zmq_msg_t Dop;
    
    zmq_msg_init(&actionMes);
    zmq_msg_init(&idMes);
    zmq_msg_init(&pathMes);
    zmq_msg_init(&nameMes);
    zmq_msg_init(&valueMes);
    zmq_msg_init(&Dop);
    
    zmq_msg_recv(&actionMes, r, 0);
    printf("ПОПЫТКА \n");
    int length = zmq_msg_size(&actionMes);
	m->action = malloc(length);
	//memcpy(m->action, zmq_msg_data(&actionMes), length);
    //printf("Получила %s \n",  m->action);
    zmq_msg_recv(&idMes, r, 0);
    zmq_msg_recv(&pathMes, r, 0);
    zmq_msg_recv(&nameMes, r, 0);
    zmq_msg_recv(&valueMes, r, 0);
    zmq_msg_recv(&Dop, r, 0);
    
    zmq_msg_close(&actionMes);
	zmq_msg_close(&idMes);
	zmq_msg_close(&pathMes);
	zmq_msg_close(&nameMes);
	zmq_msg_close(&valueMes);
	zmq_msg_close(&Dop);
	return m;
}
