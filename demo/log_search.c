#include "log_producer_config.h"
#include "log_producer_client.h"
#include "log_error.h"

int main(){
    ClsLogSearchLogInit();
    char **topics = (char**)malloc(sizeof(char*)*128);
    topics[0] ="";
    topics[1] ="";
    int topics_size = 2;
    
    get_result* r = (get_result*)malloc(sizeof(get_result));
    memset(r, 0, sizeof(get_result));
    ClsSearchLog("ap-guangzhou.cls.tencentcs.com",
                            "",
                            "",
                            "",
                            topics,topics_size,"2022-07-04 15:00:00",
                            "2022-07-04 16:00:00","",
                            1,NULL,NULL,r);
    printf("request_id:%s|message:%s\n",r->requestID, r->message);

    free(topics);
    GetResultDestroy(r);
    ClsLogSearchLogDestroy();
    
    return 0;
}
