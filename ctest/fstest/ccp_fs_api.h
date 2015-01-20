#ifndef __CCP_FS_API_H
#define __CCP_FS_API_H

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/un.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <ctype.h>
#include <stdbool.h>

#include "naclient.h"	// Add by icbilly, 2014.02.24, for CLAY FS Session Alarm send in FS API
#ifdef __FD_SETSIZE
#undef __FD_SETSIZE
#define __FD_SETSIZE 2048
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Error code for Watcher Interface
#define CCP_FS_OK                       (0)

#define CCP_FS_ERR_SOCK_CREATE			(-50)
#define CCP_FS_ERR_SOCK_SETOPT_KEEP		(-51)
#define CCP_FS_ERR_SOCK_SETOPT_LINGER	(-52)
#define CCP_FS_ERR_SOCK_SETOPT_NODELAY	(-53)
#define CCP_FS_ERR_SOCK_GETHOSTNAME		(-54)
#define CCP_FS_ERR_SOCK_CONNECT			(-55)
#define CCP_FS_ERR_SOCK_WRITABLE		(-56)
#define CCP_FS_ERR_SOCK_GETOPT_ERROR	(-57)
#define CCP_FS_ERR_SOCK_INVALID_FD		(-58)


#define CCP_FS_ERR_WATCHER_CONNECT      (-80)
#define CCP_FS_ERR_WATCHER_GET_MASTER   (-81)
#define CCP_FS_ERR_WATCHER_HA_SESSION   (-82)
#define CCP_FS_ERR_WATCHER_HNDL_SESSION (-83)
#define CCP_FS_ERR_WATCHER_IP_LIST      (-84)
#define CCP_FS_ERR_WATCHER_IP_LIST_CNT  (-85)
#define CCP_FS_ERR_WATCHER_INPUT_PARAM  (-87)

#define CCP_FS_ERR_REDIS_NOMEM          (-89)

#define CCP_FS_ERR                      (-90)
#define CCP_FS_ERR_NOMEM                (-91)
#define CCP_FS_ERR_RESOLVE              (-92)
#define CCP_FS_ERR_CONNECT              (-93)
#define CCP_FS_ERR_SEND                 (-94)
#define CCP_FS_ERR_RECV                 (-95)
#define CCP_FS_ERR_TIMEOUT              (-96)
#define CCP_FS_ERR_PROTOCOL             (-97)
#define CCP_FS_ERR_HANDLE_NULL          (-98)
#define CCP_FS_ERR_API_BLOCK            (-99)

#define CCP_FS_TYPE_NONE                1
#define CCP_FS_TYPE_STRING              2
#define CCP_FS_TYPE_LIST                3
#define CCP_FS_TYPE_SET                 4
#define CCP_FS_TYPE_HASH                5

#define CCP_FS_SERVER_MASTER            1
#define CCP_FS_SERVER_SLAVE             2

#define CCP_FS_VERSION_STRING_SIZE      32
#define CCP_FS_MULTIPLEXING_API_SIZE    16
#define CCP_FS_USED_MEMORY_HUMAN_SIZE   32

/**/
/* for FS HA */
/**/
#define CCP_FS_MAX						10
#define CCP_FS_MASTER					1
#define CCP_FS_SLAVE					2

#define CCP_FS_API_BLOCK				2
#define CCP_FS_API_OK                   1

/**/
/* for REDIS Session Management*/
/**/
#define CCP_FS_MAX_SESSION				2048
	///////////////////
#define CCP_FS_MAX_FD   				3096

#define CCP_FS_HA					    2
#define CCP_FS_STANDALONE				1

/**/
/* for Handle type(REDIS/sREDIS Management*/
/**/
#define CCP_FS_REDIS				    1
#define CCP_FS_sREDIS				    2

/**/
/* for FS HA Mode */
/**/
#define CCP_FS_INTERNAL				    1
#define CCP_FS_EXTERNAL                 2

/**/
/* for FS Locale Mode(2013.03.13), by icbilly */
/**/
#define CCP_FS_LOCAL					1
#define CCP_FS_REMOTE					2

/**/
/* for FS HA, Repl State */
/**/
#define CCP_FS_STATE_NULL               0
#define CCP_FS_STATE_BGSAVE             1
#define CCP_FS_STATE_SEND_BULK          2
#define CCP_FS_STATE_ONLINE             3

#define CCP_FS_MASTER_LINK_DOWN         0
#define CCP_FS_MASTER_LINK_UP           1

/**/
/* for FS API Default Timeout(2013.03.13), by icbilly */
/**/
#define CCP_FS_API_DEFAULT_TIMEOUT      1000	// unit (msec)

///////////////////////////////////////////////////////////////////////////////////////////////////
// Add by icbilly, 2014.02.24
// clay alarm notification when Redis session disconnect/connect
#define 	ALARM_A9605_FS_API_SESSION	    "A9605"
#define 	ALARM_A9605_FS_API_COMPONENT	"Redis Session #%0d-#%0d"
#define     ALARM_A9605_FS_API_TEXT	        "FS Redis Session Disconnect"

// Add by icbilly, 2014.04.01
// clay Info notification when Redis session disconnect/connect
#define 	ALARM_S9605_FS_API_SESSION	    "S9605"
#define     ALARM_S9605_FS_API_COMPONENT    "Redis Session #%0d-#%0d"     // Useless code in FS API v2.5.0
#define     ALARM_S9605_FS_API_TEXT	        "FS Redis Session Disconnect" // Useless code in FS API v2.5.0

/**/
/* for FS API error count check, by icbilly, 2014.08.27 */
/**/
#define     API_ERROR_CHECK_INTERVAL    10	    // seconds
#define     API_ERROR_CHECK_COUNT       1000	// counts  gn_ErrorLimitCount가 지정할 수 있는 최대값 

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _ccp_fs_buffer {
	char  *data;
	int    idx;
	int    len;
	int    size;
} redis_buffer;

typedef struct _ccp_fs_multibulk { 
	char  **bulks; 
	int   *idxs;
	int    size;
	int    len; 
} redis_multibulk;

typedef struct _ccp_fs_reply {
	int    integer;
	char  *line;
	char  *bulk;
	redis_multibulk multibulk;
} redis_reply;

typedef struct {
	int					handle;
	struct epoll_event	pool[CCP_FS_MAX_FD];
	struct epoll_event	evt;
} FS_EPOLL_T;

/**
 *  A structure to REDIS connection handle context
 */
typedef struct _ccp_fs_redis * sREDIS; /* Single Redis */
typedef struct _ccp_fs_redis   tREDIS; /* Single Redis */
typedef struct _ccp_fs_redis {
	struct {
		int major;
		int minor;
		int patch;
	} version;

	int             fd;
	char            *ip;
	int             port;
	int             timeout;
	FS_EPOLL_T		epoll;
	redis_buffer    buf;
	redis_reply     reply;
	int             error;
	int             role;

	int             nApiState;     // CCP_FS_API_BLOCK(2) / CCP_FS_API_OK(1)
	int				nHndlIndex;    // REDIS Handle Index (0~CCP_FS_MAX_SESSION), Add by icbilly, 2014.02.24
	int				nHndlSubIndex; // REDIS Handle Sub Index (0~n), Add by icbilly, 2014.02.24

	int             nSredisApiErrorCnt; // FS API Error Count per API session, 2014.08.12
} redis_redis;

/**/
/* HA Redis Handle for Master/Slave */
/**/
typedef struct _ccp_fs_ha_redis * REDIS;  /* HA Redis Handle for Master/Slave */
typedef struct _ccp_fs_ha_redis {
	int      		nCount;	 	   // redis handle connecting count
	int      		nConCount;	   // redis handle connected  count
	int		 		nFsMode;       // Internal: CCP_FS_INTERNAL(1) / External: CCP_FS_EXTERNAL(2)
	int      		nReadRedis;    // no use in API

	int             nApiErrorCnt;  // FS API Error Count per API session, 2014.08.12
	pthread_mutex_t stApiMutex;    // FS API Error Count mutex, 2014.08.27

	redis_redis *	redis_hndl;
} fs_ha_redis;

/**/
/* for FS REDIS handle session management table */
/**/
typedef struct _ccp_fs_redis_tbl tblREDIS;  // REDIS Session Management Table
typedef struct _ccp_fs_redis_tbl {
	int     nCount;
    int     nApiErrorSentFlag;              // FS API Error count alarm sent flag, by icbilly, 2014.08.27
    int     nApiErrorCntSum;                // FS API Error count total (all API session), by icbilly, 2014.08.27
	time_t  nTimes;                         // FS API internal check (default 10 seconds)

	void *	pREDIS[CCP_FS_MAX_SESSION+1];   // CCP_FS_MAX_SESSION(2048)
} fs_redis_tbl;

/*
===================================
 REDIS INFORMATION 
----------------------------------*/
typedef struct _ccp_fs_info {
  /* svr info */
  char            	redis_version[CCP_FS_VERSION_STRING_SIZE];
  int             	arch_bits;
  char            	multiplexing_api[CCP_FS_MULTIPLEXING_API_SIZE];
  long            	process_id;
  long            	uptime_in_seconds;
  long            	uptime_in_days;
  char   		  	redis_mode[20];

  /* clients */
  int             	connected_clients;
  int             	blocked_clients;
  int  				client_longest_output_list;
  int   			client_biggest_input_buf;

  /* replication */
  int             	role;
  char              master_host[24];               // replication master IP,   Add by icbilly, 2014.02.25
  int				master_port;                   // replication master port, Add by icbilly, 2014.02.25
  int             	connected_slaves;
  int               slaves_repl_status[CCP_FS_MAX];// slave replication status

  char              slaves_IP[CCP_FS_MAX][24];     // slave IP
  int               master_link_status;		       // replication master link status
  int               master_sync_left_bytes;		   // replication left bytes
  int               master_sync_in_progress;	   // replication status, 1: syncing, 0: no-syncing
  long              master_repl_offset;            // Replication Offset compare
  long              repl_offset_at_master[CCP_FS_MAX];// Replication Offset compare
  int               repl_lag[CCP_FS_MAX];          // Replication lag
  long              slave_repl_offset;             // Replication Offset compare

  /* memory */
  unsigned long   	used_memory;
  char            	used_memory_human[CCP_FS_USED_MEMORY_HUMAN_SIZE];
  unsigned long   	used_memory_peak;
  char            	used_memory_peak_human[CCP_FS_USED_MEMORY_HUMAN_SIZE];
  unsigned long		max_memory;

  long long       	rdb_changes_since_last_save;
  int             	rdb_bgsave_in_progress;
  long            	rdb_last_save_time;
  int             	rdb_last_bgsave_status;
  int             	rdb_last_bgsave_time_sec;

  int             	aof_rewrite_in_progress;
  int				aof_enabled;
  int				aof_last_rewrite_time_sec;
  int				aof_last_bgrewrite_status;
  long				aof_current_size;
  long				aof_base_size;
  int				aof_delayed_fsync;

  long long       	total_connections_received;
  long long       	total_commands_processed;
  long long       	expired_keys;
  unsigned long   	hash_max_zipmap_entries;
  unsigned long   	hash_max_zipmap_value;
  long            	pubsub_channels;
  unsigned int    	pubsub_patterns;
  int             	vm_enabled;
  int	  			dbsize;   
  
  /* stats */
  long long			ops_per_sec;  
} REDIS_INFO;

typedef struct _svr_info
{
  char              redis_version[CCP_FS_VERSION_STRING_SIZE];
  int               arch_bits;
  char              multiplexing_api[CCP_FS_MULTIPLEXING_API_SIZE];
  long              process_id;
  long              uptime_in_seconds;
  long              uptime_in_days;
  char              redis_mode[20];
  int  				dbsize;
} SVRINFO;

typedef struct _mem_info
{
  unsigned long     used_memory;
  char              used_memory_human[CCP_FS_USED_MEMORY_HUMAN_SIZE];
  unsigned long     used_memory_peak;
  char              used_memory_peak_human[CCP_FS_USED_MEMORY_HUMAN_SIZE];
} MEMINFO;

typedef struct _rep_info
{
  int               role;
  int               connected_slaves;
  int   			connected_clients;
  int               master_link_status;
  int               master_sync_left_bytes;
  int               master_sync_in_progress;
} REPLINFO;

typedef enum _cr_aggregate {
  NONE,
  SUM, 
  MIN,
  MAX
} REDIS_AGGREGATE;

typedef struct {
	size_t field_size;
	char   *field;
	size_t val_size;
	char   *val;
} HASH_FV;

typedef struct {
	size_t key_size;
	char   *key;
	size_t val_size;
	char   *val;
} KEYVALV;

typedef struct {
	size_t val_size;
	char   *val;
} VALUES;

/*
****************************************************************************************************
* Master Redis ����
***************************************************************************************************/
typedef struct {
	char	ip[28];
	int  	port;
} MASTER_INFO;

/*
****************************************************************************************************
* Master Redis ����
***************************************************************************************************/
typedef struct {
	char	ip[28];
	int  	port;
	char  	VMName[128];
} MASTER_INFO_HOST;

/*
****************************************************************************************************
* for FS HA
***************************************************************************************************/
typedef struct {
	int    	nStatus;	// Redis Master, Slave flag, CCP_FS_MASTER(1), CCP_FS_MASTER(0)
	char   	szIp[24];
	char   	szIpReal[24];
	int    	nPort;
	sREDIS	redis_hndl;
} IP_INFO;

typedef struct {
	int		 nCount;
	IP_INFO  iptbl[CCP_FS_MAX];
} FS_IP_INFO;

/*
****************************************************************************************************
* for FS HA Multiple Watcher Table
***************************************************************************************************/
#define MAX_WATCHER_TBL	  20

typedef struct {
	char ip[28];
	int  port;
} WATCHER_IP_INFO;

typedef struct {
	int  			nCount;
	WATCHER_IP_INFO wtbl[CCP_FS_MAX];
} WATCHER_TABLE;

/*
***************************************************************************************************
* Global Variables (defined in ccp_fs_api.c)
**************************************************************************************************/

extern int      gn_master_check_timeout_in; // Add by icbilly, 2014.08.12, for external FS API
extern int      gn_thread_check_timeout_in; // Add by icbilly, 2014.08.12, for internal FS API
                                            // for divide(gn_master_check_timeout & gn_thread_check_timeout)

extern int      gn_master_check_timeout_ex; // Add by icbilly, 2014.08.26, for external FS API
extern int      gn_thread_check_timeout_ex; // Add by icbilly, 2014.08.26, for external FS API

extern int      gn_ha_close_flag_ex; 		// 0	
extern int		gn_ha_close_flag_in;		// 0 CCP_FS_STANDALONE(0), CCP_FS_HA(1)
extern int      gn_master_is_duplicated_ex;	// CCP_FS_API_OK;
extern int      gn_master_is_duplicated_in;	// CCP_FS_API_OK;
extern int      gn_ha_mode;
extern tblREDIS gx_tbl_redis_ex;  			// Redis handle(REDIS) for session management
extern tblREDIS gx_tbl_redis_in; 			// Redis handle(REDIS) for session management
extern int		gn_watcher_port;			// 6310(Watcher Port�� Process���۽� ��ȣ�� �״�� ���)

extern MASTER_INFO	gx_master_info_ex;		// defined in ccp_fs_api_ex.c
extern MASTER_INFO	gx_master_info_in;		// defined in ccp_fs_api_in.c

extern struct timeval		gx_lastcommand_time;	//redis command를 마지막으로 한 시간

extern int		gn_ErrorLimitCount; //알람이 발생하는 에러의 개수
/*
***************************************************************************************************
* Global Function
**************************************************************************************************/

/*
========================
 INTERNAL Mode Function
-----------------------*/
int	   	ccp_fs_internal_ha_init(int nPort, int timeout, FS_IP_INFO *pfs_ip_info);
REDIS  	ccp_fs_internal_ha_connect(FS_IP_INFO l_fs_ip_info, int timeout);
void    ccp_fs_internal_ha_free();

/*
========================
 EXTERNAL Mode Function
-----------------------*/
int    	ccp_fs_ha_init(WATCHER_TABLE stWatcherInfo, int timeout, FS_IP_INFO *pfs_ip_info);
REDIS  	ccp_fs_ha_connect(FS_IP_INFO l_fs_ip_info, int timeout);
void    ccp_fs_external_ha_free();


sREDIS 	ccp_fs_init(void);
sREDIS 	redis_new(void);
REDIS  	ccp_fs_connect               (const char *host, int port, int timeout);
sREDIS 	ccp_fs_connect_sredis        (const char *host, int port, int timeout, int locale);  // **
sREDIS 	ccp_fs_connect_sredis_fd     (sREDIS orhnd, const char *host, int port, int timeout);  // **

REDIS  	ccp_fs_connect_uds           (int timeout);
sREDIS 	ccp_fs_connect_sredis_uds    (int timeout);	                // Add by icbilly, 2014.03.24, for UDS socket  // **
sREDIS 	ccp_fs_connect_sredis_uds_fd (sREDIS orhndl, int timeout);	// Add by icbilly, 2014.03.24, for UDS socket  // **

sREDIS 	ccp_fs_connect_watcher       (const char *host, int port, int timeout);  // **
int    	ccp_fs_query_master          (sREDIS rhnd, MASTER_INFO *master_info);
int    	ccp_fs_query_master_host     (sREDIS rhnd, MASTER_INFO_HOST *master_info); // Add by icbilly, 2014.04.05
void  	ccp_fs_delete		         (sREDIS rhnd);
void  	ccp_fs_settimeout	         (sREDIS rhnd, int timeout);
void  	ccp_fs_close		         (sREDIS rhnd);
int   	ccp_fs_get_role		         (sREDIS rhnd);
void 	ccp_fs_settimeouts	         (REDIS rhnd, int timeout);
void  	ccp_fs_ha_free		         (REDIS rhnd);
void  	ccp_fs_free			         (REDIS rhnd);
void    _free_ha_data                (sREDIS rhnd);

/* if a function call returns error it is _possible_ that the Redis server
 * replied with an error message. It is returned by this function.
 */
char* ccp_fs_errorreply	(sREDIS rhnd);
int   ccp_fs_auth		(REDIS rhnd, const char *password);
int   ccp_fs_ping		(REDIS rhnd);

/* 
 * Commands operating on all the kind of values
 */
int  ccp_fs_exists		(REDIS rhnd, const char *key);
int  ccp_fs_del			(REDIS rhnd, const char *key);
int  ccp_fs_type		(REDIS rhnd, const char *key);
int  ccp_fs_keys		(REDIS rhnd, const char *pattern, char ***keyv);
int  ccp_fs_randomkey	(REDIS rhnd, char **key);
int  ccp_fs_rename		(REDIS rhnd, const char *key, const char *new_key_name);
int  ccp_fs_renamenx	(REDIS rhnd, const char *key, const char *new_key_name);
int  ccp_fs_dbsize		(REDIS rhnd);
int  ccp_fs_expire		(REDIS rhnd, const char *key, int secs);
int  ccp_fs_expireat	(REDIS rhnd, const char *key, time_t timestamp);
int  ccp_fs_ttl			(REDIS rhnd, const char *key);
int  ccp_fs_select		(REDIS rhnd, int index);
int  ccp_fs_move		(REDIS rhnd, const char *key, int index);
int  ccp_fs_flushdb		(REDIS rhnd);
int  ccp_fs_flushall	(REDIS rhnd);
int ccp_fs_sort			(REDIS rhnd, const char *query, char ***elementv);

/* 
 * Commands operating on string values 
 */
int  ccp_fs_set			(REDIS rhnd, const char *key, const char *val);
int  ccp_fs_setb		(REDIS rhnd, const char *key, const char *val, size_t val_size);
int  ccp_fs_setv		(REDIS rhnd, const char *key, size_t key_len, const char *val, size_t val_len);
int  ccp_fs_get			(REDIS rhnd, const char *key, char **val);
int  ccp_fs_getset		(REDIS rhnd, const char *key, const char *set_val, char **get_val);
int  ccp_fs_mget		(REDIS rhnd, int keyc, const char **keyv, char ***valv);
int  ccp_fs_mset		(REDIS rhnd, int keyc, const char **keyv, const char **valv);
int  ccp_fs_msetb		(REDIS rhnd, int keyc, KEYVALV *keyvalv);
int  ccp_fs_setnx		(REDIS rhnd, const char *key, const char *val);
int  ccp_fs_incr		(REDIS rhnd, const char *key, int *new_val);
int  ccp_fs_incrby		(REDIS rhnd, const char *key, int incr_val, int *new_val);
int  ccp_fs_decr		(REDIS rhnd, const char *key, int *new_val);
int  ccp_fs_decrby		(REDIS rhnd, const char *key, int decr_val, int *new_val);
int  ccp_fs_append		(REDIS rhnd, const char *key, const char *val);
int  ccp_fs_substr		(REDIS rhnd, const char *key, int start, int end, char **substr);

/*
 * Commands operating on lists 
 */
int  ccp_fs_rpush		(REDIS rhnd, const char *key, const char *element);
int  ccp_fs_rpushb		(REDIS rhnd, const char *key, int valc, VALUES *valv);
int  ccp_fs_lpush		(REDIS rhnd, const char *key, const char *element);
int  ccp_fs_lpushb		(REDIS rhnd, const char *key, int valc, VALUES *valv);
int  ccp_fs_llen		(REDIS rhnd, const char *key);
int  ccp_fs_lrange		(REDIS rhnd, const char *key, int start, int range, char ***elementv);
int  ccp_fs_ltrim		(REDIS rhnd, const char *key, int start, int end);
int  ccp_fs_lindex		(REDIS rhnd, const char *key, int index, char **element);
int  ccp_fs_lset		(REDIS rhnd, const char *key, int index, const char *element);
int  ccp_fs_lrem		(REDIS rhnd, const char *key, int count, const char *element);
int  ccp_fs_lpop		(REDIS rhnd, const char *key, char **val);
int  ccp_fs_rpop		(REDIS rhnd, const char *key, char **val);

/*
 * Commands operating on sets 
 */
int  ccp_fs_sadd		(REDIS rhnd, const char *key, const char *member);
int  ccp_fs_srem		(REDIS rhnd, const char *key, const char *member);
int  ccp_fs_spop		(REDIS rhnd, const char *key, char **member);
int  ccp_fs_smove		(REDIS rhnd, const char *sourcekey, const char *destkey, const char *member);
int  ccp_fs_scard		(REDIS rhnd, const char *key);
int  ccp_fs_sismember	(REDIS rhnd, const char *key, const char *member);
int  ccp_fs_sinter		(REDIS rhnd, int keyc, const char **keyv, char ***members);
int  ccp_fs_sinterstore	(REDIS rhnd, const char *destkey, int keyc, const char **keyv);
int  ccp_fs_sunion		(REDIS rhnd, int keyc, const char **keyv, char ***members);
int  ccp_fs_sunionstore	(REDIS rhnd, const char *destkey, int keyc, const char **keyv);
int  ccp_fs_sdiff		(REDIS rhnd, int keyc, const char **keyv, char ***members);
int  ccp_fs_sdiffstore	(REDIS rhnd, const char *destkey, int keyc, const char **keyv);
int  ccp_fs_smembers	(REDIS rhnd, const char *key, char ***members);
int  ccp_fs_srandmember	(REDIS rhnd, const char *key, int cnt, char ***member);

/* 
 * Commands operating on sorted sets
 */
int  ccp_fs_zadd			(REDIS rhnd, const char *key, double score, const char *member);
int  ccp_fs_zrem			(REDIS rhnd, const char *key, const char *member);
int  ccp_fs_zincrby			(REDIS rhnd, const char *key, double incr_score, const char *member, double *new_score);
int  ccp_fs_zrank			(REDIS rhnd, const char *key, const char *member);
int  ccp_fs_zrevrank		(REDIS rhnd, const char *key, const char *member);
int  ccp_fs_zrange			(REDIS rhnd, const char *key, int start, int end, char ***elementv);
int  ccp_fs_zrevrange		(REDIS rhnd, const char *key, int start, int end, char ***elementv);
int  ccp_fs_zcard			(REDIS rhnd, const char *key);
int  ccp_fs_zscore			(REDIS rhnd, const char *key, const char *member, double *score);
int  ccp_fs_zremrangebyscore(REDIS rhnd, const char *key, double min, double max);
int  ccp_fs_zremrangebyrank	(REDIS rhnd, const char *key, int start, int end);
int  ccp_fs_zinterstore		(REDIS rhnd, const char *dkey, int keyc, const char **keyv, const int *weight, REDIS_AGGREGATE aggre);
int  ccp_fs_zunionstore		(REDIS rhnd, const char *dkey, int keyc, const char **keyv, const int *weight, REDIS_AGGREGATE aggre);

/* 
 * Commands operating on hashes
 */
int  ccp_fs_hset			(REDIS rhnd, const char *key, const char *field, const char *value);
int  ccp_fs_hget			(REDIS rhnd, const char *key, const char *field, char **val);
int  ccp_fs_hkeys			(REDIS rhnd, const char *key, char ***fieldv);
int  ccp_fs_hlen			(REDIS rhnd, const char *key);
int  ccp_fs_hmget			(REDIS rhnd, const char *key, int fieldc, const char **fieldv, char ***valv);
int  ccp_fs_hgetall			(REDIS rhnd, const char *key, char ***valv);
int  ccp_fs_hexists			(REDIS rhnd, const char *key, const char *field);
int  ccp_fs_hdel			(REDIS rhnd, const char *key, int fieldc, const char **fieldv);
int  ccp_fs_hvals			(REDIS rhnd, const char *key, char ***valv);
int  ccp_fs_hmset			(REDIS rhnd, const char *key, int fieldc, const char **fieldv, const char **valv);
int  ccp_fs_hmsetb			(REDIS rhnd, const char *key, int fieldc,  HASH_FV *hash_fv);
int  ccp_fs_hincrby			(REDIS rhnd, const char *key, const char *field, int incr, int *new_val);
int  ccp_fs_hsetnx			(REDIS rhnd, const char *key, const char *field, const char *value);

/* 
 * INFO
 */
int  ccp_fs_info			(REDIS rhnd, REDIS_INFO *info);
int  ccp_fs_ma_info			(REDIS rhnd, REDIS_INFO *info);
int  ccp_fs_sl_info			(REDIS rhnd, REDIS_INFO *info);
int  ccp_fs_info_svr		(REDIS rhnd, SVRINFO *svrinfo, int SVR);
int  ccp_fs_info_memory		(REDIS rhnd, MEMINFO *meminfo, int SVR);
int  ccp_fs_info_repl		(REDIS rhnd, REPLINFO *replinfo, int SVR);
long ccp_fs_memory_info		(REDIS rhnd, unsigned long *nCurrentMemory, unsigned long *nMaxMemory);
int  ccp_fs_repl_status		(REDIS rhnd, int *nNodeRole, long *nOffSet, REDIS_INFO *pInfo);
int  ccp_fs_config_set_aof      (REDIS rhnd, int flag);
int  ccp_fs_config_get_aof      (REDIS rhnd, int* flag);
int  ccp_fs_config_set_loglevel (REDIS rhnd, int flag);
int  ccp_fs_config_get_loglevel (REDIS rhnd, int* flag);

////////////////////////////////////////////////////////////////////////////
void ccp_fs_set_lastcommandtime();
//  defined at ccp_fs_api.c
void ccp_fs_set_lastcommandtime_ex(REDIS rhnd, sREDIS srhnd);

int  redisFormatCommandArgv	(char **target, int argc, const char **argv, const size_t *argvlen);

/* etc
 *
 */

int  credis_get_fd				(sREDIS rhnd);		// Add by icbilly, 2013.12.17
int  ccp_fs_save            	(REDIS rhnd);		// Add by icbilly, 2014.03.07
int  ccp_fs_bgsave          	(REDIS rhnd);		// Add by icbilly, 2014.03.07
int  ccp_fs_lastsave        	(REDIS rhnd);		// Add by icbilly, 2014.03.07
void ccp_fs_set_errorlimitcount	(int limitCount);	// Add by jyy,     2014.09.03
// -----------------------------------------------------------------------------------------------
// Add by icbilly, 2013.12.18, for Alarm Send
// Move code to here(ccp_fs_api.c) from watcher_lib.c by icbilly, 2014.02.24, for Alarm Send
// -----------------------------------------------------------------------------------------------
void  _alarm_send(char *szCode, char *szComponent, char *szAlarmInfo, int nOnOff);

/*
 * Watcher
 */
int  ccp_fs_watcher_query_iplist	(sREDIS rhnd, FS_IP_INFO *fs_ip_info);
int  ccp_fs_watcher_mode_query		(sREDIS rhnd, FS_IP_INFO *fs_ip_info);
int  ccp_fs_master_through_watcher	(sREDIS rhnd);
int  mWatcherSleep					(int nTimeout);	// mili-seconds

////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Internal 
//
////////////////////////////////////////////////////////////////////////////////////////////////////


#define FS_LOG_LVL_ALL      0xfff
#define FS_LOG_LVL_ERR      0x001
#define FS_LOG_LVL_WARN     0x002
#define FS_LOG_LVL_MAINT    0x004
#define FS_LOG_LVL_TRACE    0x008
#define FS_LOG_LVL_DEBUG    0x010

void fsLog(const char *func, int level, const char *azp_format, ...);
void fsLogOn(char *file_name, int level);
void fsLogOff();

#define xdebug(...)                 	fsLog(__func__, FS_LOG_LVL_DEBUG, __VA_ARGS__)
#define FS_DEBUG(LVL, ...)            	fsLog(__func__, LVL, __VA_ARGS__)
#define FS_DEBUG_ON(FILE_NAME, LVL)    	fsLogOn(FILE_NAME, LVL)
#define FS_DEBUG_OFF()                 	fsLogOff()

// REDIS PROTOCOL Prefix
#define REDIS_ERROR 	'-'
#define REDIS_INLINE 	'+'
#define REDIS_BULK 		'$'
#define REDIS_MULTIBULK '*'
#define REDIS_INT 		':'

//
#define REDIS_BUFFER_SIZE 		4096
#define REDIS_BUFFER_WATERMARK 	((REDIS_BUFFER_SIZE)/10+1)
#define REDIS_MULTIBULK_SIZE 	256

/*
***************************************************************************************************
* Internal Function 
**************************************************************************************************/
int 	sockCreate				 (const char *host, int port, struct sockaddr_in *addr);
int     sockConnect				 (int fd, struct sockaddr_in *addr, int timeout);

int 	sockCreateAndConnectEpoll(const char *host, int port, int timeout, FS_EPOLL_T *epoll);
int 	sockConnectEpoll		 (int fd, struct sockaddr_in *addr, int timeout, FS_EPOLL_T *epoll);
int 	sockConnectEpoll_uds	 (int fd, struct sockaddr_un *addr, int timeout, FS_EPOLL_T *epoll);
int 	sockReadableEpoll  		 (int fd, int timeout, FS_EPOLL_T *epoll);
int 	sockWritableEpoll  		 (int fd, int timeout, FS_EPOLL_T *epoll);
int 	sockSendEpoll			 (int fd, int msecs, char *buf, int size, FS_EPOLL_T *epoll);
int 	sockRecvEpoll			 (int fd, int msecs, char *buf, int size, FS_EPOLL_T *epoll);

void 	xAddStr					(char *azp_str, const char *azp_fmt,...);

sREDIS	isMasterRedis(REDIS rhndl);
sREDIS	isSlaveRedis (REDIS rhndl);
int     redis_sendfandreceive             (REDIS rhnd, sREDIS srhnd, char recvtype, const char *format, ...);
int     redis_sendfandreceive_no_errcount (REDIS rhnd, sREDIS srhnd, char recvtype, const char *format, ...); // Add 2014.09.03
int     redis_sendfandreceive_sredis      (REDIS rhnd, sREDIS srhnd, char recvtype, const char *format, ...);

////////////////////////////////////////////////////////////////////////////////////////////////////
//      API for FS alarm management by icbilly, 2014.08.27
////////////////////////////////////////////////////////////////////////////////////////////////////
int     ccp_fs_check_REDIS_handle               (REDIS rhnd, tREDIS **trhnd);
int     ccp_fs_check_REDIS_handle_no_errcount   (REDIS rhnd, tREDIS **trhnd);
void    ccp_fs_set_api_error_sREDIS (REDIS rhnd, sREDIS srhnd, int nRet);
void    ccp_fs_set_api_error_sREDIS2(REDIS rhnd, sREDIS srhnd, int nRet);
void    ccp_fs_set_api_error_REDIS  (REDIS rhnd, sREDIS srhnd);
void    ccp_fs_set_api_error        (REDIS rhnd);
void    ccp_fs_clear_api_error      (REDIS rhnd);

#ifndef _PRIVATE ////////////////////////////////////////////////////////////////////////////////////
#endif ////////////////////////////////////////////////////////////////////////////////// <_DOXYGEN>

#ifdef __cplusplus
}
#endif

#endif /* __CCP_FS_H */
