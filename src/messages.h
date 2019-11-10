/**
 * Lets have messages of format a typedef struct followed by
 * a serialize method and
 * a deserialize method.
 */

typedef struct {
    uint32_t type;
    uint32_t message_id;
    uint32_t payload;
} Message;


void serializeVC(Message* message, char* messageSerialized) {
    uint32_t temp;
    temp = htonl(message->type);
    memcpy(&messageSerialized[0], &temp, 4);
    temp = htonl(message->message_id);
    memcpy(&messageSerialized[4], &temp, 4);
    temp = htonl(message->payload);
    memcpy(&messageSerialized[8], &temp, 4);
}

void deserializeVC(char* buffer, Message* message)
{
    uint32_t tempo;
    memcpy(&tempo, &buffer[0], 4);
    message->type = ntohl(tempo);
    memcpy(&tempo, &buffer[4], 4);
    message->message_id = ntohl(tempo);
    memcpy(&tempo, &buffer[8], 4);
    message->payload = ntohl(tempo);
}