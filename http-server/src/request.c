#include "../include/request.h"

Request parse_request(char* received_request) {
    Request req;
    char method[METHOD_LENGTH];
    char path[PATH_LENGTH];
    char protocol[PROTOCOL_LENGTH];
    char headers[HEADER_LENGTH];
    char body[BODY_LENGTH];
    headers[0] = '\0';
    body[0] = '\0';

    char* token;
    int iteration = 0;

    token = strtok(received_request, "\r\n");

    while (token != NULL) {
        iteration++;
        if (iteration == 1) {
            sscanf(token, "%6s %255s %9s", method, path, protocol);
            int method_valid = valid_method(method);

            strncpy(req.protocol, protocol, PROTOCOL_LENGTH - 1);
            req.protocol[PROTOCOL_LENGTH - 1] = '\0';

            if (method_valid == 0) {
                // invalid the whole request if checks fail
                strncpy(req.method, "", METHOD_LENGTH - 1);
                req.method[METHOD_LENGTH - 1] = '\0';
                strncpy(req.path, "/invalid", PATH_LENGTH - 1);
                req.path[PATH_LENGTH - 1] = '\0';
            } else {
                // return a valid request if everything went well with checking lengths
                strncpy(req.method, method, METHOD_LENGTH - 1);
                req.method[METHOD_LENGTH - 1] = '\0';
                strncpy(req.path, path, PATH_LENGTH - 1);
                req.path[PATH_LENGTH - 1] = '\0';
            }
        } else if (iteration == 6) {
            strncpy(body, token, BODY_LENGTH - 1);
            body[BODY_LENGTH - 1] = '\0';
            strncpy(req.body, body, BODY_LENGTH - 1);
            req.body[BODY_LENGTH - 1] = '\0';
        } else {
            strcat(headers, token);
            strcat(headers, " ");
        }
        token = strtok(NULL, "\r\n");
    }

    strncpy(req.headers, headers, HEADER_LENGTH - 1);
    req.headers[HEADER_LENGTH - 1] = '\0';
    return req;
}

void get_echo_tail(const char* path, char* tail) {
    const char* prefix = "/echo/";
    size_t prefix_len = strlen(prefix);

    if (strncmp(path, prefix, prefix_len) == 0) {
        const char* tail_start = path + prefix_len;
        strncpy(tail, tail_start, 6);
        tail[6] = '\0';
    }
}

int valid_method(char* method) {
    if (strncmp(method, "GET", 3) == 0 || strncmp(method, "POST", 4) == 0 || strncmp(method, "PUT", 3) == 0 ||
        strncmp(method, "DELETE", 6) == 0) {
        return 1;
    }
    return 0;
}

void parse_user_agent(char* headers, char* user_agent) {
    char* token;
    char* target = NULL;

    token = strtok(headers, " ");

    while (token != NULL) {
        token = strtok(NULL, " ");
        if (strncmp(token, "User-Agent:", 11) == 0) {
            target = token + 12;
            char* space = strstr(target, " ");
            size_t len = space - target;
            strncpy(user_agent, target, len);
            user_agent[len] = '\0';
            break;
        }
    }
}
