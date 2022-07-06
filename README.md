# tencentcloud-cls-sdk-c
## 日志上传

### 功能特点

* 异步
    * 异步写入，客户端线程无阻塞
* 聚合&压缩 上传
    * 支持按超时时间、日志数、日志size聚合数据发送
    * 支持lz4压缩
* 缓存
    * 支持缓存上限可设置
    * 超过上限后日志写入失败



- 核心上报架构

![ios核心架构图](ios_sdk.jpg)

### 环境依赖

C Producer使用curl进行网络操作，您需要确认这些库已经安装，并且将它们的头文件目录和库文件目录都加入到了项目中。

### libcurl下载以及安装

libcurl建议 7.49.0 及以上版本

### LOG C SDK的安装

安装时请在cmake命令中指定第三方库头文件以及库文件的路径，编译命令如下：

```
    cmake .
    make
    make install
```

### 配置参数

| 参数                         | 说明                                                         |                             取值                             |
| ---------------------------- | ------------------------------------------------------------ | :----------------------------------------------------------: |
| topic                        | 日志主题 ID ，通过接口SetTopic设置                           | 可在控制台获取https://console.cloud.tencent.com/cls/logset/desc |
| accessKeyId                  | 通过接口setAccessKeyId设置                                   | 参考官网文档：https://cloud.tencent.com/document/product/614/12445 |
| accessKey                    | 通过接口setAccessKeySecret设置                               | 参考官网文档：https://cloud.tencent.com/document/product/614/12445 |
| endpoint                     | 地域信息。通过接口setEndpoint设置，                          | 参考官方文档：https://cloud.tencent.com/document/product/614/18940 |
| logBytesPerPackage           | 缓存的日志包的大小上限，取值为1~5242880，单位为字节。默认为1024 * 1024。通过SetPackageLogBytes接口设置 |                        整数，单位字节                        |
| logCountPerPackage           | 缓存的日志包中包含日志数量的最大值，取值为1~10000，默认为1024条。通过SetPackageLogCount接口设置 |                             整数                             |
| packageTimeoutInMS           | 日志的发送逗留时间，如果缓存超时，则会被立即发送，单位为毫秒，默认为3000。通过SetPackageTimeout接口设置 |                        整数，单位毫秒                        |
| maxBufferBytes               | 单个Producer Client实例可以使用的内存的上限，超出缓存时add_log接口会立即返回失败。通过接口SetMaxBufferLimit设置 |                        整数，单位字节                        |
| sendThreadCount              | 发送线程数，默认为1。通过接口SetSendThreadCount设置          |                             整数                             |
| connectTimeoutSec            | 网络连接超时时间，默认为10s。通过接口SetConnectTimeoutSec设置 |                         整数，单位秒                         |
| sendTimeoutSec               | 读写超时，默认为15s。通过接口SetSendTimeoutSec设置           |                         整数，单位秒                         |
| destroyFlusherWaitTimeoutSec | flusher线程销毁最大等待时间，默认为1s。通过接口SetDestroyFlusherWaitSec设置 |                         整数，单位秒                         |
| destroySenderWaitTimeoutSec  | sender线程池销毁最大等待时间，默认为1s。通过接口SetDestroySenderWaitSec设置 |                         整数，单位秒                         |
| compressType                 | 数据上传时的压缩类型，默认为LZ4压缩，默认为1s。通过接口SetCompressType设置 |                0 不压缩，1 LZ4压缩， 默认为1                 |

### 使用demo

```
    if (ClsLogProducerInit(LOG_GLOBAL_ALL) != LOG_PRODUCER_OK)
    {
        printf("ClsLogProducerInit init fail \n");
        exit(1);
    }
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
    clslogproducer *producer = ConstructorClsLogProducer(config, notifyFunc, NULL);
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
    DestructorClsLogProducer(producer);

    ClsLogProducerDestroy();
```

## 日志检索

- ##### 接口api文档参考官网：https://cloud.tencent.com/document/product/614/16875

- ##### 接口描述

  - 请求入参描述

  | 字段名       | 类型   | 位置  | 必须 | 含义                                                         |
  | :----------- | :----- | :---- | :--- | :----------------------------------------------------------- |
  | logset_id    | string | query | 是   | 要查询的 logset ID                                           |
  | topic_ids    | string | query | 是   | 要查询的 topic ID                                            |
  | start_time   | string | query | 是   | 要查询的日志的起始时间，格式 YYYY-mm-dd HH:MM:SS             |
  | end_time     | string | query | 是   | 要查询的日志的结束时间，格式 YYYY-mm-dd HH:MM:SS             |
  | query_string | string | query | 是   | 查询语句，详情参考 [检索语法与规则](https://cloud.tencent.com/document/product/614/47044) |
  | limit        | int    | query | 是   | 单次要返回的日志条数，单次返回的最大条数为100                |
  | context      | string | query | 否   | 加载更多使用，透传上次返回的 context 值，获取后续的日志内容，通过游标最多可获取10000条，请尽可能缩小时间范围 |
  | sort         | string | query | 否   | 按时间排序 asc（升序）或者 desc（降序），默认为 desc         |

  - 响应描述

  | 字段名   | 类型                   | 必有 | 含义                     |
  | :------- | :--------------------- | :--- | :----------------------- |
  | context  | string                 | 是   | 获取更多检索结果的游标   |
  | listover | bool                   | 是   | 搜索结果是否已经全部返回 |
  | results  | JsonArray（LogObject） | 是   | 日志内容信息             |

  - LogObject 格式如下

  | 字段名     | 类型   | 必有 | 含义                |
  | :--------- | :----- | :--- | :------------------ |
  | topic_id   | string | 是   | 日志属于的 topic ID |
  | topic_name | string | 是   | 日志主题的名字      |
  | timestamp  | string | 是   | 日志时间            |
  | content    | string | 是   | 日志内容            |
  | filename   | string | 是   | 采集路径            |
  | source     | string | 是   | 日志来源设备        |

- ##### 接口描述

```objective-c
/*
@region 地域信息
@secretid 密钥id
@secretkey 密钥
@logsetid 日志集id
@topicids topic列表
@starttime 要查询的日志的起始时间，格式 YYYY-mm-dd HH:MM:SS
@endtime 要查询的日志的结束时间，格式 YYYY-mm-dd HH:MM:SS
@query 查询语句
@limit 单次要返回的日志条数，单次返回的最大条数为100
@context 加载更多使用，透传上次返回的 context 值，获取后续的日志内容，通过游标最多可获取10000条，请尽可能缩小时间范围
@sort 按时间排序 asc（升序）或者 desc（降序），默认为 desc
*/
int ClsSearchLog(const char *region,const char *secretid, const char* secretkey,const char* logsetid,const char **topicids,const int topicidslens,const char* starttime,const char* endtime,const char* query,size_t limit,const char* context,const char* sort,get_result* result){

struct result
{
    int statusCode;//返回码
    char * message;//响应消息回包 json格式
    char * requestID; //本次请求序列号
};
```

- ##### 使用demo

```objective-c

LogSearchClient *sclient = [[LogSearchClient alloc] init];
//topicid以数组的形式传递
NSArray *topics = [NSArray arrayWithObjects:@"your topicid",nil];
    SearchReult r = [sclient SearchLog:@"ap-guangzhou.cls.tencentcs.com" secretid:@"" secretkey:@"" logsetid:@"" topicids:topics starttime:@"" endtime:@"" query:@"" limit:10 context:nil sort:nil];
    NSLog(@"%@",r.message);

//释放资源
[sclient DestroyLogSearch];

//初始化LogSearchClient对象 
ClsLogSearchLogInit();
char **topics = (char**)malloc(sizeof(char*)*128);
topics[0] ="";
topics[1] ="";
int topics_size = 2;
//申请回包响应结构内存
get_result* r = (get_result*)malloc(sizeof(get_result));
memset(r, 0, sizeof(get_result));

//调用检索接口
ClsSearchLog("ap-guangzhou.cls.tencentcs.com",
                            "",
                            "",
                            "",
                            topics,topics_size,"2022-07-04 15:00:00",
                            "2022-07-04 16:00:00","",
                            1,NULL,NULL,r);
printf("request_id:%s|message:%s\n",r->requestID, r->message);

free(topics);
//释放内存
GetResultDestroy(r);
ClsLogSearchLogDestroy();
```

