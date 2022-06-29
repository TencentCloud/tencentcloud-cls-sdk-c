//
// Created by herrylv on 06/5/2022
//
#include "log_producer_client.h"
#include "log_producer_manager.h"
#include "cls_log.h"
#include "post_logs_api.h"
#include <stdarg.h>
#include <string.h>

static uint32_t s_init_flag = 0;
static int s_last_result = 0;

unsigned int LOG_GET_TIME();

typedef struct
{

    ProducerManager *producermgr;
    ProducerConfig *producerconf;

} PrivateProducerClient;

struct clslogproducer
{
    clslogproducerclient *ancestor;
};

int ClsLogProducerInit()
{
    // if already init, just return s_last_result
    if (s_init_flag == 1)
    {
        return s_last_result;
    }
    s_init_flag = 1;
    if (0 != cls_log_init(LOG_GLOBAL_ALL))
    {
        s_last_result = LOG_PRODUCER_INVALID;
    }
    else
    {
        s_last_result = LOG_PRODUCER_OK;
    }
    return s_last_result;
}

void ClsLogProducerDestroy()
{
    if (s_init_flag == 0)
    {
        return;
    }
    s_init_flag = 0;
    cls_log_destroy();
}

clslogproducer *ConstructorClsLogProducer(ProducerConfig *config, SendCallBackFunc callbackfunc, void *user_param)
{
    if (!is_valid(config))
    {
        return NULL;
    }
    clslogproducer *producer = (clslogproducer *)malloc(sizeof(clslogproducer));
    clslogproducerclient *producerclient = (clslogproducerclient *)malloc(sizeof(clslogproducerclient));
    PrivateProducerClient *privateclient = (PrivateProducerClient *)malloc(sizeof(PrivateProducerClient));
    producerclient->private_client = privateclient;
    privateclient->producerconf = config;
    privateclient->producermgr = ConstructorProducerManager(config);
    privateclient->producermgr->callbackfunc = callbackfunc;
    privateclient->producermgr->user_param = user_param;

    cls_debug_log("create producer client success, topic : %s", config->topic);
    producerclient->efficient = true;
    producer->ancestor = producerclient;
    return producer;
}

void DestructorClsLogProducer(clslogproducer *producer)
{
    if (producer == NULL)
    {
        return;
    }
    clslogproducerclient *client = producer->ancestor;
    client->efficient = false;
    PrivateProducerClient *client_private = (PrivateProducerClient *)client->private_client;
    destroy_log_producer_manager(client_private->producermgr);
    DestroyClsLogProducerConfig(client_private->producerconf);
    free(client_private);
    free(client);
    free(producer);
}

extern clslogproducerclient *GetClsLogProducer(clslogproducer *producer, const char *config_name)
{
    if (producer == NULL)
    {
        return NULL;
    }
    return producer->ancestor;
}

int _PostClsLog(clslogproducerclient *client,
                int32_t pair_count, char **keys,
                int32_t *key_lens, char **values,
                int32_t *value_lens, int flush)
{
    if (client == NULL || !client->efficient)
    {
        return LOG_PRODUCER_INVALID;
    }

    ProducerManager *manager = ((PrivateProducerClient *)client->private_client)->producermgr;
    return log_producer_manager_add_log_int32(manager, pair_count, keys, key_lens, values, value_lens, flush, -1);
}

int PostClsLog(clslogproducerclient *client, int32_t kv_count, ...)
{

    //判断client的有效性
    if (client == NULL || !client->efficient)
    {
        return LOG_PRODUCER_INVALID;
    }
    va_list argp;
    va_start(argp, kv_count);

    int32_t pairs = kv_count / 2;

    //keys、values、key_lens、val_lens相当于是一个二维的数组
    char **keys = (char **)malloc(pairs * sizeof(char *));
    char **values = (char **)malloc(pairs * sizeof(char *));
    size_t *key_lens = (size_t *)malloc(pairs * sizeof(size_t));
    size_t *val_lens = (size_t *)malloc(pairs * sizeof(size_t));

    int32_t i = 0;
    for (; i < pairs; ++i)
    {
        const char *key = va_arg(argp, const char *);
        const char *value = va_arg(argp, const char *);
        keys[i] = (char *)key;
        values[i] = (char *)value;
        key_lens[i] = strlen(key);
        val_lens[i] = strlen(value);
    }

    int rst = _PostClsLog(client, pairs, keys, key_lens, values, val_lens, 0);
    free(keys);
    free(values);
    free(key_lens);
    free(val_lens);
    return rst;

    return 0;
}
