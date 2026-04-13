#include <cstdio>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <error.h>

int main() {
  char* msg = (char*)malloc(2 * sizeof(char));
	msg[0] = 'a';
	msg[1] = '\0';
  struct msgbuf m = {1, *msg};
  int msgid = msgget(111, 0644 | IPC_CREAT);
  if (msgsnd(msgid, &m, 0, 0)) {
		perror("msgsnd");
    exit(1);
  }

	if (msgrcv(111, &m, sizeof(m.mtext), 0, 0) == -1) {
		perror("msgrcv");
		exit(1);
	}

	printf("%s\n", m.mtext);
	return 0;
}
