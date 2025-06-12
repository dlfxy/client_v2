// 1. disp头文件
#include <stdio.h>
#include <stdlib.h>  // 使用 system()
// ANSI 颜色代码
#define COLOR_MAIN   "\033[1;35m"
#define COLOR_PURPLE     "\x1b[38;5;141m"  // #a277ff (亮紫)
#define COLOR_TEAL       "\x1b[38;5;85m"   // #61ffca (蓝绿)
#define COLOR_ORANGE     "\x1b[38;5;222m"  // #ffca85 (浅橙)
#define COLOR_GRAY_WHITE "\x1b[38;5;255m"  // #edecee (灰白)
#define COLOR_PINK_PURPLE "\x1b[38;5;213m" // #f694ff (粉紫)
#define COLOR_RESET      "\x1b[0m"         // 重置颜色

#define CO_OPT1 COLOR_GRAY_WHITE  // 括号颜色
#define CO_OPT2 COLOR_PURPLE      // 数字颜色
#define CO_OPT3 COLOR_ORANGE      // 文本颜色
#define COLOR_WARNING COLOR_PINK_PURPLE

// 清屏宏 (跨平台)
#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

void disp_header();

// 打印左侧DIR栏和命令行提示
#define DIR_LABEL "DIR"
#define DIR_LABEL_WIDTH 8

void disp_prompt(const char* user, const char* cwd);



//2. client_init头文件

int sock_fd;//全局变量sockfd，不然很多函数都要加参填参很麻烦


//3. tlv头文件
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

typedef struct tlv_s {
    uint8_t  type;   // 主类别 (1字节)
    uint16_t len;    // 数据长度 (2字节)
    uint8_t  value[]; // 柔性数组
} tlv_t;



typedef enum {
    //  命令类型:COMMAND,0*
    CMD_SHORT_CD,   // 短的cd,不带数据
    CMD_SHORT_LS,   // 短的cd，不带数据
    CMD_LONG_CD,    // 长的cd，携带路径
    CMD_MKDIR,      // 列出文件
    CMD_REMOVE,         // 删除文件
    CMD_UPLOAD,     // 上传
    CMD_DOWNLOAD,   // 下载

    // 认证类型:AUTH,1*
    AUTH_REGISTER = 16,// 用户注册
    AUTH_LOGIN,     // 用户登录
    AUTH_LOGOUT,    // 用户登出
    AUTH_TOK,       // 发送token
    AUTH_TOK_REF,   // Token刷新
    AUTH_SALT,


    // 文件传输类型:TRANS,2*
    TRANS_META = 32,// 文件元数据
    TRANS_CHUNK_DATA,// 文件分块数据
    END_MARKER,     // 传输结束标志，空包
    TRANS_RESEND,   // 重传请求
    TRANS_STREAM_BEG,// 流式传输开始，空包
    STREAM_END,     // 流式传输结束，空包

    // 相应类型:RESPONSE暂时保留,3*
    SUCCESS_REGIS = 48, // 注册成功
    SUCCESS_LOGIN,      // 登录成功
    OPEN_DIR_FAILED,    // 目录不存在或不是目录
    CHANGE_DIR_SUCCESS, // 切换目录成功
    MKDIR_DIR_FAILED,   //  创建目录失败
    MKDIR_DIR_SUCCESS,  //  创建目录成功
    REMOVE_DIR_FAILED,  //  删除目录失败
    REMOVE_DIR_SUCCESS, //  删除目录成功                                                    
    UPLOAD_FILE_FAILED, // 上传文件失败 
    UPLOAD_FILE_SUCCESS,// 上传文件成功 
    DOWNLOAD_FILE_FAILED,// 下载文件失败 
    DOWNLOAD_FILE_SUCCESS,// 下载文件成功 

    // 错误类型:ERROR,4*
    ERR_NAME_CONFLICT = 64,// 重名错误
    ERR_PASSWORD_INVALID,  // 密码错误
    ERR_USER_NOT_FOUND,    // 未找到用户名
    ERR_FILE,                   // 文件操作错误
    ERR_NET,                    // 网络错误
    ERR_SER                     // 服务端内部错误
}tlvType;



//4. handle头文件

int handle_usr(int type);






//5. usr头文件
char usr_name[128];//用户
char cwd[1024];
int usr_register();
int usr_login();
void usr_exit();

//6. auth头文件





//7. cmd头文件









int main() {

    // TCP 连接
    //init_client(, &sock_fd);

    //展示UI
    disp_header();

    int type;
    scanf("%d", &type);
    handle_usr(type);

    //char cwd[512] = "~/"; // 默认初始目录,handle里面可能用得上：应该是在handle里自己定义

    system(CLEAR_SCREEN);
    while (1)
    {
        char command[128] = { 0 };
        //每次展示prompt
        disp_prompt(usr_name, cwd);
        scanf("%s", command);
        //handle_cmd(command);
    }
}


//1. disp函数

void disp_header() {
    system(CLEAR_SCREEN);
    printf("\n");
    printf(
        COLOR_TEAL
        "\t__        __                              _                     ____    _         _    \n"
        "\t \\ \\      / /   __ _   _ __     __ _    __| |   __ _    ___     |  _ \\  (_)  ___  | | __\n"
        "\t  \\ \\ /\\ / /   / _` | | '_ \\   / _` |  / _` |  / _` |  / _ \\    | | | | | | / __| | |/ /\n"
        "\t   \\ V  V /   | (_| | | | | | | (_| | | (_| | | (_| | | (_) |   | |_| | | | \\__ \\ |   < \n"
        "\t    \\_/\\_/     \\__,_| |_| |_|  \\__, |  \\__,_|  \\__,_|  \\___/    |____/  |_| |___/ |_|\\_\\\n"
        "\t                               |___/\n"
        COLOR_RESET
    );
    printf(COLOR_TEAL "\n\tYour files, everywhere.\n" COLOR_RESET);
    printf("\n");
    printf("\n");

    printf(CO_OPT1 "\t[" CO_OPT2 "0" CO_OPT1 "]" CO_OPT3 "  新用户注册\t" COLOR_RESET);
    printf(CO_OPT1 "\t[" CO_OPT2 "1" CO_OPT1 "]" CO_OPT3 "  用户登录\t" COLOR_RESET);
    printf(CO_OPT1 "\t[" CO_OPT2 "2" CO_OPT1 "]" CO_OPT3 "  忘记密码\t" COLOR_RESET);
    printf(CO_OPT1 "\t[" CO_OPT2 "3" CO_OPT1 "]" CO_OPT3 "  退出系统\t\n" COLOR_RESET);
    printf(
        COLOR_GRAY_WHITE "\n"
        "\t---------------------------------------------------------\n" COLOR_RESET
        COLOR_ORANGE          "\t* " COLOR_TEAL"今日推荐：" COLOR_ORANGE "新用户注册即送 5GB 免费存储空间!\n"
        "\t* " COLOR_TEAL "系统状态：" COLOR_ORANGE "正常运行中,快去备份你的文件吧!\n"
        "\t* " COLOR_TEAL "最新动态：" COLOR_ORANGE "客户端 v5.2.0 即将发布，" COLOR_PINK_PURPLE "新增6元首充礼包!\n"
        COLOR_GRAY_WHITE "\t---------------------------------------------------------\n\n"
    );
    printf(COLOR_WARNING "\t>>请选择操作编号 [" COLOR_PURPLE "0-3" COLOR_WARNING "]:" COLOR_RESET "   ");
    fflush(stdout);
}

void disp_prompt(const char* user, const char* cwd) {
    printf("\033[2J\033[H");
    // 左侧大写DIR
    printf("\033[1;34m%-*s\033[0m", DIR_LABEL_WIDTH, DIR_LABEL); // 蓝色高亮
    printf("  @%s: %s\n", user, cwd);
    printf("%-*s", DIR_LABEL_WIDTH, ""); // 补齐左栏
    fflush(stdout);
}
//client_init函数

void disp_end() {

}

//2. tlv函数

// 创建TLV包
tlv_t* tlv_create(uint8_t type, const void* data, uint16_t len) {
    // 检查参数有效性
    if (len > 0 && data == NULL) {
        fprintf(stderr, "错误: 非空长度需要有效数据指针\n");
        return NULL;
    }

    // 计算总内存大小（结构体 + 数据区）
    size_t total_size = sizeof(tlv_t) + len;
    tlv_t* tlv = (tlv_t*)malloc(total_size);
    if (tlv == NULL) {
        perror("内存分配失败");
        return NULL;
    }

    // 填充TLV头部
    tlv->type = type;
    tlv->len = len;

    // 如果有数据则拷贝
    if (len > 0 && data != NULL) {
        memcpy(tlv->value, data, len);
    }

    return tlv;
}

// 释放TLV包
void tlv_free(tlv_t* tlv) {
    if (tlv) {
        free(tlv);
    }
}

// 发送TLV包
int tlv_send(int sockfd, tlv_t* tlv) {
    // 发送头部 (type + len)
    uint8_t header[3];
    header[0] = tlv->type;
    *(uint16_t*)(header + 1) = htons(tlv->len); // 网络字节序

    if (send(sockfd, header, 3, 0) != 3) {
        return -1; // 发送头部失败
    }

    // 发送数据体 (如果有)
    if (tlv->len > 0 && send(sockfd, tlv->value, tlv->len, 0) != tlv->len) {
        return -1; // 发送数据失败
    }
    return 0;
}

// 接收TLV包
tlv_t* tlv_recv(int sockfd) {
    // 接收头部
    uint8_t header[3];
    if (recv(sockfd, header, 3, MSG_WAITALL) != 3) {
        return NULL; // 接收头部失败
    }

    // 解析头部
    uint8_t type = header[0];
    uint16_t len = ntohs(*(uint16_t*)(header + 1)); // 主机字节序

    // 创建TLV包
    tlv_t* tlv = (tlv_t*)malloc(sizeof(tlv_t) + len);
    if (!tlv) return NULL;

    tlv->type = type;
    tlv->len = len;

    // 接收数据体 (如果有)
    if (len > 0 && recv(sockfd, tlv->value, len, MSG_WAITALL) != len) {
        free(tlv);
        return NULL;
    }
    return tlv;
}



//3. handle函数

int handle_usr(int type)
{
    switch (type)
    {
    case 0:usr_register();
    case 1:usr_login();
    case 2:usr_exit();
    case 3:usr_exit();
    }
    return 0;
}




//4. usr函数

int usr_register() {
    char salt[1024] = { 0 };
    while (1)
    {
        char pswd1[1024] = { 0 };
        char pswd2[1024] = { 0 };
        fflush(stdout);
        // 1. 输入用户名，判断是否重名
        while (1)
        {
            memset(usr_name, 0, sizeof(usr_name));
            printf("用户名: ");
            scanf("%127s", usr_name);

            //发送用户名
            tlv_t* t = tlv_create(AUTH_REGISTER, usr_name, strlen(usr_name) + 1);
            tlv_send(sock_fd, t);
            tlv_free(t);

            //收服务端的响应
            t = tlv_recv(sock_fd);

            //正确，保存盐值退出
            if (AUTH_SALT == t->type && t->len > 0)//收到的包不对也让用户重新输入
            {
                memcpy(salt, t->value, sizeof(salt) - 1);
                salt[strlen(salt) + 1] = '\0'; // 确保字符串终止
                tlv_free(t);
                break;
            }
            //错误继续
            else if (ERR_NAME_CONFLICT == t->type)
            {
                printf("\t错误：用户名重复！\n");
                tlv_free(t);
            }
        }

        //清理输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);


        while (1)
        {
            // 2. 输入两次密码并判断
            char* p1 = getpass("\t密码：  "); strncpy(pswd1, p1, sizeof(pswd1) - 1);
            char* p2 = getpass("\t再次输入密码：  ");
            strncpy(pswd2, p2, sizeof(pswd2) - 1);
            if (strcmp(pswd1, pswd2) != 0)
            {
                printf("\t输入错误！\n");
            }
            else//两次输入一致
            {
                //todo:加盐,发送密码
                //auth_pswd();
                tlv_t* t = tlv_create(AUTH_REGISTER, pswd1, strlen(pswd1) + 1);
                tlv_send(sock_fd, t);
                tlv_free(t);

                //收服务端的响应
                t = tlv_recv(sock_fd);
                if (SUCCESS_LOGIN == t->type)
                {
                    //todo:保存cwd的内容
                    tlv_free(t);
                    printf("\t注册成功！欢迎你，%s。\n", usr_name);
                    sleep(1);
                    return 0;
                }
            }
        }
    }
}



int usr_login() {

    char salt[1024] = { 0 };
    while (1)
    {

        char pswd1[1024] = { 0 };
        char pswd2[1024] = { 0 };

        fflush(stdout);

        // 1. 输入用户名，判断是否重名
        while (1)
        {
            memset(usr_name, 0, sizeof(usr_name));
            printf("\t用户名: ");
            scanf("%127s", usr_name);

            //发送用户名
            tlv_t* t = tlv_create(AUTH_LOGIN, usr_name, strlen(usr_name) + 1);
            tlv_send(sock_fd, t);
            tlv_free(t);

            //收服务端的响应
            t = tlv_recv(sock_fd);

            //正确，保存盐值break
            if (AUTH_SALT == t->type && t->len > 0)//收到的包不对也让用户重新输入
            {
                memcpy(salt, t->value, sizeof(salt) - 1);
                salt[strlen(salt) + 1] = '\0'; // 确保字符串终止
                tlv_free(t);
                break;
            }
            //错误继续
            else if (ERR_USER_NOT_FOUND == t->type)
            {
                printf("\t错误：用户名不存在！\n");
                tlv_free(t);
            }
        }

        //清理输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        while (1)
        {
            // 2. 输入两次密码并判断
            char* p1 = getpass("\t密码：  "); strncpy(pswd1, p1, sizeof(pswd1) - 1);
            char* p2 = getpass("\t再次输入密码：  ");
            strncpy(pswd2, p2, sizeof(pswd2) - 1);
            if (strcmp(pswd1, pswd2) != 0)
            {
                printf("\t输入错误！\n");
            }
            else
            {
                //todo:加盐,发送密码
                //auth_pswd();
                tlv_t* t = tlv_create(AUTH_LOGIN, pswd1, strlen(pswd1) + 1);
                tlv_send(sock_fd, t);
                tlv_free(t);

                //收服务端的响应
                t = tlv_recv(sock_fd);
                if (SUCCESS_LOGIN == t->type)
                {
                    //todo:保存cwd的内容
                    tlv_free(t);
                    printf("\t登录成功！欢迎你，%s。\n", usr_name);
                    sleep(1);
                    return 0;
                }
                else if (ERR_PASSWORD_INVALID == t->type)
                {
                    printf("\t密码错误！\n");
                    tlv_free(t);
                }
            }

        }
    }
}


void usr_exit()
{
    disp_end();
    exit(0);
}

//5. auth函数(加盐，sha256的信息等)





//6.cmd函数
