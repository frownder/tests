
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
/* client�� ������ �����´�. */

void clayc_init();
/* client�� �ʱ�ȭ �Ѵ�. �̰��� ���°�� segmentaion falut�� ���߽�Ų��. */

void clayc_start();
/* client process �� �����Ѵ�. */

void clayc_wait();
/* client procces �� ���۵ɶ����� ��ٸ���.*/

void clayc_close();
/* �����Ų��. */

void clayc_reboot();
/* ���� �ñ׳� ���� �����Ų��. */

void clayc_takeover();

int clayc_ensure_connected();
/* agent�� ����� �����̸� 0 �׷��� ������ -1 */

void clayc_debug_on();
/* debug ������ stdout���� ����Ѵ�. */

void clayc_debug_off();
/* debug�� ���� �ʴ´�. */

void clayc_active_vrrp( int act );
/*
	vrrp option�� �����Ѵ�.
	VRRP_ACTIVE : VRRP�� ����Ѵ�.
	VRRP_NOTUSE : VRRP�� ������� �ʴ´�.
*/

void clayc_do_not_restart();
/* ����� ���� */

void clayc_thread_max( int max );
/* �ִ� ������ �� */

int clayc_oid_matched( OID a , OID b );
/*
	0: ����.
	-: a�� ����
	+: b�� ����
*/
int clayc_oid_nmatched( OID a , OID b , int length );
/*
	0: ����.
	-: a�� ����
	+: b�� ����
*/

void clayc_snmp_handler( oid_handler handle  );
/* �ڵ鷯�� �����Ѵ�. */

void clayc_snmp_set_handler( oid_set_handler handle  );
/* SET �ڵ鷯�� �����Ѵ�. */

void clayc_set_processname( const char* name );
/* ���μ��� ID�� �����Ѵ�. �ݵ�� �ʿ��ϴ�. */

// 1.5.2 �߰�����.
void clayc_set_processversion( const char* ver );
/* ���μ����� ������ �����Ѵ�. �ݵ�� �ʿ��ϴ�. */

void clayc_wait_for_agent();
/* agnet�� ����ɶ����� ��ٸ���. */


void clayc_v1_trap( const char* community , const char* enterprise , const char* agentaddr , int type , int spectype , const char* oids, const char* oidType , const char* value );
/* snmp v1 trap�� �߻���Ų��. */

void clayc_v2_notification( const char* community, const char* oidvalue, const char* oids, const char* oidtypes, const char* oidvalues );
/* snmp v2 notification�� �߻���Ų��. */

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

/* message trace ���� */

// for event
void clayc_event_handler( event_handler handler );

/*
int clayc_polling_write( void* channel, const char* var );
	0: normal
	-1: handler not set
*/

void clayc_polling_handler( poll_handler handle  );
/*
	polling �ڵ鷯�� �����Ѵ�.
*/

void clayc_adv_polling_handler( adv_poll_handler handle  );
/*
	advanced polling �ڵ鷯�� �����Ѵ�.
*/

// for configuration
int clayc_set_configfile( const char* filename );
/*
	0 : ����
	-1 : file not found
	-2 : io error
	-3 : format error
	-4 : unknown error
*/

int clayc_get_property( const char* section, const char* key, char* buffer, int length );
/*
	�������� �����´�.
	0���� ũ�� ����.
	-1�̸� buffer too short
	-2�̸� ���������� ���� �߻�
	-3�̸� no such property
*/

int clayc_get_table_records_count( const char* section, const char* table );
/*
	���̺� ���ڵ� ���� �����´�.
	0���� ������ �ش� �׸� ����
*/


int clayc_get_table_property( const char* section, const char* table, const char* column, int row, char* buffer, int length );
/*
	���̺� ���� �����´�.
	0���� ũ�� ����.
	-1�̸� buffer too short
	-2�̸� ���������� ���� �߻�
	-3�̸� out of index.
*/


/*
void clayc_init_handler( init_handler handle );
	�ý��� INIT�� ������ �����Ѵ�.
*/

#ifdef __cplusplus
void clayc_process_monitor( const char* processid, int trap=PRC_NO_TRAP, int restart=PRC_NONE );
#else
void clayc_process_monitor( const char* processid, int trap, int restart );
#endif

int clayc_poll_write( void* handler, char* buffer, const char* format, ...  );
/*
	write ����� �ݳ��Ѵ�.
	0���� ������ ����
*/







const char* nagent_client_version();
/* client�� ������ �����´�. */

void nagent_client_init();
/* client�� �ʱ�ȭ �Ѵ�. �̰��� ���°�� segmentaion falut�� ���߽�Ų��. */

void nagent_client_start();
/* client process �� �����Ѵ�. */

void nagent_client_wait();
/* client procces �� ���۵ɶ����� ��ٸ���.*/

void nagent_client_close();
/* �����Ų��. */

void nagent_client_reboot();
/* ���� �ñ׳� ���� �����Ų��. */

int nagent_client_ensure_connected();
/* agent�� ����� �����̸� 0 �׷��� ������ -1 */

void nagent_client_debug_on();
/* debug ������ stdout���� ����Ѵ�. */

void nagent_client_debug_off();
/* debug�� ���� �ʴ´�. */

void nagent_client_active_vrrp( int act );
/*
	vrrp option�� �����Ѵ�.
	VRRP_ACTIVE : VRRP�� ����Ѵ�.
	VRRP_NOTUSE : VRRP�� ������� �ʴ´�.
*/

void nagent_client_takeover();

void nagent_client_do_not_restart();
/* ����� ���� */

void nagent_client_thread_max( int max );
/* �ִ� ������ �� */

int nagent_client_oid_matched( OID a , OID b );
/*
	0: ����.
	-: a�� ����
	+: b�� ����
*/
int nagent_client_oid_nmatched( OID a , OID b , int length );
/*
	0: ����.
	-: a�� ����
	+: b�� ����
*/

void nagent_client_snmp_handler( oid_handler handle  );
/* �ڵ鷯�� �����Ѵ�. */

void nagent_client_set_processname( const char* name );
/* ���μ��� ID�� �����Ѵ�. �ݵ�� �ʿ��ϴ�. */

// 1.5.2 �߰�����.
void nagent_client_set_processversion( const char* ver );
/* ���μ����� ������ �����Ѵ�. �ݵ�� �ʿ��ϴ�. */

void nagent_client_wait_for_agent();
/* agnet�� ����ɶ����� ��ٸ���. */


void nagent_client_v1_trap( const char* community , const char* enterprise , const char* agentaddr , int type , int spectype , const char* oids, const char* oidType , const char* value );
/* snmp v1 trap�� �߻���Ų��. */

void nagent_client_v2_notification( const char* community, const char* oidvalue, const char* oids, const char* oidtypes, const char* oidvalues );
/* snmp v2 notification�� �߻���Ų��. */

// for event
void nagent_client_event_handler( event_handler handler );

/*
int nagent_client_polling_write( void* channel, const char* var );
	0: normal
	-1: handler not set
*/

void nagent_client_polling_handler( poll_handler handle  );
/*
	polling �ڵ鷯�� �����Ѵ�.
*/

void nagent_client_adv_polling_handler( adv_poll_handler handle  );
/*
	advanced polling �ڵ鷯�� �����Ѵ�.
*/

// for configuration
int nagent_client_set_configfile( const char* filename );
/*
	0 : ����
	-1 : file not found
	-2 : io error
	-3 : format error
	-4 : unknown error
*/

int nagent_client_get_property( const char* section, const char* key, char* buffer, int length );
/*
	�������� �����´�.
	0���� ũ�� ����.
	-1�̸� buffer too short
	-2�̸� ���������� ���� �߻�
	-3�̸� no such property
*/

int nagent_client_get_table_records_count( const char* section, const char* table );
/*
	���̺� ���ڵ� ���� �����´�.
	0���� ������ �ش� �׸� ����
*/


int nagent_client_get_table_property( const char* section, const char* table, const char* column, int row, char* buffer, int length );
/*
	���̺� ���� �����´�.
	0���� ũ�� ����.
	-1�̸� buffer too short
	-2�̸� ���������� ���� �߻�
	-3�̸� out of index.
*/
#ifdef __cplusplus
}

#endif

#endif
