
#include "log_producer_config.h"
#include "log_producer_client.h"
#include "log_error.h"

void callback(const char *config_name, int result, size_t log_bytes, size_t compressed_bytes, const char *req_id, const char *message, const unsigned char *raw_buffer)
{
    if (result == LOG_PRODUCER_OK)
    {
        if (req_id == NULL)
        {
            req_id = "";
        }
        printf("send success, config : %s, result : %d, log bytes : %d, compressed bytes : %d, request id : %s \n",
               config_name, (result),
               (int)log_bytes, (int)compressed_bytes, req_id);
    }
    else
    {
        if (message == NULL)
        {
            message = "";
        }
        if (req_id == NULL)
        {
            req_id = "";
        }
        printf("send fail, config : %s, result : %d, log bytes : %d, compressed bytes : %d, request id : %s, error message : %s\n",
               config_name, (result),
               (int)log_bytes, (int)compressed_bytes, req_id, message);
    }
}

clslogproducer *ConstructorLogProducer(SendCallBackFunc notifyFunc)
{
    //调用malloch函数开辟内存，并初始化默认的配置
    ProducerConfig *config = ConstructLogConfig();
    SetEndpoint(config, "ap-guangzhou.cls.tencentcs.com");
    SetAccessId(config, "");
    SetAccessKey(config, "");
    SetTopic(config, "");
    SetPackageLogBytes(config, 4 * 1024 * 1024);
    setPackageTimeout(config, 3000);
    SetMaxBufferLimit(config, 64 * 1024 * 1024);
    set_send_thread_count(config, 4);
    SetCompressType(config, 1);
    SetConnectTtimeoutSec(config, 10);
    SetSendTimeoutSec(config, 15);
    return ConstructorClsLogProducer(config, notifyFunc, NULL);
}

void post_logs()
{
    if (ClsLogProducerInit(LOG_GLOBAL_ALL) != LOG_PRODUCER_OK)
    {
        printf("ClsLogProducerInit init fail \n");
        exit(1);
    }
    clslogproducer *producer = ConstructorLogProducer(callback);
    if (producer == NULL)
    {
        printf("create log producer by config fail \n");
        exit(1);
    }

    clslogproducerclient *client = GetClsLogProducer(producer, NULL);
    if (client == NULL)
    {
        printf("create log producer client by config fail \n");
        exit(1);
    }
    int i = 0;
    for (; i < 10; ++i)
    {
        char indexStr[32];
        sprintf(indexStr, "%d", i);

        int rst = PostClsLog(client, 20, "key1", "value_1",
                             "key2", "value_2",
                             "key3", "value_3",
                             "key4", "value_4",
                             "key5", "value_5",
                             "key6", "value_6",
                             "key7", "value_7",
                             "key8", "value_8",
                             "key9", "value_9",
                             "index", indexStr);
        if (rst != LOG_PRODUCER_OK)
        {
            printf("add log error %d \n", rst);
        }
    }

    DestructorClsLogProducer(producer);

    ClsLogProducerDestroy();
}

int main(int argc, char *argv[])
{
    post_logs();
    return 0;
}
