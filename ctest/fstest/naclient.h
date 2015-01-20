
#ifndef __CLAYC__
#define __CLAYC__

#include <stdio.h>

#define CLI_EVENT_PROPERTY_CHANGED		1
#define CLI_EVENT_SAVE_START			2
#define CLI_EVENT_SAVE_END				3
#define CLI_EVENT_SERVICE_START			4
#define CLI_EVENT_SERVICE_STOP			5

#define AGENT_EVENT_ACCEPTED			1500
#define AGENT_EVENT_REJECTED			1501
#define AGENT_EVENT_DUPLICATED			1502

#define VRRP_EVENT_ACTIVE				9801
#define VRRP_EVENT_DEACTIVE				9802
#define VRRP_EVENT_ACCEPTED				9803
#define VRRP_EVENT_REJECTED				9804

#define VRRP_ACTIVE						1
#define VRRP_NOTUSE						0

#define PRC_TRAP						1
#define PRC_NO_TRAP						0
#define PRC_RESTART						1
#define PRC_NONE						0

struct __oid
{
	int oid[100];
	int length;
};

typedef struct __oid OID;
typedef char* (*oid_handler)( OID o );
typedef int (*oid_set_handler)( OID o, const char* value );
typedef char* (*poll_handler)( const char* line , int length );
typedef void (*event_handler)( int event , const char* section , const char* key , const char* value );
typedef void (*adv_poll_handler)( void* handler, const int argv, const char** argc, const char* value );
//typedef int (*init_handler)();

#ifdef __cplusplus
extern "C"
{
#endif
// init & close
// for snmp

const char* clayc_version();
/* client의 버전을 가져온다. */

void clayc_init();
/* client를 초기화 한다. 이것이 없는경우 segmentaion falut를 유발시킨다. */

void clayc_start();
/* client process 를 시작한다. */

void clayc_wait();
/* client procces 가 시작될때까지 기다린다.*/

void clayc_close();
/* 종료시킨다. */

void clayc_reboot();
/* 종료 시그널 없이 종료시킨다. */

void clayc_takeover();

int clayc_ensure_connected();
/* agent와 연결된 상태이면 0 그렇지 않으면 -1 */

void clayc_debug_on();
/* debug 내용을 stdout으로 출력한다. */

void clayc_debug_off();
/* debug를 하지 않는다. */

void clayc_active_vrrp( int act );
/*
	vrrp option을 지정한다.
	VRRP_ACTIVE : VRRP를 사용한다.
	VRRP_NOTUSE : VRRP를 사용하지 않는다.
*/

void clayc_do_not_restart();
/* 재시작 금지 */

void clayc_thread_max( int max );
/* 최대 스레드 수 */

int clayc_oid_matched( OID a , OID b );
/*
	0: 같다.
	-: a가 먼저
	+: b가 먼저
*/
int clayc_oid_nmatched( OID a , OID b , int length );
/*
	0: 같다.
	-: a가 먼저
	+: b가 먼저
*/

void clayc_snmp_handler( oid_handler handle  );
/* 핸들러를 설정한다. */

void clayc_snmp_set_handler( oid_set_handler handle  );
/* SET 핸들러를 설정한다. */

void clayc_set_processname( const char* name );
/* 프로세스 ID를 정의한다. 반드시 필요하다. */

// 1.5.2 추가사항.
void clayc_set_processversion( const char* ver );
/* 프로세스의 버전을 정의한다. 반드시 필요하다. */

void clayc_wait_for_agent();
/* agnet와 연결될때까지 기다린다. */


void clayc_v1_trap( const char* community , const char* enterprise , const char* agentaddr , int type , int spectype , const char* oids, const char* oidType , const char* value );
/* snmp v1 trap을 발생시킨다. */

void clayc_v2_notification( const char* community, const char* oidvalue, const char* oids, const char* oidtypes, const char* oidvalues );
/* snmp v2 notification을 발생시킨다. */

#ifdef __cplusplus
void clayc_send_trace( const char* traceid , const char* srcip , int srcport , const char* destip , int destport , const char* msgbody, const char* tracetime, const char* extensions=NULL );
#else
void clayc_send_trace( const char* traceid , const char* srcip , int srcport , const char* destip , int destport , const char* msgbody, const char* tracetime, const char* extensions );
#endif

/* trace V2 */
void clayc_send_trace_v2( const char* traceid, const char* nodeid, const char* srcaddr, const char* destaddr, const char* msgname, const char* msgId, int msglength, const char* msgbody, const char* prototype, const char* tracetime );

/* clay alarm notification */
#define ALARM_CLEAR	1
#define ALARM_OCCUR	0
void clayc_alarm_notification( const char* code, const char* nodeid, const char* subcomponent, const char *detailinfo, int clear );

/* message trace 전송 */

// for event
void clayc_event_handler( event_handler handler );

/*
int clayc_polling_write( void* channel, const char* var );
	0: normal
	-1: handler not set
*/

void clayc_polling_handler( poll_handler handle  );
/*
	polling 핸들러를 설정한다.
*/

void clayc_adv_polling_handler( adv_poll_handler handle  );
/*
	advanced polling 핸들러를 설정한다.
*/

// for configuration
int clayc_set_configfile( const char* filename );
/*
	0 : 정상
	-1 : file not found
	-2 : io error
	-3 : format error
	-4 : unknown error
*/

int clayc_get_property( const char* section, const char* key, char* buffer, int length );
/*
	설정값을 가져온다.
	0보다 크면 정상.
	-1이면 buffer too short
	-2이면 내부적으로 오류 발생
	-3이면 no such property
*/

int clayc_get_table_records_count( const char* section, const char* table );
/*
	테이블 레코드 수를 가져온다.
	0보다 작으면 해당 항목 없음
*/


int clayc_get_table_property( const char* section, const char* table, const char* column, int row, char* buffer, int length );
/*
	테이블 값을 가져온다.
	0보다 크면 정상.
	-1이면 buffer too short
	-2이면 내부적으로 오류 발생
	-3이면 out of index.
*/


/*
void clayc_init_handler( init_handler handle );
	시스템 INIT를 별도로 수행한다.
*/

#ifdef __cplusplus
void clayc_process_monitor( const char* processid, int trap=PRC_NO_TRAP, int restart=PRC_NONE );
#else
void clayc_process_monitor( const char* processid, int trap, int restart );
#endif

int clayc_poll_write( void* handler, char* buffer, const char* format, ...  );
/*
	write 결과를 반납한다.
	0보다 작으면 오류
*/







const char* nagent_client_version();
/* client의 버전을 가져온다. */

void nagent_client_init();
/* client를 초기화 한다. 이것이 없는경우 segmentaion falut를 유발시킨다. */

void nagent_client_start();
/* client process 를 시작한다. */

void nagent_client_wait();
/* client procces 가 시작될때까지 기다린다.*/

void nagent_client_close();
/* 종료시킨다. */

void nagent_client_reboot();
/* 종료 시그널 없이 종료시킨다. */

int nagent_client_ensure_connected();
/* agent와 연결된 상태이면 0 그렇지 않으면 -1 */

void nagent_client_debug_on();
/* debug 내용을 stdout으로 출력한다. */

void nagent_client_debug_off();
/* debug를 하지 않는다. */

void nagent_client_active_vrrp( int act );
/*
	vrrp option을 지정한다.
	VRRP_ACTIVE : VRRP를 사용한다.
	VRRP_NOTUSE : VRRP를 사용하지 않는다.
*/

void nagent_client_takeover();

void nagent_client_do_not_restart();
/* 재시작 금지 */

void nagent_client_thread_max( int max );
/* 최대 스레드 수 */

int nagent_client_oid_matched( OID a , OID b );
/*
	0: 같다.
	-: a가 먼저
	+: b가 먼저
*/
int nagent_client_oid_nmatched( OID a , OID b , int length );
/*
	0: 같다.
	-: a가 먼저
	+: b가 먼저
*/

void nagent_client_snmp_handler( oid_handler handle  );
/* 핸들러를 설정한다. */

void nagent_client_set_processname( const char* name );
/* 프로세스 ID를 정의한다. 반드시 필요하다. */

// 1.5.2 추가사항.
void nagent_client_set_processversion( const char* ver );
/* 프로세스의 버전을 정의한다. 반드시 필요하다. */

void nagent_client_wait_for_agent();
/* agnet와 연결될때까지 기다린다. */


void nagent_client_v1_trap( const char* community , const char* enterprise , const char* agentaddr , int type , int spectype , const char* oids, const char* oidType , const char* value );
/* snmp v1 trap을 발생시킨다. */

void nagent_client_v2_notification( const char* community, const char* oidvalue, const char* oids, const char* oidtypes, const char* oidvalues );
/* snmp v2 notification을 발생시킨다. */

// for event
void nagent_client_event_handler( event_handler handler );

/*
int nagent_client_polling_write( void* channel, const char* var );
	0: normal
	-1: handler not set
*/

void nagent_client_polling_handler( poll_handler handle  );
/*
	polling 핸들러를 설정한다.
*/

void nagent_client_adv_polling_handler( adv_poll_handler handle  );
/*
	advanced polling 핸들러를 설정한다.
*/

// for configuration
int nagent_client_set_configfile( const char* filename );
/*
	0 : 정상
	-1 : file not found
	-2 : io error
	-3 : format error
	-4 : unknown error
*/

int nagent_client_get_property( const char* section, const char* key, char* buffer, int length );
/*
	설정값을 가져온다.
	0보다 크면 정상.
	-1이면 buffer too short
	-2이면 내부적으로 오류 발생
	-3이면 no such property
*/

int nagent_client_get_table_records_count( const char* section, const char* table );
/*
	테이블 레코드 수를 가져온다.
	0보다 작으면 해당 항목 없음
*/


int nagent_client_get_table_property( const char* section, const char* table, const char* column, int row, char* buffer, int length );
/*
	테이블 값을 가져온다.
	0보다 크면 정상.
	-1이면 buffer too short
	-2이면 내부적으로 오류 발생
	-3이면 out of index.
*/
#ifdef __cplusplus
}

#endif

#endif
