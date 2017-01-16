//
// Created by danter on 1/16/17.
//

#ifndef GCF_SERVER_SOUPBIN_H
#define GCF_SERVER_SOUPBIN_H

#define USERNAME_LEN 6
#define PASSWD_LEN 10

#define SESSION_LEN 10
#define REQ_SEQ_LEN 20
#define LOGIN_REQ_PACKET_LEN 49
#define SOUP_BIN_HDR_LEN 3

#define LOGIN_ACC_PACKET_LEN 33
#define SOUP_HEARTBEAT_LEN 3

#define GET_LENGTH(buf) \
 (short)((unsigned char)buf[0] << 8 | (unsigned char)buf[1])

#define GET_TYPE(buf) buf[2]


#endif //GCF_SERVER_SOUPBIN_H
