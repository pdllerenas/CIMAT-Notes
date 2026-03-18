#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PERMS 0644
struct my_msgbuf
{
   long mtype; // This is where the ID goes!
   char mtext[200];
};

int main(void)
{
   struct my_msgbuf buf;
   int msqid;
   int len;
   char input[256];
   char *text_start;

   if ((msqid = msgget(0x0abc, PERMS | IPC_CREAT)) == -1)
   {
      perror("msgget");
      exit(1);
   }

   printf("Message queue ready. Enter: [ID] [Message]\n");
   printf("Press Ctrl+D to quit.\n");

   while (fgets(input, sizeof(input), stdin) != NULL)
   {
      long receiver_id = strtol(input, &text_start, 10);

      if (input == text_start)
      {
         printf("Error: Enter ID.\n");
         continue;
      }

      // space after [ID]
      if (*text_start == ' ')
         text_start++;

      len = strlen(text_start);
      // replace newline with null char
      if (len > 0 && text_start[len - 1] == '\n')
      {
         text_start[len - 1] = '\0';
         len--;
      }

      // receiver id attached to msg buffer
      buf.mtype = receiver_id;
      strcpy(buf.mtext, text_start); // don't send id, only after text start

      if (msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0) == -1)
      {
         perror("msgsnd");
      }
      else
      {
         printf("Sent to ID %ld: %s\n", buf.mtype, buf.mtext);
      }
   }

   buf.mtype = 10;
   strcpy(buf.mtext, "end");
   len = strlen(buf.mtext);
   if (msgsnd(msqid, &buf, len+1, 0) == -1)
      perror("msgsnd");

   printf("message queue: done sending messages.\n");
   return 0;
}