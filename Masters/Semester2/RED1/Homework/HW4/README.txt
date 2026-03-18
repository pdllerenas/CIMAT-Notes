1. First, compile and run mq_sender.c, then compile and run mq_receiver.c.
2. On the receiver side, enter an ID (int)
3. On the sender side, send the message with the format [ID] [Message]
4. receiver side will see the message sent.
5. If another user runs the receiver side program, and enters a different ID, the sender may target that new ID.