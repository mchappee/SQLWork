#include <dlfcn.h>      // dlsym
#include <stdio.h>
#include <sys/types.h>

#include "oci.h"

static char *OCIHandleTypes[] = {"", "HTYPE_ENV", "HTYPE_ERROR", "HTYPE_SVCCTX", "HTYPE_STMT", "HTYPE_BIND", "HTYPE_DEFINE", "HTYPE_DESCRIBE", "HTYPE_SERVER", "HTYPE_SESSION", "HTYPE_TRANS", "HTYPE_COMPLEXOBJECT", "HTYPE_SECURITY", "HTYPE_SUBSCRIPTION", "HTYPE_DIRPATH_CTX", "HTYPE_DIRPATH_COLUMN_ARRAY", "HTYPE_DIRPATH_STREAM", "HTYPE_PROC"};
static char *OCIDescrTypes[] =  {"DTYPE_LOB", "DTYPE_SNAP", "DTYPE_RSET", "DTYPE_PARAM", "DTYPE_ROWID", "DTYPE_COMPLEXOBJECTCOMP", "DTYPE_FILE", "DTYPE_AQENQ_OPTIONS", "DTYPE_AQDEQ_OPTIONS", "DTYPE_AQMSG_PROPERTIES", "DTYPE_AQAGENT", "DTYPE_LOCATOR", "DTYPE_DATETIME", "DTYPE_INTERVAL", "DTYPE_AQNFY_DESCRIPTOR"};
static char *OCIAttrs[] =       {"", "FNCODE", "OBJECT", "NONBLOCKING_MODE", "SQLCODE", "ENV", "SERVER", "SESSION", "TRANS", "ROW_COUNT", "SQLFNCODE", "PREFETCH_ROWS", "NESTED_PREFETCH_ROWS", "PREFETCH_MEMORY", "NESTED_PREFETCH_MEMORY", "CHAR_COUNT", "PDSCL", "PDPRC", "PARAM_COUNT", "ROWID", "CHARSET", "NCHAR", "USERNAME", "PASSWORD", "STMT_TYPE", "INTERNAL_NAME", "EXTERNAL_NAME", "XID", "TRANS_LOCK", "TRANS_NAME", "HEAPALLOC", "CHARSET_ID", "CHARSET_FORM", "MAXDATA_SIZE", "CACHE_OPT_SIZE", "CACHE_MAX_SIZE", "PINOPTION", "ALLOC_DURATION", "PIN_DURATION", "FDO", "POSTPROCESSING_CALLBACK", "POSTPROCESSING_CONTEXT", "ROWS_RETURNED", "FOCBK", "IN_V_MODE", "LOBEMPTY", "SESSLANG", "VISIBILITY", "RELATIVE_MSGID", "SEQUENCE_DEVIATION", "CONSUMER_NAME", "DEQ_MODE", "NAVIGATION", "WAIT", "DEQ_MSGID", "PRIORITY", "DELAY", "EXPIRATION", "CORRELATION", "ATTEMPTS", "RECIPIENT_LIST", "EXCEPTION_QUEUE", "ENQ_TIME", "MSG_STATE", "AGENT_NAME", "AGENT_ADDRESS", "AGENT_PROTOCOL", "INVALID", "SENDER_ID", "ORIGINAL_MSGID", "QUEUE_NAME", "NFY_MSGID", "MSG_PROP", "NUM_DML_ERRORS", "DML_ROW_OFFSET", "DATEFORMAT", "BUF_ADDR", "BUF_SIZE", "DIRPATH_MODE", "DIRPATH_NOLOG", "DIRPATH_PARALLEL", "NUM_ROWS", "COL_COUNT", "STREAM_OFFSET", "SHARED_HEAPALLOC", "SERVER_GROUP", "MIGSESSION", "NOCACHE", "MEMPOOL_SIZE", "MEMPOOL_INSTNAME", "MEMPOOL_APPNAME", "MEMPOOL_HOMENAME", "MEMPOOL_MODEL", "MODES", "SUBSCR_NAME", "SUBSCR_CALLBACK", "SUBSCR_CTX", "SUBSCR_PAYLOAD", "SUBSCR_NAMESPACE", "PROXY_CREDENTIALS", "INITIAL_CLIENT_ROLES", "UNK", "NUM_COLS", "LIST_COLUMNS", "RDBA", "CLUSTERED", "PARTITIONED", "INDEX_ONLY", "LIST_ARGUMENTS", "LIST_SUBPROGRAMS", "REF_TDO", "LINK", "MIN", "MAX", "INCR", "CACHE", "ORDER", "HW_MARK", "TYPE_SCHEMA", "TIMESTAMP", "NUM_ATTRS", "NUM_PARAMS", "OBJID", "PTYPE", "PARAM", "OVERLOAD_ID", "TABLESPACE", "TDO", "LTYPE", "PARSE_ERROR_OFFSET", "IS_TEMPORARY", "IS_TYPED", "DURATION", "IS_INVOKER_RIGHTS", "OBJ_NAME", "OBJ_SCHEMA", "OBJ_ID"};
static char *OCICreds[] =       {"", "RDBMS", "EXT", "PROXY"};
static char *SQLTTypes[] =      {"", "SQLT_CHR", "SQLT_NUM", "SQLT_INT", "SQLT_FLT", "SQLT_STR", "SQLT_VNU", "SQLT_PDN", "SQLT_LNG", "SQLT_VCS", "SQLT_NON", "SQLT_RID", "SQLT_DAT", "INVALID(13)", "INVALID(14)", "SQLT_VBI", "INVALID(16)", "INVALID(17)", "INVALID(18)", "INVALID(19)", "INVALID(20)", "INVALID(21)", "INVALID(22)", "SQLT_BIN", "SQLT_LBI", "INVALID(25)", "INVALID(26)", "INVALID(27)", "INVALID(28)", "INVALID(29)", "INVALID(30)", "INVALID(31)", "INVALID(32)", "INVALID(33)", "INVALID(34)", "INVALID(35)", "INVALID(36)", "INVALID(37)", "INVALID(38)", "INVALID(39)", "INVALID(40)", "INVALID(41)", "INVALID(42)", "INVALID(43)", "INVALID(44)", "INVALID(45)", "INVALID(46)", "INVALID(47)", "INVALID(48)", "INVALID(49)", "INVALID(50)", "INVALID(51)", "INVALID(52)", "INVALID(53)", "INVALID(54)", "INVALID(55)", "INVALID(56)", "INVALID(57)", "INVALID(58)", "INVALID(59)", "INVALID(60)", "INVALID(61)", "INVALID(62)", "INVALID(63)", "INVALID(64)", "INVALID(65)", "INVALID(66)", "INVALID(67)", "SQLT_UIN", "INVALID(69)", "INVALID(70)", "INVALID(71)", "INVALID(72)", "INVALID(73)", "INVALID(74)", "INVALID(75)", "INVALID(76)", "INVALID(77)", "INVALID(78)", "INVALID(79)", "INVALID(80)", "INVALID(81)", "INVALID(82)", "INVALID(83)", "INVALID(84)", "INVALID(85)", "INVALID(86)", "INVALID(87)", "INVALID(88)", "INVALID(89)", "INVALID(90)", "SQLT_SLS", "INVALID(92)", "INVALID(93)", "SQLT_LVC", "SQLT_LVB", "SQLT_AFC", "SQLT_AVC", "INVALID(98)", "INVALID(99)", "INVALID(100)", "INVALID(101)", "SQLT_CUR", "INVALID(103)", "SQLT_RDD", "SQLT_LAB", "SQLT_OSL", "INVALID(107)", "SQLT_NTY", "INVALID(109)", "SQLT_REF", "INVALID(111)", "SQLT_CLOB", "SQLT_BLOB", "SQLT_BFILEE", "SQLT_CFILEE", "SQLT_RSET", "INVALID(117)", "INVALID(118)", "INVALID(119)", "INVALID(121)", "INVALID(121)", "SQLT_NCO", "INVALID(123)", "INVALID(124)", "INVALID(125)", "INVALID(126)", "INVALID(127)", "INVALID(128)", "INVALID(129)", "INVALID(130)", "INVALID(131)", "INVALID(132)", "INVALID(133)", "INVALID(134)", "INVALID(135)", "INVALID(136)", "INVALID(137)", "INVALID(138)", "INVALID(139)", "INVALID(140)", "INVALID(141)", "INVALID(142)", "INVALID(143)", "INVALID(144)", "INVALID(145)", "INVALID(146)", "INVALID(147)", "INVALID(148)", "INVALID(149)", "INVALID(150)", "INVALID(151)", "INVALID(152)", "INVALID(153)", "INVALID(154)", "SQLT_VST  155                                      /* OCIString type */", "SQLT_ODT  156                                        /* OCIDate type */", "INVALID(157)", "INVALID(158)", "INVALID(159)", "INVALID(160)", "INVALID(161)", "INVALID(162)", "INVALID(163)", "INVALID(164)", "INVALID(165)", "INVALID(166)", "INVALID(167)", "INVALID(168)", "INVALID(169)", "INVALID(170)", "INVALID(171)", "INVALID(172)", "INVALID(173)", "INVALID(174)", "INVALID(175)", "INVALID(176)", "INVALID(177)", "INVALID(178)", "INVALID(179)", "INVALID(180)", "INVALID(181)", "INVALID(182)", "INVALID(183)", "SQLT_DATE", "SQLT_TIME", "SQLT_TIME_TZ", "SQLT_TIMESTAMP", "SQLT_TIMESTAMP_TZ", "SQLT_INTERVAL_YM", "SQLT_INTERVAL_DS"};
static char *SQLFCode1[] =     {"", "CREATE TABLE", "SET ROLE", "INSERT", "SELECT", "UPDATE", "DROP ROLE", "DROP VIEW", "DROP TABLE", "DELETE", "CREATE VIEW", "DROP USER", "CREATE ROLE", "CREATE SEQUENCE", "ALTER SEQUENCE", "(NOT USED)", "DROP SEQUENCE", "CREATE SCHEMA", "CREATE CLUSTER", "CREATE USER", "CREATE INDEX", "DROP INDEX", "DROP CLUSTER", "VALIDATE INDEX", "CREATE PROCEDURE", "ALTER PROCEDURE", "ALTER TABLE", "EXPLAIN", "GRANT", "REVOKE", "CREATE SYNONYM", "DROP SYNONYM", "ALTER SYSTEM SWITCH LOG", "SET TRANSACTION", "PL/SQL EXECUTE", "LOCK", "NOOP", "RENAME", "COMMENT", "AUDIT", "NO AUDIT", "ALTER INDEX", "CREATE EXTERNAL DATABASE", "DROP EXTERNAL DATABASE", "CREATE DATABASE", "ALTER DATABASE", "CREATE ROLLBACK SEGMENT", "ALTER ROLLBACK SEGMENT", "DROP ROLLBACK SEGMENT", "CREATE TABLESPACE", "ALTER TABLESPACE", "DROP TABLESPACE", "ALTER SESSION", "ALTER USER", "COMMIT (WORK)", "ROLLBACK", "SAVEPOINT", "CREATE CONTROL FILE", "ALTER TRACING", "CREATE TRIGGER", "ALTER TRIGGER", "DROP TRIGGER", "ANALYZE TABLE", "ANALYZE INDEX", "ANALYZE CLUSTER", "CREATE PROFILE", "DROP PROFILE", "ALTER PROFILE", "DROP PROCEDURE", "(NOT USED)", "ALTER RESOURCE COST", "CREATE SNAPSHOT LOG", "ALTER SNAPSHOT LOG", "DROP SNAPSHOT LOG", "CREATE SNAPSHOT", "ALTER SNAPSHOT", "DROP SNAPSHOT", "CREATE TYPE", "DROP TYPE", "ALTER ROLE", "ALTER TYPE", "CREATE TYPE BODY", "ALTER TYPE BODY", "DROP TYPE BODY", "DROP LIBRARY", "TRUNCATE TABLE", "TRUNCATE CLUSTER", "CREATE BITMAPFILE", "ALTER VIEW", "DROP BITMAPFILE", "SET CONSTRAINTS", "CREATE FUNCTION", "ALTER FUNCTION", "DROP FUNCTION", "CREATE PACKAGE", "ALTER PACKAGE", "DROP PACKAGE", "CREATE PACKAGE BODY", "ALTER PACKAGE BODY", "DROP PACKAGE BODY"};
static char *SQLFCode2[] =     {"CREATE DIRECTORY", "DROP DIRECTORY", "CREATE LIBRARY", "CREATE JAVA", "ALTER JAVA", "DROP JAVA", "CREATE OPERATOR", "CREATE INDEXTYPE", "DROP INDEXTYPE", "ALTER INDEXTYPE", "DROP OPERATOR", "ASSOCIATE STATISTICS", "DISASSOCIATE STATISTICS", "CALL METHOD", "CREATE SUMMARY", "ALTER SUMMARY", "DROP SUMMARY", "CREATE DIMENSION", "ALTER DIMENSION", "DROP DIMENSION", "CREATE CONTEXT", "DROP CONTEXT", "ALTER OUTLINE", "CREATE OUTLINE", "DROP OUTLINE", "UPDATE INDEXES", "ALTER OPERATOR"};
static char *OCIFNCode[] =     {"", "INITIALIZE", "HANDLEALLOC", "HANDLEFREE", "DESCRIPTORALLOC", "DESCRIPTORFREE", "ENVINIT", "SERVERATTACH", "SERVERDETACH", "UNUSED", "SESSIONBEGIN", "SESSIONEND", "PASSWORDCHANGE", "STMTPREPARE", "UNUSED", "UNUSED", "UNUSED", "BINDDYNAMIC", "BINDOBJECT", "UNUSED", "BINDARRAYOFSTRUCT", "STMTEXECUTE", "UNUSED", "UNUSED", "UNUSED", "DEFINEOBJECT", "DEFINEDYNAMIC", "DEFINEARRAYOFSTRUCT", "STMTFETCH", "STMTGETBIND", "UNUSED", "UNUSED", "DESCRIBEANY", "TRANSSTART", "TRANSDETACH", "TRANSCOMMIT", "UNUSED", "ERRORGET", "LOBOPENFILE", "LOBCLOSEFILE", "LOBCREATEFILE", "LOBFILEDELETE", "LOBCOPY", "LOBAPPEND", "LOBERASE", "LOBLENGTH", "LOBTRIM", "LOBREAD", "LOBWRITE", "UNUSED", "SVCCTXBREAK", "SERVERVERSION", "UNUSED", "UNUSED", "ATTRGET", "ATTRSET", "PARAMSET", "PARAMGET", "STMTGETPIECEINFO", "LDATOSVCCTX", "UNUSED", "STMTSETPIECEINFO", "TRANSFORGET", "TRANSPREPARE", "TRANSROLLBACK", "DEFINEBYPOS", "BINDBYPOS", "BINDBYNAME", "LOBASSIGN", "LOBISEQUAL", "LOBISINIT", "LOBENABLEBUFFERING", "LOBCHARSETID", "LOBCHARSETFORM", "LOBFILESETNAME", "LOBFILEGETNAME", "LOGON", "LOGOFF", "LOBDISABLEBUFFERING", "LOBFLUSHBUFFER", "LOBLOADFROMFILE", "LOBOPEN", "LOBCLOSE", "LOBISOPEN", "LOBFILEISOPEN", "LOBFILEEXISTS", "LOBFILECLOSEALL", "LOBCREATETEMP", "LOBFREETEMP", "LOBISTEMP", "AQENQ", "AQDEQ", "RESET", "SVCCTXTOLDA", "LOBLOCATORASSIGN", "UBINDBYNAME", "AQLISTEN", "SVC2HST", "SVCRH"};
static char *OCIPieceCodes[] = {"OCI_ONE_PIECE",  "OCI_FIRST_PIECE", "OCI_NEXT_PIECE",  "OCI_LAST_PIECE"};

static boolean (*pOCICharSetConversionIsReplacementUsed) (void *);
static boolean (*pOCIRefIsEqual) (OCIEnv *, OCIRef *, OCIRef *);
static boolean (*pOCIRefIsNull) (OCIEnv *, OCIRef *);
static boolean (*pOCIThreadIsMulti) (/* ??? */);
static boolean (*pOCIWideCharIsAlnum) (void *, OCIWchar);
static boolean (*pOCIWideCharIsAlpha) (void *, OCIWchar);
static boolean (*pOCIWideCharIsCntrl) (void *, OCIWchar);
static boolean (*pOCIWideCharIsDigit) (void *, OCIWchar);
static boolean (*pOCIWideCharIsGraph) (void *, OCIWchar);
static boolean (*pOCIWideCharIsLower) (void *, OCIWchar);
static boolean (*pOCIWideCharIsPrint) (void *, OCIWchar);
static boolean (*pOCIWideCharIsPunct) (void *, OCIWchar);
static boolean (*pOCIWideCharIsSingleByte) (void *, OCIWchar);
static boolean (*pOCIWideCharIsSpace) (void *, OCIWchar);
static boolean (*pOCIWideCharIsUpper) (void *, OCIWchar);
static boolean (*pOCIWideCharIsXdigit) (void *, OCIWchar);
static int (*pOCIMultiByteStrcmp) (void *, OraText *, OraText *, int);
static int (*pOCIMultiByteStrncmp) (void *, OraText *, size_t, OraText *, size_t, int);
static int (*pOCIWideCharStrcmp) (void *, OCIWchar *, OCIWchar *, int);
static int (*pOCIWideCharStrncmp) (void *, OCIWchar *, size_t, OCIWchar *, size_t, int);
static OCITypeCode (*pOCITypeCollTypeCode) (OCIEnv *, OCIError *, OCIType *);
static OCITypeCode (*pOCITypeElemExtTypeCode) (OCIEnv *, OCIError *, OCITypeElem *);
static OCITypeCode (*pOCITypeElemTypeCode) (OCIEnv *, OCIError *, OCITypeElem *);
static OCITypeCode (*pOCITypeTypeCode) (OCIEnv *, OCIError *, OCIType *);
static OCITypeEncap (*pOCITypeMethodEncap) (OCIEnv *, OCIError *, OCITypeMethod *);
static OCITypeMethodFlag (*pOCITypeMethodFlags) (OCIEnv *, OCIError *, OCITypeMethod *);
static OCITypeParamMode (*pOCITypeElemParamMode) (OCIEnv *, OCIError *, OCITypeElem *);
static OCIWchar *(*pOCIWideCharStrchr) (void *, OCIWchar *, OCIWchar);
static OCIWchar *(*pOCIWideCharStrrchr) (void *, OCIWchar *, OCIWchar);
static OCIWchar (*pOCIWideCharToLower) (void *, OCIWchar);
static OCIWchar (*pOCIWideCharToUpper) (void *, OCIWchar);
static OraText *(*pOCIMessageGet) (OCIMsg *, ub4, OraText *, size_t);
static sb1 (*pOCITypeElemNumScale) (OCIEnv *, OCIError *, OCITypeElem *);
static sb4 (*pOCICollMax) (OCIEnv *, OCIColl *);
static size_t (*pOCIMultiByteStrCaseConversion) (void *, OraText *, OraText *, ub4);
static size_t (*pOCIMultiByteStrcat) (void *, OraText *, OraText *);
static size_t (*pOCIMultiByteStrcpy) (void *, OraText *, OraText *);
static size_t (*pOCIMultiByteStrlen) (void *, OraText *);
static size_t (*pOCIMultiByteStrncat) (void *, OraText *, OraText *, size_t);
static size_t (*pOCIMultiByteStrncpy) (void *, OraText *, OraText *, size_t);
static size_t (*pOCIMultiByteStrnDisplayLength) (void *, OraText *, size_t);
static size_t (*pOCIWideCharDisplayLength) (void *, OCIWchar);
static size_t (*pOCIWideCharMultiByteLength) (void *, OCIWchar);
static size_t (*pOCIWideCharStrCaseConversion) (void *, OCIWchar *, OCIWchar *, ub4);
static size_t (*pOCIWideCharStrcat) (void *, OCIWchar *, OCIWchar *);
static size_t (*pOCIWideCharStrcpy) (void *, OCIWchar *, OCIWchar *);
static size_t (*pOCIWideCharStrlen) (void *, OCIWchar *);
static size_t (*pOCIWideCharStrncat) (void *, OCIWchar *, OCIWchar *, size_t);
static size_t (*pOCIWideCharStrncpy) (void *, OCIWchar *, OCIWchar *, size_t);
static sword (*pOCIAQDeq) (OCISvcCtx *, OCIError *, OraText *, OCIAQDeqOptions *, OCIAQMsgProperties *, OCIType *, void **, void **, OCIRaw **, ub4);
static sword (*pOCIAQEnq) (OCISvcCtx *, OCIError *, OraText *, OCIAQEnqOptions *, OCIAQMsgProperties *, OCIType *, void **, void **, OCIRaw **, ub4);
static sword (*pOCIAQListen) (/* ??? */);
static sword (*pOCIAttrGet) (void *, ub4, void *, ub4 *, ub4, OCIError *);
static sword (*pOCIAttrSet) (void *, ub4, void *, ub4, ub4, OCIError *);
static sword (*pOCIBindArrayOfStruct) (OCIBind *, OCIError *, ub4, ub4, ub4, ub4);
static sword (*pOCIBindByName) (OCIStmt *, OCIBind **, OCIError *, OraText *, sb4, void *, sb4, ub2, void *, ub2 *, ub2 *, ub4, ub4 *, ub4);
static sword (*pOCIBindByPos) (OCIStmt *, OCIBind **, OCIError *, ub4, void *, sb4, ub2, void *, ub2 *, ub2 *, ub4, ub4 *, ub4);
static sword (*pOCIBindDynamic) (OCIBind *, OCIError *, void *, OCICallbackInBind, void *, OCICallbackOutBind);
static sword (*pOCIBindObject) (OCIBind *, OCIError *, OCIType *, void **, ub4 *, void **, ub4 *);
static sword (*pOCIBreak) (void *, OCIError *);
static sword (*pOCICacheFlush) (OCIEnv *, OCIError *, OCISvcCtx *, void *, OCIRef *(*) (void *, ub1 *), OCIRef **);
static sword (*pOCICacheFlushRefresh) (OCIEnv *, OCIError *, OCISvcCtx *, void *, OCIRef *(*) (void *, ub1 *), OCIRef **);
static sword (*pOCICacheFree) (OCIEnv *, OCIError *, OCISvcCtx *);
static sword (*pOCICacheGetObjects) (OCIEnv *, OCIError *, OCISvcCtx *, OCIObjectProperty, void *, void (*) (void *, void *));
static sword (*pOCICacheRefresh) (OCIEnv *, OCIError *, OCISvcCtx *, OCIRefreshOpt, void *, OCIRef *(*) (void *), OCIRef **);
static sword (*pOCICacheRegister) (OCIEnv *, OCIError *, OCIObjectEvent, void *, void (*) (void *, OCIObjectEvent, void *));
static sword (*pOCICacheUnmark) (OCIEnv *, OCIError *, OCISvcCtx *);
static sword (*pOCICacheUnpin) (OCIEnv *, OCIError *, OCISvcCtx *);
static sword (*pOCICharSetToUnicode) (void *, ub2 *, size_t, OraText *, size_t, size_t *);
static sword (*pOCICollAppend) (OCIEnv *, OCIError *, void *, void *, OCIColl *);
static sword (*pOCICollAssignElem) (OCIEnv *, OCIError *, sb4, void *, void *, OCIColl *);
static sword (*pOCICollAssign) (OCIEnv *, OCIError *, OCIColl *, OCIColl *);
static sword (*pOCICollGetElem) (OCIEnv *, OCIError *, OCIColl *, sb4, boolean *, void **, void **);
static sword (*pOCICollIsLocator) (OCIEnv *, OCIError *, OCIColl *, boolean *);
static sword (*pOCICollSize) (OCIEnv *, OCIError *, OCIColl *, sb4 *);
static sword (*pOCICollTrim) (OCIEnv *, OCIError *, sb4, OCIColl *);
static sword (*pOCIContextClearValue) (void *, OCIError *, ub1 *, ub1);
static sword (*pOCIContextGenerateKey) (void *, OCIError *, ub4 *);
static sword (*pOCIContextGetValue) (void *, OCIError *, ub1 *, ub1, void **);
static sword (*pOCIContextSetValue) (void *, OCIError *, OCIDuration, ub1 *, ub1, void *);
static sword (*pOCIDateAddDays) (OCIError *, OCIDate *, sb4, OCIDate *);
static sword (*pOCIDateAddMonths) (OCIError *, OCIDate *, sb4, OCIDate *);
static sword (*pOCIDateAssign) (OCIError *, OCIDate *, OCIDate *);
static sword (*pOCIDateCheck) (OCIError *, OCIDate *, uword *);
static sword (*pOCIDateCompare) (OCIError *, OCIDate *, OCIDate *, sword *);
static sword (*pOCIDateDaysBetween) (OCIError *, OCIDate *, OCIDate *, sb4 *);
static sword (*pOCIDateFromText) (OCIError *, text *, ub4, text *, ub1, text *, ub4, OCIDate *);
static sword (*pOCIDateLastDay) (OCIError *, OCIDate *, OCIDate *);
static sword (*pOCIDateNextDay) (OCIError *, OCIDate *, text *, ub4, OCIDate *);
static sword (*pOCIDateSysDate) (OCIError *, OCIDate *);
static sword (*pOCIDateTimeAssign) (void *, OCIError *, OCIDateTime *, OCIDateTime *);
static sword (*pOCIDateTimeCheck) (void *, OCIError *, OCIDateTime *, ub4 *);
static sword (*pOCIDateTimeCompare) (void *, OCIError *, OCIDateTime *, OCIDateTime *, sword *);
static sword (*pOCIDateTimeConvert) (void *, OCIError *, OCIDateTime *, OCIDateTime *, ub1);
static sword (*pOCIDateTimeFromText) (void *, OCIError *, OraText *, ub4, ub1, OraText *, ub1, OraText *, ub4, OCIDateTime *);
static sword (*pOCIDateTimeGetDate) (void *, OCIError *, OCIDateTime *, sb2 *, ub1 *, ub1 *);
static sword (*pOCIDateTimeGetTime) (void *, OCIError *, OCIDateTime *, ub1 *, ub1 *, ub1 *, ub4 *);
static sword (*pOCIDateTimeGetTimeZone) (void *, OCIError *, OCIDateTime *, sb1 *, sb1 *);
static sword (*pOCIDateTimeIntervalAdd) (void *, OCIError *, OCIDateTime *, OCIInterval *, OCIDateTime *);
static sword (*pOCIDateTimeIntervalSub) (void *, OCIError *, OCIDateTime *, OCIInterval *, OCIDateTime *);
static sword (*pOCIDateTimeSetDate) (void *, OCIError *, OCIDateTime *, sb2, ub1, ub1);
static sword (*pOCIDateTimeSetTime) (void *, OCIError *, OCIDateTime *, ub1, ub1, ub1, ub4);
static sword (*pOCIDateTimeSetTimeZone) (void *, OCIError *, OCIDateTime *, sb1, sb1);
static sword (*pOCIDateTimeSubtract) (void *, OCIError *, OCIDateTime *, OCIDateTime *, OCIInterval *, ub1);
static sword (*pOCIDateTimeSysTimeStamp) (void *, OCIError *, OCIDateTime *);
static sword (*pOCIDateTimeToText) (void *, OCIError *, OCIDateTime *, OraText *, ub1, OraText *, ub4, ub4 *, OraText *);
static sword (*pOCIDateToText) (OCIError *, OCIDate *, text *, ub1, text *, ub4, ub4 *, text *);
static sword (*pOCIDateZoneToZone) (OCIError *, OCIDate *, text *, ub4, text *, ub4, OCIDate *);
static sword (*pOCIDefineArrayOfStruct) (OCIDefine *, OCIError *, ub4, ub4, ub4, ub4);
static sword (*pOCIDefineByPos) (OCIStmt *, OCIDefine **, OCIError *, ub4, void *, sb4, ub2, void *, ub2 *, ub2 *, ub4);
static sword (*pOCIDefineDynamic) (OCIDefine *, OCIError *, void *, OCICallbackDefine);
static sword (*pOCIDefineObject) (OCIDefine *, OCIError *, OCIType *, void **, ub4 *, void **, ub4 *);
static sword (*pOCIDescribeAny) (OCISvcCtx *, OCIError *, void *, ub4, ub1, ub1, ub1, OCIDescribe *);
static sword (*pOCIDescriptorAlloc) (void *, void **, ub4, size_t, void **);
static sword (*pOCIDescriptorFree) (void *, ub4);
static sword (*pOCIDirPathAbort) (OCIDirPathCtx *, OCIError *);
static sword (*pOCIDirPathColArrayEntryGet) (OCIDirPathColArray *, OCIError *, ub4, ub2, ub1 **, ub4 *, ub1 *);
static sword (*pOCIDirPathColArrayEntrySet) (OCIDirPathColArray *, OCIError *, ub4, ub2, ub1 *, ub4, ub1);
static sword (*pOCIDirPathColArrayReset) (OCIDirPathColArray *, OCIError *);
static sword (*pOCIDirPathColArrayRowGet) (OCIDirPathColArray *, OCIError *, ub4, ub1 ***, ub4 **, ub1 **);
static sword (*pOCIDirPathColArrayToStream) (OCIDirPathColArray *, OCIDirPathCtx *, OCIDirPathStream *, OCIError *, ub4, ub4);
static sword (*pOCIDirPathDataSave) (OCIDirPathCtx *, OCIError *);
static sword (*pOCIDirPathFinish) (OCIDirPathCtx *, OCIError *);
static sword (*pOCIDirPathFlushRow) (OCIDirPathCtx *, OCIError *);
static sword (*pOCIDirPathLoadStream) (OCIDirPathCtx *, OCIDirPathStream *, OCIError *);
static sword (*pOCIDirPathPrepare) (OCIDirPathCtx *, OCISvcCtx *, OCIError *);
static sword (*pOCIDirPathStreamReset) (OCIDirPathStream *, OCIError *);
static sword (*pOCIDirPathStreamToStream) (OCIDirPathStream *, OCIDirPathStream *, OCIDirPathCtx *, OCIError *, ub4, ub4);
static sword (*pOCIDurationBegin) (OCIEnv *, OCIError *, OCISvcCtx *, OCIDuration, OCIDuration *);
static sword (*pOCIDurationEnd) (OCIEnv *, OCIError *, OCISvcCtx *, OCIDuration);
static sword (*pOCIDurationGetParent) (OCIEnv *, OCIError *, OCIDuration, OCIDuration *);
static sword (*pOCIEnvCallback) (OCIEnv *, ub4, size_t, void *);
static sword (*pOCIEnvCreate) (OCIEnv **, ub4, void *, void *(*) (void *, size_t), void *(*) (void *, void *, size_t), void (*) (void *, void *), size_t, void **);
static sword (*pOCIEnvInit) (OCIEnv **, ub4, size_t, void **);
static sword (*pOCIErrorGet) (void *, ub4, OraText *, sb4 *, OraText *, ub4, ub4);
static sword (*pOCIExtractFromFile) (void *, OCIError *, ub4, OraText *);
static sword (*pOCIExtractFromList) (void *, OCIError *, uword, OraText **, ub1 *, uword *, void ***);
static sword (*pOCIExtractFromStr) (void *, OCIError *, ub4, OraText *);
static sword (*pOCIExtractInit) (void *, OCIError *);
static sword (*pOCIExtractReset) (void *, OCIError *);
static sword (*pOCIExtractSetKey) (void *, OCIError *, OraText *, ub1, ub4, void *, sb4 *, OraText **);
static sword (*pOCIExtractSetNumKeys) (void *, OCIError *, uword);
static sword (*pOCIExtractTerm) (void *, OCIError *);
static sword (*pOCIExtractToBool) (void *, OCIError *, OraText *, uword, ub1 *);
static sword (*pOCIExtractToInt) (void *, OCIError *, OraText *, uword, sb4 *);
static sword (*pOCIExtractToList) (void *, OCIError *, uword *);
static sword (*pOCIExtractToOCINum) (void *, OCIError *, OraText *, uword, OCINumber *);
static sword (*pOCIExtractToStr) (void *, OCIError *, OraText *, uword, OraText *, uword);
static sword (*pOCIFileClose) (void *, OCIError *, OCIFileObject *);
static sword (*pOCIFileExists) (void *, OCIError *, OraText *, OraText *, ub1 *);
static sword (*pOCIFileFlush) (void *, OCIError *, OCIFileObject *);
static sword (*pOCIFileGetLength) (void *, OCIError *, OraText *, OraText *, ubig_ora *);
static sword (*pOCIFileInit) (void *, OCIError *);
static sword (*pOCIFileOpen) (void *, OCIError *, OCIFileObject **, OraText *, OraText *, ub4, ub4, ub4);
static sword (*pOCIFileRead) (void *, OCIError *, OCIFileObject *, void *, ub4, ub4 *);
static sword (*pOCIFileSeek) (void *, OCIError *, OCIFileObject *, uword, ubig_ora, sb1);
static sword (*pOCIFileTerm) (void *, OCIError *);
static sword (*pOCIFileWrite) (void *, OCIError *, OCIFileObject *, void *, ub4, ub4 *);
static sword (*pOCIFormatInit) (void *, OCIError *);
static sword (*pOCIFormatString) (void *, OCIError *, OraText *, sbig_ora, sbig_ora *, OraText *, ...);
static sword (*pOCIFormatTChar) (void);
static sword (*pOCIFormatTDouble) (void);
static sword (*pOCIFormatTDvoid) (void);
static sword (*pOCIFormatTEb1) (void);
static sword (*pOCIFormatTEb2) (void);
static sword (*pOCIFormatTEb4) (void);
static sword (*pOCIFormatTEnd) (void);
static sword (*pOCIFormatTerm) (void *, OCIError *);
static sword (*pOCIFormatTEword) (void);
static sword (*pOCIFormatTSb1) (void);
static sword (*pOCIFormatTSb2) (void);
static sword (*pOCIFormatTSb4) (void);
static sword (*pOCIFormatTSbig_ora) (void);
static sword (*pOCIFormatTSword) (void);
static sword (*pOCIFormatTText) (void);
static sword (*pOCIFormatTUb1) (void);
static sword (*pOCIFormatTUb2) (void);
static sword (*pOCIFormatTUb4) (void);
static sword (*pOCIFormatTUbig_ora) (void);
static sword (*pOCIFormatTUword) (void);
static sword (*pOCIHandleAlloc) (void *, void **, ub4, size_t, void **);
static sword (*pOCIHandleFree) (void *, ub4);
static sword (*pOCIInitialize) (ub4, void *, void *(*) (void *, size_t), void *(*) (void *, void *, size_t), void (*) (void *, void *));
static sword (*pOCIIntervalAdd) (void *, OCIError *, OCIInterval *, OCIInterval *, OCIInterval *);
static sword (*pOCIIntervalAssign) (void *, OCIError *, OCIInterval *, OCIInterval *);
static sword (*pOCIIntervalCheck) (void *, OCIError *, OCIInterval *, ub4 *);
static sword (*pOCIIntervalCompare) (void *, OCIError *, OCIInterval *, OCIInterval *, sword *);
static sword (*pOCIIntervalDivide) (void *, OCIError *, OCIInterval *, OCINumber *, OCIInterval *);
static sword (*pOCIIntervalFromNumber) (void *, OCIError *, OCIInterval *, OCINumber *);
static sword (*pOCIIntervalFromText) (void *, OCIError *, OraText *, ub4, ub1, OCIInterval *);
static sword (*pOCIIntervalGetDaySecond) (void *, OCIError *, sb4 *, sb4 *, sb4 *, sb4 *, sb4 *, OCIInterval *);
static sword (*pOCIIntervalGetYearMonth) (void *, OCIError *, sb4 *, sb4 *, OCIInterval *);
static sword (*pOCIIntervalMultiply) (void *, OCIError *, OCIInterval *, OCINumber *, OCIInterval *);
static sword (*pOCIIntervalSetDaySecond) (void *, OCIError *, sb4, sb4, sb4, sb4, sb4, OCIInterval *);
static sword (*pOCIIntervalSetYearMonth) (void *, OCIError *, sb4, sb4, OCIInterval *);
static sword (*pOCIIntervalSubtract) (void *, OCIError *, OCIInterval *, OCIInterval *, OCIInterval *);
static sword (*pOCIIntervalToNumber) (void *, OCIError *, const OCIInterval *, OCINumber *);
static sword (*pOCIIntervalToText) (void *, OCIError *, OCIInterval *, OraText *, ub4, ub4 *);
static sword (*pOCIIterCreate) (OCIEnv *, OCIError *, OCIColl *, OCIIter **);
static sword (*pOCIIterDelete) (OCIEnv *, OCIError *, OCIIter **);
static sword (*pOCIIterGetCurrent) (OCIEnv *, OCIError *, OCIIter *, void **, void **);
static sword (*pOCIIterInit) (OCIEnv *, OCIError *, OCIColl *, OCIIter *);
static sword (*pOCIIterNext) (OCIEnv *, OCIError *, OCIIter *, void **, void **, boolean *);
static sword (*pOCIIterPrev) (OCIEnv *, OCIError *, OCIIter *, void **, void **, boolean *);
static sword (*pOCILdaToSvcCtx) (OCISvcCtx **, OCIError *, Lda_Def *);
static sword (*pOCILobAppend) (OCISvcCtx *, OCIError *, OCILobLocator *, OCILobLocator *);
static sword (*pOCILobAssign) (OCIEnv *, OCIError *, OCILobLocator *, OCILobLocator **);
static sword (*pOCILobCharSetForm) (OCIEnv *, OCIError *, OCILobLocator *, ub1 *);
static sword (*pOCILobCharSetId) (OCIEnv *, OCIError *, OCILobLocator *, ub2 *);
static sword (*pOCILobClose) (OCISvcCtx *, OCIError *, OCILobLocator *);
static sword (*pOCILobCopy) (OCISvcCtx *, OCIError *, OCILobLocator *, OCILobLocator *, ub4, ub4, ub4);
static sword (*pOCILobCreateTemporary) (OCISvcCtx *, OCIError *, OCILobLocator *, ub2, ub1, ub1, boolean, OCIDuration);
static sword (*pOCILobDisableBuffering) (OCISvcCtx *, OCIError *, OCILobLocator *);
static sword (*pOCILobEnableBuffering) (OCISvcCtx *, OCIError *, OCILobLocator *);
static sword (*pOCILobErase) (OCISvcCtx *, OCIError *, OCILobLocator *, ub4 *, ub4);
static sword (*pOCILobFileCloseAll) (OCISvcCtx *, OCIError *);
static sword (*pOCILobFileClose) (OCISvcCtx *, OCIError *, OCILobLocator *);
static sword (*pOCILobFileExists) (OCISvcCtx *, OCIError *, OCILobLocator *, boolean *);
static sword (*pOCILobFileGetName) (OCIEnv *, OCIError *, OCILobLocator *, OraText *, ub2 *, OraText *, ub2 *);
static sword (*pOCILobFileIsOpen) (OCISvcCtx *, OCIError *, OCILobLocator *, boolean *);
static sword (*pOCILobFileOpen) (OCISvcCtx *, OCIError *, OCILobLocator *, ub1);
static sword (*pOCILobFileSetName) (OCIEnv *, OCIError *, OCILobLocator **, OraText *, ub2, OraText *, ub2);
static sword (*pOCILobFlushBuffer) (OCISvcCtx *, OCIError *, OCILobLocator *, ub4);
static sword (*pOCILobFreeTemporary) (OCISvcCtx *, OCIError *, OCILobLocator *);
static sword (*pOCILobGetChunkSize) (OCISvcCtx *, OCIError *, OCILobLocator *, ub4 *);
static sword (*pOCILobGetLength) (OCISvcCtx *, OCIError *, OCILobLocator *, ub4 *);
static sword (*pOCILobIsEqual) (OCIEnv *, OCILobLocator *, OCILobLocator *, boolean *);
static sword (*pOCILobIsOpen) (OCISvcCtx *, OCIError *, OCILobLocator *, boolean *);
static sword (*pOCILobIsTemporary) (OCIEnv *, OCIError *, OCILobLocator *, boolean *);
static sword (*pOCILobLoadFromFile) (OCISvcCtx *, OCIError *, OCILobLocator *, OCILobLocator *, ub4, ub4, ub4);
static sword (*pOCILobLocatorAssign) (OCISvcCtx *, OCIError *, OCILobLocator *, OCILobLocator **);
static sword (*pOCILobLocatorIsInit) (OCIEnv *, OCIError *, OCILobLocator *, boolean *);
static sword (*pOCILobOpen) (OCISvcCtx *, OCIError *, OCILobLocator *, ub1);
static sword (*pOCILobRead) (OCISvcCtx *, OCIError *, OCILobLocator *, ub4 *, ub4, void *, ub4, void *, sb4 (*) (void *, void *, ub4, ub1), ub2, ub1);
static sword (*pOCILobTrim) (OCISvcCtx *, OCIError *, OCILobLocator *, ub4);
static sword (*pOCILobWriteAppend) (OCISvcCtx *, OCIError *, OCILobLocator *, ub4 *, void *, ub4, ub1, void *, sb4 (*) (void *, void *, ub4 *, ub1 *), ub2, ub1);
static sword (*pOCILobWrite) (OCISvcCtx *, OCIError *, OCILobLocator *, ub4 *, ub4, void *, ub4, ub1, void *, sb4 (*) (void *, void *, ub4 *, ub1 *), ub2, ub1);
static sword (*pOCILogoff) (OCISvcCtx *, OCIError *);
static sword (*pOCILogon) (OCIEnv *, OCIError *, OCISvcCtx **, OraText *, ub4, OraText *, ub4, OraText *, ub4);
static sword (*pOCIMemoryAlloc) (void *, OCIError *, void **, OCIDuration, ub4, ub4);
static sword (*pOCIMemoryFree) (void *, OCIError *, void *);
static sword (*pOCIMemoryResize) (void *, OCIError *, void **, ub4, ub4);
static sword (*pOCIMemorySetCurrentIDs) (void *, OCIError *, ub4, ub4, ub4);
static sword (*pOCIMessageClose) (void *, OCIError *, OCIMsg *);
static sword (*pOCIMessageOpen) (void *, OCIError *, OCIMsg **, OraText *, OraText *, OCIDuration);
static sword (*pOCIMultiByteInSizeToWideChar) (void *, OCIWchar *, size_t, OraText *, size_t, size_t *);
static sword (*pOCIMultiByteToWideChar) (void *, OCIWchar *, OraText *, size_t *);
static sword (*pOCINlsGetInfo) (void *, OCIError *, OraText *, size_t, ub2);
static sword (*pOCINumberAbs) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberAdd) (OCIError *, OCINumber *, OCINumber *, OCINumber *);
static sword (*pOCINumberArcCos) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberArcSin) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberArcTan2) (OCIError *, OCINumber *, OCINumber *, OCINumber *);
static sword (*pOCINumberArcTan) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberAssign) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberCeil) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberCmp) (OCIError *, OCINumber *, OCINumber *, sword *);
static sword (*pOCINumberCos) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberDec) (OCIError *, OCINumber *);
static sword (*pOCINumberDiv) (OCIError *, OCINumber *, OCINumber *, OCINumber *);
static sword (*pOCINumberExp) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberFloor) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberFromInt) (OCIError *, void *, uword, uword, OCINumber *);
static sword (*pOCINumberFromReal) (OCIError *, void *, uword, OCINumber *);
static sword (*pOCINumberFromText) (OCIError *, text *, ub4, text *, ub4, text *, ub4, OCINumber *);
static sword (*pOCINumberHypCos) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberHypSin) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberHypTan) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberInc) (OCIError *, OCINumber *);
static sword (*pOCINumberIntPower) (OCIError *, OCINumber *, sword, OCINumber *);
static sword (*pOCINumberIsInt) (OCIError *, OCINumber *, boolean *);
static sword (*pOCINumberIsZero) (OCIError *, OCINumber *, boolean *);
static sword (*pOCINumberLn) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberLog) (OCIError *, OCINumber *, OCINumber *, OCINumber *);
static sword (*pOCINumberMod) (OCIError *, OCINumber *, OCINumber *, OCINumber *);
static sword (*pOCINumberMul) (OCIError *, OCINumber *, OCINumber *, OCINumber *);
static sword (*pOCINumberNeg) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberPower) (OCIError *, OCINumber *, OCINumber *, OCINumber *);
static sword (*pOCINumberPrec) (OCIError *, OCINumber *, eword, OCINumber *);
static sword (*pOCINumberRound) (OCIError *, OCINumber *, sword, OCINumber *);
static sword (*pOCINumberShift) (OCIError *, OCINumber *, sword, OCINumber *);
static sword (*pOCINumberSign) (OCIError *, OCINumber *, sword *);
static sword (*pOCINumberSin) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberSqrt) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberSub) (OCIError *, OCINumber *, OCINumber *, OCINumber *);
static sword (*pOCINumberTan) (OCIError *, OCINumber *, OCINumber *);
static sword (*pOCINumberToInt) (OCIError *, OCINumber *, uword, uword, void *);
static sword (*pOCINumberToReal) (OCIError *, OCINumber *, uword, void *);
static sword (*pOCINumberToText) (OCIError *, OCINumber *, text *, ub4, text *, ub4, ub4 *, text *);
static sword (*pOCINumberTrunc) (OCIError *, OCINumber *, sword, OCINumber *);
static sword (*pOCIObjectAlwaysLatest) (OCIEnv *, OCIError *, void *);
static sword (*pOCIObjectArrayPin) (OCIEnv *, OCIError *, OCIRef **, ub4, OCIComplexObject **, ub4, OCIPinOpt, OCIDuration, OCILockOpt, void **, ub4 *);
static sword (*pOCIObjectCopy) (OCIEnv *, OCIError *, OCISvcCtx *, void *, void *, void *, void *, OCIType *, OCIDuration, ub1);
static sword (*pOCIObjectExists) (OCIEnv *, OCIError *, void *, boolean *);
static sword (*pOCIObjectFlush) (OCIEnv *, OCIError *, void *);
static sword (*pOCIObjectFlushRefresh) (OCIEnv *, OCIError *, void *);
static sword (*pOCIObjectFree) (OCIEnv *, OCIError *, void *, ub2);
static sword (*pOCIObjectGetInd) (OCIEnv *, OCIError *, void *, void **);
static sword (*pOCIObjectGetObjectRef) (OCIEnv *, OCIError *, void *, OCIRef *);
static sword (*pOCIObjectGetPrimaryKeyTypeRef) (OCIEnv *, OCIError *, OCISvcCtx *, void *, OCIRef *);
static sword (*pOCIObjectGetProperty) (OCIEnv *, OCIError *, void *, OCIObjectPropId, void *, ub4 *);
static sword (*pOCIObjectGetTypeRef) (OCIEnv *, OCIError *, void *, OCIRef *);
static sword (*pOCIObjectIsDirtied) (OCIEnv *, OCIError *, void *, boolean *);
static sword (*pOCIObjectIsDirty) (OCIEnv *, OCIError *, void *, boolean *);
static sword (*pOCIObjectIsLoaded) (OCIEnv *, OCIError *, void *, boolean *);
static sword (*pOCIObjectIsLocked) (OCIEnv *, OCIError *, void *, boolean *);
static sword (*pOCIObjectLockNoWait) (OCIEnv *, OCIError *, void *);
static sword (*pOCIObjectLock) (OCIEnv *, OCIError *, void *);
static sword (*pOCIObjectMakeObjectRef) (OCIEnv *, OCIError *, OCISvcCtx *, void *, void **, ub4, OCIRef *);
static sword (*pOCIObjectMarkDeleteByRef) (OCIEnv *, OCIError *, OCIRef *);
static sword (*pOCIObjectMarkDelete) (OCIEnv *, OCIError *, void *);
static sword (*pOCIObjectMarkUpdate) (OCIEnv *, OCIError *, void *);
static sword (*pOCIObjectNew) (OCIEnv *, OCIError *, OCISvcCtx *, OCITypeCode, OCIType *, void *, OCIDuration, boolean, void **);
static sword (*pOCIObjectNotAlwaysLatest) (OCIEnv *, OCIError *, void *);
static sword (*pOCIObjectPinCountReset) (OCIEnv *, OCIError *, void *);
static sword (*pOCIObjectPin) (OCIEnv *, OCIError *, OCIRef *, OCIComplexObject *, OCIPinOpt, OCIDuration, OCILockOpt, void **);
static sword (*pOCIObjectPinTable) (OCIEnv *, OCIError *, OCISvcCtx *, text *, ub4, text *, ub4, OCIRef *, OCIDuration, void **);
static sword (*pOCIObjectRefresh) (OCIEnv *, OCIError *, void *);
static sword (*pOCIObjectUnmarkByRef) (OCIEnv *, OCIError *, OCIRef *);
static sword (*pOCIObjectUnmark) (OCIEnv *, OCIError *, void *);
static sword (*pOCIObjectUnpin) (OCIEnv *, OCIError *, void *);
static sword (*pOCIParamGet) (void *, ub4, OCIError *, void **, ub4);
static sword (*pOCIParamSet) (void *, ub4, OCIError *, void *, ub4, ub4);
static sword (*pOCIPasswordChange) (OCISvcCtx *, OCIError *, OraText *, ub4, OraText *, ub4, OraText *, ub4, ub4);
static sword (*pOCIPicklerFdoFree) (OCIEnv *, OCIError *, OCIPicklerFdo *);
static sword (*pOCIPicklerFdoInit) (OCIEnv *, OCIError *, OCIPicklerFdo **);
static sword (*pOCIPicklerImageAddNullScalar) (OCIEnv *, OCIError *, OCIPicklerImage *);
static sword (*pOCIPicklerImageAddScalar) (OCIEnv *, OCIError *, OCIPicklerImage *, void *, ub4);
static sword (*pOCIPicklerImageCollAddScalar) (OCIEnv *, OCIError *, OCIPicklerImage *, void *, ub4, OCIInd);
static sword (*pOCIPicklerImageCollBegin) (OCIEnv *, OCIError *, OCIPicklerImage *, OCIPicklerTds *);
static sword (*pOCIPicklerImageCollBeginScan) (OCIEnv *, OCIError *, OCIPicklerImage *, OCIPicklerTds *, ub4, ub4, OCIInd *);
static sword (*pOCIPicklerImageCollEnd) (OCIEnv *, OCIError *, OCIPicklerImage *);
static sword (*pOCIPicklerImageCollGetScalar) (OCIEnv *, OCIError *, OCIPicklerImage *, void *, ub4 *, OCIInd *);
static sword (*pOCIPicklerImageCollGetScalarSize) (OCIEnv *, OCIError *, OCIPicklerTds *, ub4 *);
static sword (*pOCIPicklerImageFree) (OCIEnv *, OCIError *, OCIPicklerImage *);
static sword (*pOCIPicklerImageGenerate) (OCIEnv *, OCIError *, OCIPicklerImage *);
static sword (*pOCIPicklerImageGetScalar) (OCIEnv *, OCIError *, OCIPicklerImage *, ub4, void *, ub4 *, OCIInd *);
static sword (*pOCIPicklerImageGetScalarSize) (OCIEnv *, OCIError *, OCIPicklerImage *, ub4, ub4 *);
static sword (*pOCIPicklerImageInit) (OCIEnv *, OCIError *, OCIPicklerFdo *, OCIPicklerTds *, OCIPicklerImage **);
static sword (*pOCIPicklerTdsAddAttr) (OCIEnv *, OCIError *, OCIPicklerTds *, OCIPicklerTdsElement);
static sword (*pOCIPicklerTdsCreateElementChar) (OCIEnv *, OCIError *, OCIPicklerTds *, ub2, OCIPicklerTdsElement *);
static sword (*pOCIPicklerTdsCreateElementNumber) (OCIEnv *, OCIError *, OCIPicklerTds *, ub1, sb1, OCIPicklerTdsElement *);
static sword (*pOCIPicklerTdsCreateElement) (OCIEnv *, OCIError *, OCIPicklerTds *, OCITypeCode, OCIPicklerTdsElement *);
static sword (*pOCIPicklerTdsCreateElementRaw) (OCIEnv *, OCIError *, OCIPicklerTds *, ub2, OCIPicklerTdsElement *);
static sword (*pOCIPicklerTdsCreateElementVarchar) (OCIEnv *, OCIError *, OCIPicklerTds *, ub2, OCIPicklerTdsElement *);
static sword (*pOCIPicklerTdsCtxFree) (OCIEnv *, OCIError *, OCIPicklerTdsCtx *);
static sword (*pOCIPicklerTdsCtxInit) (OCIEnv *, OCIError *, OCIPicklerTdsCtx **);
static sword (*pOCIPicklerTdsFree) (OCIEnv *, OCIError *, OCIPicklerTds *);
static sword (*pOCIPicklerTdsGenerate) (OCIEnv *, OCIError *, OCIPicklerTds *);
static sword (*pOCIPicklerTdsGetAttr) (OCIEnv *, OCIError *, OCIPicklerTds *, ub1, OCITypeCode *, ub2 *);
static sword (*pOCIPicklerTdsInit) (OCIEnv *, OCIError *, OCIPicklerTdsCtx *, OCIPicklerTds **);
static sword (*pOCIRawAllocSize) (OCIEnv *, OCIError *, OCIRaw *, ub4 *);
static sword (*pOCIRawAssignBytes) (OCIEnv *, OCIError *, ub1 *, ub4, OCIRaw **);
static sword (*pOCIRawAssignRaw) (OCIEnv *, OCIError *, OCIRaw *, OCIRaw **);
static sword (*pOCIRawResize) (OCIEnv *, OCIError *, ub4, OCIRaw **);
static sword (*pOCIRefAssign) (OCIEnv *, OCIError *, OCIRef *, OCIRef **);
static sword (*pOCIRefFromHex) (OCIEnv *, OCIError *, OCISvcCtx *, text *, ub4, OCIRef **);
static sword (*pOCIRefToHex) (OCIEnv *, OCIError *, OCIRef *, text *, ub4 *);
static sword (*pOCIReset) (void *, OCIError *);
static sword (*pOCIResultSetToStmt) (OCIResult *, OCIError *);
static sword (*pOCISecurityAbortIdentity) (OCISecurity *, OCIError *, nzttIdentity **);
static sword (*pOCISecurityClosePersona) (OCISecurity *, OCIError *, nzttPersona *);
static sword (*pOCISecurityCloseWallet) (OCISecurity *, OCIError *, nzttWallet *);
static sword (*pOCISecurityCreateIdentity) (OCISecurity *, OCIError *, nzttIdentType, nzttIdentityDesc *, nzttIdentity **);
static sword (*pOCISecurityCreatePersona) (OCISecurity *, OCIError *, nzttIdentType, nzttCipherType, nzttPersonaDesc *, nzttPersona **);
static sword (*pOCISecurityCreateWallet) (OCISecurity *, OCIError *, size_t, OraText *, size_t, OraText *, nzttWallet *);
static sword (*pOCISecurityDecrypt) (OCISecurity *, OCIError *, nzttPersona *, nzttces, size_t, ub1 *, nzttBufferBlock *);
static sword (*pOCISecurityDeEnvelope) (OCISecurity *, OCIError *, nzttPersona *, nzttces, size_t, ub1 *, nzttBufferBlock *, boolean *, boolean *, nzttIdentity **);
static sword (*pOCISecurityDestroyWallet) (OCISecurity *, OCIError *, size_t, OraText *, size_t, OraText *);
static sword (*pOCISecurityEncryptExpansion) (OCISecurity *, OCIError *, nzttPersona *, size_t, size_t *);
static sword (*pOCISecurityEncrypt) (OCISecurity *, OCIError *, nzttPersona *, nzttces, size_t, ub1 *, nzttBufferBlock *);
static sword (*pOCISecurityEnvelope) (OCISecurity *, OCIError *, nzttPersona *, size_t, nzttIdentity *, nzttces, size_t, ub1 *, nzttBufferBlock *);
static sword (*pOCISecurityFreeIdentity) (OCISecurity *, OCIError *, nzttIdentity **);
static sword (*pOCISecurityGetIdentity) (OCISecurity *, OCIError *, size_t, OraText *, nzttIdentity **);
static sword (*pOCISecurityGetProtection) (OCISecurity *, OCIError *, nzttPersona *, nzttcef, nztttdufmt *, nzttProtInfo *);
static sword (*pOCISecurityHashExpansion) (OCISecurity *, OCIError *, nzttPersona *, size_t, size_t *);
static sword (*pOCISecurityHash) (OCISecurity *, OCIError *, nzttPersona *, nzttces, size_t, ub1 *, nzttBufferBlock *);
static sword (*pOCISecurityInitBlock) (OCISecurity *, OCIError *, nzttBufferBlock *);
static sword (*pOCISecurityInitialize) (OCISecurity *, OCIError *);
static sword (*pOCISecurityKeyedHashExpansion) (OCISecurity *, OCIError *, nzttPersona *, size_t, size_t *);
static sword (*pOCISecurityKeyedHash) (OCISecurity *, OCIError *, nzttPersona *, nzttces, size_t, ub1 *, nzttBufferBlock *);
static sword (*pOCISecurityOpenPersona) (OCISecurity *, OCIError *, nzttPersona *);
static sword (*pOCISecurityOpenWallet) (OCISecurity *, OCIError *, size_t, OraText *, size_t, OraText *, nzttWallet *);
static sword (*pOCISecurityPKDecrypt) (OCISecurity *, OCIError *, nzttPersona *, nzttces, size_t, ub1 *, nzttBufferBlock *);
static sword (*pOCISecurityPKEncryptExpansion) (OCISecurity *, OCIError *, nzttPersona *, size_t, size_t, size_t *);
static sword (*pOCISecurity_PKEncrypt) (OCISecurity *, OCIError *, nzttPersona *, size_t, nzttIdentity *, nzttces, size_t, ub1 *, nzttBufferBlock *);
static sword (*pOCISecurityPurgeBlock) (OCISecurity *, OCIError *, nzttBufferBlock *);
static sword (*pOCISecurityRandomBytes) (OCISecurity *, OCIError *, nzttPersona *, size_t, nzttBufferBlock *);
static sword (*pOCISecurityRandomNumber) (OCISecurity *, OCIError *, nzttPersona *, uword *);
static sword (*pOCISecurityRemoveIdentity) (OCISecurity *, OCIError *, nzttIdentity **);
static sword (*pOCISecurityRemovePersona) (OCISecurity *, OCIError *, nzttPersona **);
static sword (*pOCISecurityReuseBlock) (OCISecurity *, OCIError *, nzttBufferBlock *);
static sword (*pOCISecuritySeedRandom) (OCISecurity *, OCIError *, nzttPersona *, size_t, ub1 *);
static sword (*pOCISecuritySetBlock) (OCISecurity *, OCIError *, uword, size_t, size_t, ub1 *, nzttBufferBlock *);
static sword (*pOCISecuritySetProtection) (OCISecurity *, OCIError *, nzttPersona *, nzttcef, nztttdufmt, nzttProtInfo *);
static sword (*pOCISecuritySignDetached) (OCISecurity *, OCIError *, nzttPersona *, nzttces, size_t, ub1 *, nzttBufferBlock *);
static sword (*pOCISecuritySignDetExpansion) (OCISecurity *, OCIError *, nzttPersona *, size_t, size_t *);
static sword (*pOCISecuritySignExpansion) (OCISecurity *, OCIError *, nzttPersona *, size_t, size_t *);
static sword (*pOCISecuritySign) (OCISecurity *, OCIError *, nzttPersona *, nzttces, size_t, ub1 *, nzttBufferBlock *);
static sword (*pOCISecurityStorePersona) (OCISecurity *, OCIError *, nzttPersona **, nzttWallet *);
static sword (*pOCISecurityStoreTrustedIdentity) (OCISecurity *, OCIError *, nzttIdentity **, nzttPersona *);
static sword (*pOCISecurityTerminate) (OCISecurity *, OCIError *);
static sword (*pOCISecurityValidate) (OCISecurity *, OCIError *, nzttPersona *, nzttIdentity *, boolean *);
static sword (*pOCISecurityVerifyDetached) (OCISecurity *, OCIError *, nzttPersona *, nzttces, size_t, ub1 *, size_t, ub1 *, boolean *, boolean *, nzttIdentity **);
static sword (*pOCISecurityVerify) (OCISecurity *, OCIError *, nzttPersona *, nzttces, size_t, ub1 *, nzttBufferBlock *, boolean *, boolean *, nzttIdentity **);
static sword (*pOCIServerAttach) (OCIServer *, OCIError *, OraText *, sb4, ub4);
static sword (*pOCIServerDetach) (OCIServer *, OCIError *, ub4);
static sword (*pOCIServerVersion) (void *, OCIError *, OraText *, ub4, ub1);
static sword (*pOCISessionBegin) (OCISvcCtx *, OCIError *, OCISession *, ub4, ub4);
static sword (*pOCISessionEnd) (OCISvcCtx *, OCIError *, OCISession *, ub4);
static sword (*pOCIStmtExecute) (OCISvcCtx *, OCIStmt *, OCIError *, ub4, ub4, OCISnapshot *, OCISnapshot *, ub4);
static sword (*pOCIStmtFetch) (OCIStmt *, OCIError *, ub4, ub2, ub4);
static sword (*pOCIStmtGetBindInfo) (OCIStmt *, OCIError *, ub4, ub4, sb4 *, OraText **, ub1 *, OraText **, ub1 *, ub1 *, OCIBind **);
static sword (*pOCIStmtGetPieceInfo) (OCIStmt *, OCIError *, void **, ub4 *, ub1 *, ub4 *, ub4 *, ub1 *);
static sword (*pOCIStmtPrepare) (OCIStmt *, OCIError *, OraText *, ub4, ub4, ub4);
static sword (*pOCIStmtSetPieceInfo) (void *, ub4, OCIError *, void *, ub4 *, ub1, void *, ub2 *);
static sword (*pOCIStringAllocSize) (OCIEnv *, OCIError *, OCIString *, ub4 *);
static sword (*pOCIStringAssign) (OCIEnv *, OCIError *, OCIString *, OCIString **);
static sword (*pOCIStringAssignText) (OCIEnv *, OCIError *, text *, ub4, OCIString **);
static sword (*pOCIStringResize) (OCIEnv *, OCIError *, ub4, OCIString **);
static sword (*pOCISubscriptionDisable) (OCISubscription *, OCIError *, ub4);
static sword (*pOCISubscriptionEnable) (OCISubscription *, OCIError *, ub4);
static sword (*pOCISubscriptionPost) (OCISvcCtx *, OCISubscription **, ub2, OCIError *, ub4);
static sword (*pOCISubscriptionRegister) (OCISvcCtx *, OCISubscription **, ub2, OCIError *, ub4);
static sword (*pOCISubscriptionUnRegister) (OCISvcCtx *, OCISubscription *, OCIError *, ub4);
static sword (*pOCISvcCtxToLda) (OCISvcCtx *, OCIError *, Lda_Def *);
static sword (*pOCITableDelete) (OCIEnv *, OCIError *, sb4, OCITable *);
static sword (*pOCITableExists) (OCIEnv *, OCIError *, OCITable *, sb4, boolean *);
static sword (*pOCITableFirst) (OCIEnv *, OCIError *, OCITable *, sb4 *);
static sword (*pOCITableLast) (OCIEnv *, OCIError *, OCITable *, sb4 *);
static sword (*pOCITableNext) (OCIEnv *, OCIError *, sb4, OCITable *, sb4 *, boolean *);
static sword (*pOCITablePrev) (OCIEnv *, OCIError *, sb4, OCITable *, sb4 *, boolean *);
static sword (*pOCITableSize) (OCIEnv *, OCIError *, OCITable *, sb4 *);
static sword (*pOCITerminate) (ub4);
static sword (*pOCIThreadClose) (void *, OCIError *, OCIThreadHandle *);
static sword (*pOCIThreadCreate) (void *, OCIError *, void (*) (void *), void *, OCIThreadId *, OCIThreadHandle *);
static sword (*pOCIThreadHandleGet) (void *, OCIError *, OCIThreadHandle *);
static sword (*pOCIThreadHndDestroy) (void *, OCIError *, OCIThreadHandle **);
static sword (*pOCIThreadHndInit) (void *, OCIError *, OCIThreadHandle **);
static sword (*pOCIThreadIdDestroy) (void *, OCIError *, OCIThreadId **);
static sword (*pOCIThreadIdGet) (void *, OCIError *, OCIThreadId *);
static sword (*pOCIThreadIdInit) (void *, OCIError *, OCIThreadId **);
static sword (*pOCIThreadIdNull) (void *, OCIError *, OCIThreadId *, boolean *);
static sword (*pOCIThreadIdSame) (void *, OCIError *, OCIThreadId *, OCIThreadId *, boolean *);
static sword (*pOCIThreadIdSetNull) (void *, OCIError *, OCIThreadId *);
static sword (*pOCIThreadIdSet) (void *, OCIError *, OCIThreadId *, OCIThreadId *);
static sword (*pOCIThreadInit) (void *, OCIError *);
static sword (*pOCIThreadJoin) (void *, OCIError *, OCIThreadHandle *);
static sword (*pOCIThreadKeyDestroy) (void *, OCIError *, OCIThreadKey **);
static sword (*pOCIThreadKeyGet) (void *, OCIError *, OCIThreadKey *, void **);
static sword (*pOCIThreadKeyInit) (void *, OCIError *, OCIThreadKey **, OCIThreadKeyDestFunc);
static sword (*pOCIThreadKeySet) (void *, OCIError *, OCIThreadKey *, void *);
static sword (*pOCIThreadMutexAcquire) (void *, OCIError *, OCIThreadMutex *);
static sword (*pOCIThreadMutexDestroy) (void *, OCIError *, OCIThreadMutex **);
static sword (*pOCIThreadMutexInit) (void *, OCIError *, OCIThreadMutex **);
static sword (*pOCIThreadMutexRelease) (void *, OCIError *, OCIThreadMutex *);
static sword (*pOCIThreadTerm) (void *, OCIError *);
static sword (*pOCITransCommit) (OCISvcCtx *, OCIError *, ub4);
static sword (*pOCITransDetach) (OCISvcCtx *, OCIError *, ub4);
static sword (*pOCITransForget) (OCISvcCtx *, OCIError *, ub4);
static sword (*pOCITransPrepare) (OCISvcCtx *, OCIError *, ub4);
static sword (*pOCITransRollback) (OCISvcCtx *, OCIError *, ub4);
static sword (*pOCITransStart) (OCISvcCtx *, OCIError *, uword, ub4);
static sword (*pOCITypeArrayByName) (OCIEnv *, OCIError *, OCISvcCtx *, ub4, text **, ub4 *, text **, ub4 *, text **, ub4 *, OCIDuration, OCITypeGetOpt, OCIType **);
static sword (*pOCITypeArrayByRef) (OCIEnv *, OCIError *, ub4, OCIRef **, OCIDuration, OCITypeGetOpt, OCIType **);
static sword (*pOCITypeAttrByName) (OCIEnv *, OCIError *, OCIType *, text *, ub4, OCITypeElem **);
static sword (*pOCITypeAttrNext) (OCIEnv *, OCIError *, OCITypeIter *, OCITypeElem **);
static sword (*pOCITypeByName) (OCIEnv *, OCIError *, OCISvcCtx *, text *, ub4, text *, ub4, text *, ub4, OCIDuration, OCITypeGetOpt, OCIType **);
static sword (*pOCITypeByRef) (OCIEnv *, OCIError *, OCIRef *, OCIDuration, OCITypeGetOpt, OCIType **);
static sword (*pOCITypeCollElem) (OCIEnv *, OCIError *, OCIType *, OCITypeElem **);
static sword (*pOCITypeCollExtTypeCode) (OCIEnv *, OCIError *, OCIType *, OCITypeCode *);
static sword (*pOCITypeCollSize) (OCIEnv *, OCIError *, OCIType *, ub4 *);
static sword (*pOCITypeElemParameterizedType) (OCIEnv *, OCIError *, OCITypeElem *, OCIType **);
static sword (*pOCITypeElemType) (OCIEnv *, OCIError *, OCITypeElem *, OCIType **);
static sword (*pOCITypeIterFree) (OCIEnv *, OCIError *, OCITypeIter *);
static sword (*pOCITypeIterNew) (OCIEnv *, OCIError *, OCIType *, OCITypeIter **);
static sword (*pOCITypeIterSet) (OCIEnv *, OCIError *, OCIType *, OCITypeIter *);
static sword (*pOCITypeMethodByName) (OCIEnv *, OCIError *, OCIType *, text *, ub4, OCITypeMethod **);
static sword (*pOCITypeMethodMap) (OCIEnv *, OCIError *, OCIType *, OCITypeMethod **);
static sword (*pOCITypeMethodNext) (OCIEnv *, OCIError *, OCITypeIter *, OCITypeMethod **);
static sword (*pOCITypeMethodOrder) (OCIEnv *, OCIError *, OCIType *, OCITypeMethod **);
static sword (*pOCITypeParamByName) (OCIEnv *, OCIError *, OCITypeMethod *, text *, ub4, OCITypeElem **);
static sword (*pOCITypeParamByPos) (OCIEnv *, OCIError *, OCITypeMethod *, ub4, OCITypeElem **);
static sword (*pOCITypeParamPos) (OCIEnv *, OCIError *, OCITypeMethod *, text *, ub4, ub4 *, OCITypeElem **);
static sword (*pOCITypeResult) (OCIEnv *, OCIError *, OCITypeMethod *, OCITypeElem **);
static sword (*pOCITypeVTInit) (OCIEnv *, OCIError *);
static sword (*pOCITypeVTInsert) (OCIEnv *, OCIError *, text *, ub4, text *, ub4, text *, ub4);
static sword (*pOCITypeVTSelect) (OCIEnv *, OCIError *, text *, ub4, text *, ub4, text **, ub4 *, ub2 *);
static sword (*pOCIUnicodeToCharSet) (void *, OraText *, size_t, ub2 *, size_t, size_t *);
static sword (*pOCIUserCallbackGet) (void *, ub4, void *, ub4, ub4, OCIUserCallback *, void **);
static sword (*pOCIUserCallbackRegister) (void *, ub4, void *, OCIUserCallback, void *, ub4, ub4);
static sword (*pOCIWideCharInSizeToMultiByte) (void *, OraText *, size_t, OCIWchar *, size_t, size_t *);
static sword (*pOCIWideCharToMultiByte) (void *, OraText *, OCIWchar *, size_t *);
static text *(*pOCIStringPtr) (OCIEnv *, OCIString *);
static text *(*pOCITypeElemDefaultValue) (OCIEnv *, OCIError *, OCITypeElem *, ub4 *);
static text *(*pOCITypeElemName) (OCIEnv *, OCIError *, OCITypeElem *, ub4 *);
static text *(*pOCITypeMethodName) (OCIEnv *, OCIError *, OCITypeMethod *, ub4 *);
static text *(*pOCITypeName) (OCIEnv *, OCIError *, OCIType *, ub4 *);
static text *(*pOCITypeSchema) (OCIEnv *, OCIError *, OCIType *, ub4 *);
static text *(*pOCITypeVersion) (OCIEnv *, OCIError *, OCIType *, ub4 *);
static ub1 *(*pOCIRawPtr) (OCIEnv *, OCIRaw *);
static ub1 (*pOCITypeElemNumPrec) (OCIEnv *, OCIError *, OCITypeElem *);
static ub2 (*pOCITypeElemCharSetForm) (OCIEnv *, OCIError *, OCITypeElem *);
static ub2 (*pOCITypeElemCharSetID) (OCIEnv *, OCIError *, OCITypeElem *);
static ub4 (*pOCIRawSize) (OCIEnv *, OCIRaw *);
static ub4 (*pOCIRefHexSize) (OCIEnv *, OCIRef *);
static ub4 (*pOCIStringSize) (OCIEnv *, OCIString *);
static ub4 (*pOCITypeAttrs) (OCIEnv *, OCIError *, OCIType *);
static ub4 (*pOCITypeElemFlags) (OCIEnv *, OCIError *, OCITypeElem *);
static ub4 (*pOCITypeElemLength) (OCIEnv *, OCIError *, OCITypeElem *);
static ub4 (*pOCITypeMethodOverload) (OCIEnv *, OCIError *, OCIType *, text *, ub4);
static ub4 (*pOCITypeMethodParams) (OCIEnv *, OCIError *, OCITypeMethod *);
static ub4 (*pOCITypeMethods) (OCIEnv *, OCIError *, OCIType *);
static void (*pOCINumberSetPi) (OCIError *, OCINumber *);
static void (*pOCINumberSetZero) (OCIError *, OCINumber *);
static void (*pOCIRefClear) (OCIEnv *, OCIRef *);
static void (*pOCIThreadProcessInit) (/* ??? */);

int _init( void )
{
    pOCIAQDeq				    = dlsym( RTLD_NEXT, "OCIAQDeq");
    pOCIAQEnq				    = dlsym( RTLD_NEXT, "OCIAQEnq");
    pOCIAQListen			    = dlsym( RTLD_NEXT, "OCIAQListen");
    pOCIAttrGet				    = dlsym( RTLD_NEXT, "OCIAttrGet");
    pOCIAttrSet				    = dlsym( RTLD_NEXT, "OCIAttrSet");
    pOCIBindArrayOfStruct		    = dlsym( RTLD_NEXT, "OCIBindArrayOfStruct");
    pOCIBindByName			    = dlsym( RTLD_NEXT, "OCIBindByName");
    pOCIBindByPos			    = dlsym( RTLD_NEXT, "OCIBindByPos");
    pOCIBindDynamic			    = dlsym( RTLD_NEXT, "OCIBindDynamic");
    pOCIBindObject			    = dlsym( RTLD_NEXT, "OCIBindObject");
    pOCIBreak				    = dlsym( RTLD_NEXT, "OCIBreak");
    pOCICacheFlush			    = dlsym( RTLD_NEXT, "OCICacheFlush");
    pOCICacheFlushRefresh		    = dlsym( RTLD_NEXT, "OCICacheFlushRefresh");
    pOCICacheFree			    = dlsym( RTLD_NEXT, "OCICacheFree");
    pOCICacheGetObjects			    = dlsym( RTLD_NEXT, "OCICacheGetObjects");
    pOCICacheRefresh			    = dlsym( RTLD_NEXT, "OCICacheRefresh");
    pOCICacheRegister			    = dlsym( RTLD_NEXT, "OCICacheRegister");
    pOCICacheUnmark			    = dlsym( RTLD_NEXT, "OCICacheUnmark");
    pOCICacheUnpin			    = dlsym( RTLD_NEXT, "OCICacheUnpin");
    pOCICharSetConversionIsReplacementUsed  = dlsym( RTLD_NEXT, "OCICharSetConversionIsReplacementUsed");
    pOCICharSetToUnicode		    = dlsym( RTLD_NEXT, "OCICharSetToUnicode");
    pOCICollAppend			    = dlsym( RTLD_NEXT, "OCICollAppend");
    pOCICollAssign			    = dlsym( RTLD_NEXT, "OCICollAssign");
    pOCICollAssignElem			    = dlsym( RTLD_NEXT, "OCICollAssignElem");
    pOCICollGetElem			    = dlsym( RTLD_NEXT, "OCICollGetElem");
    pOCICollIsLocator			    = dlsym( RTLD_NEXT, "OCICollIsLocator");
    pOCICollMax				    = dlsym( RTLD_NEXT, "OCICollMax");
    pOCICollSize			    = dlsym( RTLD_NEXT, "OCICollSize");
    pOCICollTrim			    = dlsym( RTLD_NEXT, "OCICollTrim");
    pOCIContextClearValue		    = dlsym( RTLD_NEXT, "OCIContextClearValue");
    pOCIContextGenerateKey		    = dlsym( RTLD_NEXT, "OCIContextGenerateKey");
    pOCIContextGetValue			    = dlsym( RTLD_NEXT, "OCIContextGetValue");
    pOCIContextSetValue			    = dlsym( RTLD_NEXT, "OCIContextSetValue");
    pOCIDateAddDays			    = dlsym( RTLD_NEXT, "OCIDateAddDays");
    pOCIDateAddMonths			    = dlsym( RTLD_NEXT, "OCIDateAddMonths");
    pOCIDateAssign			    = dlsym( RTLD_NEXT, "OCIDateAssign");
    pOCIDateCheck			    = dlsym( RTLD_NEXT, "OCIDateCheck");
    pOCIDateCompare			    = dlsym( RTLD_NEXT, "OCIDateCompare");
    pOCIDateDaysBetween			    = dlsym( RTLD_NEXT, "OCIDateDaysBetween");
    pOCIDateFromText			    = dlsym( RTLD_NEXT, "OCIDateFromText");
    pOCIDateLastDay			    = dlsym( RTLD_NEXT, "OCIDateLastDay");
    pOCIDateNextDay			    = dlsym( RTLD_NEXT, "OCIDateNextDay");
    pOCIDateSysDate			    = dlsym( RTLD_NEXT, "OCIDateSysDate");
    pOCIDateTimeAssign			    = dlsym( RTLD_NEXT, "OCIDateTimeAssign");
    pOCIDateTimeCheck			    = dlsym( RTLD_NEXT, "OCIDateTimeCheck");
    pOCIDateTimeCompare			    = dlsym( RTLD_NEXT, "OCIDateTimeCompare");
    pOCIDateTimeConvert			    = dlsym( RTLD_NEXT, "OCIDateTimeConvert");
    pOCIDateTimeFromText		    = dlsym( RTLD_NEXT, "OCIDateTimeFromText");
    pOCIDateTimeGetDate			    = dlsym( RTLD_NEXT, "OCIDateTimeGetDate");
    pOCIDateTimeGetTime			    = dlsym( RTLD_NEXT, "OCIDateTimeGetTime");
    pOCIDateTimeGetTimeZone		    = dlsym( RTLD_NEXT, "OCIDateTimeGetTimeZone");
    pOCIDateTimeIntervalAdd		    = dlsym( RTLD_NEXT, "OCIDateTimeIntervalAdd");
    pOCIDateTimeIntervalSub		    = dlsym( RTLD_NEXT, "OCIDateTimeIntervalSub");
    pOCIDateTimeSetDate			    = dlsym( RTLD_NEXT, "OCIDateTimeSetDate");
    pOCIDateTimeSetTime			    = dlsym( RTLD_NEXT, "OCIDateTimeSetTime");
    pOCIDateTimeSetTimeZone		    = dlsym( RTLD_NEXT, "OCIDateTimeSetTimeZone");
    pOCIDateTimeSubtract		    = dlsym( RTLD_NEXT, "OCIDateTimeSubtract");
    pOCIDateTimeSysTimeStamp		    = dlsym( RTLD_NEXT, "OCIDateTimeSysTimeStamp");
    pOCIDateTimeToText			    = dlsym( RTLD_NEXT, "OCIDateTimeToText");
    pOCIDateToText			    = dlsym( RTLD_NEXT, "OCIDateToText");
    pOCIDateZoneToZone			    = dlsym( RTLD_NEXT, "OCIDateZoneToZone");
    pOCIDefineArrayOfStruct		    = dlsym( RTLD_NEXT, "OCIDefineArrayOfStruct");
    pOCIDefineByPos			    = dlsym( RTLD_NEXT, "OCIDefineByPos");
    pOCIDefineDynamic			    = dlsym( RTLD_NEXT, "OCIDefineDynamic");
    pOCIDefineObject			    = dlsym( RTLD_NEXT, "OCIDefineObject");
    pOCIDescribeAny			    = dlsym( RTLD_NEXT, "OCIDescribeAny");
    pOCIDescriptorAlloc			    = dlsym( RTLD_NEXT, "OCIDescriptorAlloc");
    pOCIDescriptorFree			    = dlsym( RTLD_NEXT, "OCIDescriptorFree");
    pOCIDirPathAbort			    = dlsym( RTLD_NEXT, "OCIDirPathAbort");
    pOCIDirPathColArrayEntryGet		    = dlsym( RTLD_NEXT, "OCIDirPathColArrayEntryGet");
    pOCIDirPathColArrayEntrySet		    = dlsym( RTLD_NEXT, "OCIDirPathColArrayEntrySet");
    pOCIDirPathColArrayReset		    = dlsym( RTLD_NEXT, "OCIDirPathColArrayReset");
    pOCIDirPathColArrayRowGet		    = dlsym( RTLD_NEXT, "OCIDirPathColArrayRowGet");
    pOCIDirPathColArrayToStream		    = dlsym( RTLD_NEXT, "OCIDirPathColArrayToStream");
    pOCIDirPathDataSave			    = dlsym( RTLD_NEXT, "OCIDirPathDataSave");
    pOCIDirPathFinish			    = dlsym( RTLD_NEXT, "OCIDirPathFinish");
    pOCIDirPathFlushRow			    = dlsym( RTLD_NEXT, "OCIDirPathFlushRow");
    pOCIDirPathLoadStream		    = dlsym( RTLD_NEXT, "OCIDirPathLoadStream");
    pOCIDirPathPrepare			    = dlsym( RTLD_NEXT, "OCIDirPathPrepare");
    pOCIDirPathStreamReset		    = dlsym( RTLD_NEXT, "OCIDirPathStreamReset");
    pOCIDirPathStreamToStream		    = dlsym( RTLD_NEXT, "OCIDirPathStreamToStream");
    pOCIDurationBegin			    = dlsym( RTLD_NEXT, "OCIDurationBegin");
    pOCIDurationEnd			    = dlsym( RTLD_NEXT, "OCIDurationEnd");
    pOCIDurationGetParent		    = dlsym( RTLD_NEXT, "OCIDurationGetParent");
    pOCIEnvCallback			    = dlsym( RTLD_NEXT, "OCIEnvCallback");
    pOCIEnvCreate			    = dlsym( RTLD_NEXT, "OCIEnvCreate");
    pOCIEnvInit				    = dlsym( RTLD_NEXT, "OCIEnvInit");
    pOCIErrorGet			    = dlsym( RTLD_NEXT, "OCIErrorGet");
    pOCIExtractFromFile			    = dlsym( RTLD_NEXT, "OCIExtractFromFile");
    pOCIExtractFromList			    = dlsym( RTLD_NEXT, "OCIExtractFromList");
    pOCIExtractFromStr			    = dlsym( RTLD_NEXT, "OCIExtractFromStr");
    pOCIExtractInit			    = dlsym( RTLD_NEXT, "OCIExtractInit");
    pOCIExtractReset			    = dlsym( RTLD_NEXT, "OCIExtractReset");
    pOCIExtractSetKey			    = dlsym( RTLD_NEXT, "OCIExtractSetKey");
    pOCIExtractSetNumKeys		    = dlsym( RTLD_NEXT, "OCIExtractSetNumKeys");
    pOCIExtractTerm			    = dlsym( RTLD_NEXT, "OCIExtractTerm");
    pOCIExtractToBool			    = dlsym( RTLD_NEXT, "OCIExtractToBool");
    pOCIExtractToInt			    = dlsym( RTLD_NEXT, "OCIExtractToInt");
    pOCIExtractToList			    = dlsym( RTLD_NEXT, "OCIExtractToList");
    pOCIExtractToOCINum			    = dlsym( RTLD_NEXT, "OCIExtractToOCINum");
    pOCIExtractToStr			    = dlsym( RTLD_NEXT, "OCIExtractToStr");
    pOCIFileClose			    = dlsym( RTLD_NEXT, "OCIFileClose");
    pOCIFileExists			    = dlsym( RTLD_NEXT, "OCIFileExists");
    pOCIFileFlush			    = dlsym( RTLD_NEXT, "OCIFileFlush");
    pOCIFileGetLength			    = dlsym( RTLD_NEXT, "OCIFileGetLength");
    pOCIFileInit			    = dlsym( RTLD_NEXT, "OCIFileInit");
    pOCIFileOpen			    = dlsym( RTLD_NEXT, "OCIFileOpen");
    pOCIFileRead			    = dlsym( RTLD_NEXT, "OCIFileRead");
    pOCIFileSeek			    = dlsym( RTLD_NEXT, "OCIFileSeek");
    pOCIFileTerm			    = dlsym( RTLD_NEXT, "OCIFileTerm");
    pOCIFileWrite			    = dlsym( RTLD_NEXT, "OCIFileWrite");
    pOCIFormatInit			    = dlsym( RTLD_NEXT, "OCIFormatInit");
    pOCIFormatString			    = dlsym( RTLD_NEXT, "OCIFormatString");
    pOCIFormatTChar			    = dlsym( RTLD_NEXT, "OCIFormatTChar");
    pOCIFormatTDouble			    = dlsym( RTLD_NEXT, "OCIFormatTDouble");
    pOCIFormatTDvoid			    = dlsym( RTLD_NEXT, "OCIFormatTDvoid");
    pOCIFormatTEb1			    = dlsym( RTLD_NEXT, "OCIFormatTEb1");
    pOCIFormatTEb2			    = dlsym( RTLD_NEXT, "OCIFormatTEb2");
    pOCIFormatTEb4			    = dlsym( RTLD_NEXT, "OCIFormatTEb4");
    pOCIFormatTEnd			    = dlsym( RTLD_NEXT, "OCIFormatTEnd");
    pOCIFormatTEword			    = dlsym( RTLD_NEXT, "OCIFormatTEword");
    pOCIFormatTSb1			    = dlsym( RTLD_NEXT, "OCIFormatTSb1");
    pOCIFormatTSb2			    = dlsym( RTLD_NEXT, "OCIFormatTSb2");
    pOCIFormatTSb4			    = dlsym( RTLD_NEXT, "OCIFormatTSb4");
    pOCIFormatTSbig_ora			    = dlsym( RTLD_NEXT, "OCIFormatTSbig_ora");
    pOCIFormatTSword			    = dlsym( RTLD_NEXT, "OCIFormatTSword");
    pOCIFormatTText			    = dlsym( RTLD_NEXT, "OCIFormatTText");
    pOCIFormatTUb1			    = dlsym( RTLD_NEXT, "OCIFormatTUb1");
    pOCIFormatTUb2			    = dlsym( RTLD_NEXT, "OCIFormatTUb2");
    pOCIFormatTUb4			    = dlsym( RTLD_NEXT, "OCIFormatTUb4");
    pOCIFormatTUbig_ora			    = dlsym( RTLD_NEXT, "OCIFormatTUbig_ora");
    pOCIFormatTUword			    = dlsym( RTLD_NEXT, "OCIFormatTUword");
    pOCIFormatTerm			    = dlsym( RTLD_NEXT, "OCIFormatTerm");
    pOCIHandleAlloc			    = dlsym( RTLD_NEXT, "OCIHandleAlloc");
    pOCIHandleFree			    = dlsym( RTLD_NEXT, "OCIHandleFree");
    pOCIInitialize			    = dlsym( RTLD_NEXT, "OCIInitialize");
    pOCIIntervalAdd			    = dlsym( RTLD_NEXT, "OCIIntervalAdd");
    pOCIIntervalAssign			    = dlsym( RTLD_NEXT, "OCIIntervalAssign");
    pOCIIntervalCheck			    = dlsym( RTLD_NEXT, "OCIIntervalCheck");
    pOCIIntervalCompare			    = dlsym( RTLD_NEXT, "OCIIntervalCompare");
    pOCIIntervalDivide			    = dlsym( RTLD_NEXT, "OCIIntervalDivide");
    pOCIIntervalFromNumber		    = dlsym( RTLD_NEXT, "OCIIntervalFromNumber");
    pOCIIntervalFromText		    = dlsym( RTLD_NEXT, "OCIIntervalFromText");
    pOCIIntervalGetDaySecond		    = dlsym( RTLD_NEXT, "OCIIntervalGetDaySecond");
    pOCIIntervalGetYearMonth		    = dlsym( RTLD_NEXT, "OCIIntervalGetYearMonth");
    pOCIIntervalMultiply		    = dlsym( RTLD_NEXT, "OCIIntervalMultiply");
    pOCIIntervalSetDaySecond		    = dlsym( RTLD_NEXT, "OCIIntervalSetDaySecond");
    pOCIIntervalSetYearMonth		    = dlsym( RTLD_NEXT, "OCIIntervalSetYearMonth");
    pOCIIntervalSubtract		    = dlsym( RTLD_NEXT, "OCIIntervalSubtract");
    pOCIIntervalToNumber		    = dlsym( RTLD_NEXT, "OCIIntervalToNumber");
    pOCIIntervalToText			    = dlsym( RTLD_NEXT, "OCIIntervalToText");
    pOCIIterCreate			    = dlsym( RTLD_NEXT, "OCIIterCreate");
    pOCIIterDelete			    = dlsym( RTLD_NEXT, "OCIIterDelete");
    pOCIIterGetCurrent			    = dlsym( RTLD_NEXT, "OCIIterGetCurrent");
    pOCIIterInit			    = dlsym( RTLD_NEXT, "OCIIterInit");
    pOCIIterNext			    = dlsym( RTLD_NEXT, "OCIIterNext");
    pOCIIterPrev			    = dlsym( RTLD_NEXT, "OCIIterPrev");
    pOCILdaToSvcCtx			    = dlsym( RTLD_NEXT, "OCILdaToSvcCtx");
    pOCILobAppend			    = dlsym( RTLD_NEXT, "OCILobAppend");
    pOCILobAssign			    = dlsym( RTLD_NEXT, "OCILobAssign");
    pOCILobCharSetForm			    = dlsym( RTLD_NEXT, "OCILobCharSetForm");
    pOCILobCharSetId			    = dlsym( RTLD_NEXT, "OCILobCharSetId");
    pOCILobClose			    = dlsym( RTLD_NEXT, "OCILobClose");
    pOCILobCopy				    = dlsym( RTLD_NEXT, "OCILobCopy");
    pOCILobCreateTemporary		    = dlsym( RTLD_NEXT, "OCILobCreateTemporary");
    pOCILobDisableBuffering		    = dlsym( RTLD_NEXT, "OCILobDisableBuffering");
    pOCILobEnableBuffering		    = dlsym( RTLD_NEXT, "OCILobEnableBuffering");
    pOCILobErase			    = dlsym( RTLD_NEXT, "OCILobErase");
    pOCILobFileClose			    = dlsym( RTLD_NEXT, "OCILobFileClose");
    pOCILobFileCloseAll			    = dlsym( RTLD_NEXT, "OCILobFileCloseAll");
    pOCILobFileExists			    = dlsym( RTLD_NEXT, "OCILobFileExists");
    pOCILobFileGetName			    = dlsym( RTLD_NEXT, "OCILobFileGetName");
    pOCILobFileIsOpen			    = dlsym( RTLD_NEXT, "OCILobFileIsOpen");
    pOCILobFileOpen			    = dlsym( RTLD_NEXT, "OCILobFileOpen");
    pOCILobFileSetName			    = dlsym( RTLD_NEXT, "OCILobFileSetName");
    pOCILobFlushBuffer			    = dlsym( RTLD_NEXT, "OCILobFlushBuffer");
    pOCILobFreeTemporary		    = dlsym( RTLD_NEXT, "OCILobFreeTemporary");
    pOCILobGetChunkSize			    = dlsym( RTLD_NEXT, "OCILobGetChunkSize");
    pOCILobGetLength			    = dlsym( RTLD_NEXT, "OCILobGetLength");
    pOCILobIsEqual			    = dlsym( RTLD_NEXT, "OCILobIsEqual");
    pOCILobIsOpen			    = dlsym( RTLD_NEXT, "OCILobIsOpen");
    pOCILobIsTemporary			    = dlsym( RTLD_NEXT, "OCILobIsTemporary");
    pOCILobLoadFromFile			    = dlsym( RTLD_NEXT, "OCILobLoadFromFile");
    pOCILobLocatorAssign		    = dlsym( RTLD_NEXT, "OCILobLocatorAssign");
    pOCILobLocatorIsInit		    = dlsym( RTLD_NEXT, "OCILobLocatorIsInit");
    pOCILobOpen				    = dlsym( RTLD_NEXT, "OCILobOpen");
    pOCILobRead				    = dlsym( RTLD_NEXT, "OCILobRead");
    pOCILobTrim				    = dlsym( RTLD_NEXT, "OCILobTrim");
    pOCILobWrite			    = dlsym( RTLD_NEXT, "OCILobWrite");
    pOCILobWriteAppend			    = dlsym( RTLD_NEXT, "OCILobWriteAppend");
    pOCILogoff				    = dlsym( RTLD_NEXT, "OCILogoff");
    pOCILogon				    = dlsym( RTLD_NEXT, "OCILogon");
    pOCIMemoryAlloc			    = dlsym( RTLD_NEXT, "OCIMemoryAlloc");
    pOCIMemoryFree			    = dlsym( RTLD_NEXT, "OCIMemoryFree");
    pOCIMemoryResize			    = dlsym( RTLD_NEXT, "OCIMemoryResize");
    pOCIMemorySetCurrentIDs		    = dlsym( RTLD_NEXT, "OCIMemorySetCurrentIDs");
    pOCIMessageClose			    = dlsym( RTLD_NEXT, "OCIMessageClose");
    pOCIMessageGet			    = dlsym( RTLD_NEXT, "OCIMessageGet");
    pOCIMessageOpen			    = dlsym( RTLD_NEXT, "OCIMessageOpen");
    pOCIMultiByteInSizeToWideChar	    = dlsym( RTLD_NEXT, "OCIMultiByteInSizeToWideChar");
    pOCIMultiByteStrCaseConversion	    = dlsym( RTLD_NEXT, "OCIMultiByteStrCaseConversion");
    pOCIMultiByteStrcat			    = dlsym( RTLD_NEXT, "OCIMultiByteStrcat");
    pOCIMultiByteStrcmp			    = dlsym( RTLD_NEXT, "OCIMultiByteStrcmp");
    pOCIMultiByteStrcpy			    = dlsym( RTLD_NEXT, "OCIMultiByteStrcpy");
    pOCIMultiByteStrlen			    = dlsym( RTLD_NEXT, "OCIMultiByteStrlen");
    pOCIMultiByteStrnDisplayLength	    = dlsym( RTLD_NEXT, "OCIMultiByteStrnDisplayLength");
    pOCIMultiByteStrncat		    = dlsym( RTLD_NEXT, "OCIMultiByteStrncat");
    pOCIMultiByteStrncmp		    = dlsym( RTLD_NEXT, "OCIMultiByteStrncmp");
    pOCIMultiByteStrncpy		    = dlsym( RTLD_NEXT, "OCIMultiByteStrncpy");
    pOCIMultiByteToWideChar		    = dlsym( RTLD_NEXT, "OCIMultiByteToWideChar");
    pOCINlsGetInfo			    = dlsym( RTLD_NEXT, "OCINlsGetInfo");
    pOCINumberAbs			    = dlsym( RTLD_NEXT, "OCINumberAbs");
    pOCINumberAdd			    = dlsym( RTLD_NEXT, "OCINumberAdd");
    pOCINumberArcCos			    = dlsym( RTLD_NEXT, "OCINumberArcCos");
    pOCINumberArcSin			    = dlsym( RTLD_NEXT, "OCINumberArcSin");
    pOCINumberArcTan			    = dlsym( RTLD_NEXT, "OCINumberArcTan");
    pOCINumberArcTan2			    = dlsym( RTLD_NEXT, "OCINumberArcTan2");
    pOCINumberAssign			    = dlsym( RTLD_NEXT, "OCINumberAssign");
    pOCINumberCeil			    = dlsym( RTLD_NEXT, "OCINumberCeil");
    pOCINumberCmp			    = dlsym( RTLD_NEXT, "OCINumberCmp");
    pOCINumberCos			    = dlsym( RTLD_NEXT, "OCINumberCos");
    pOCINumberDec			    = dlsym( RTLD_NEXT, "OCINumberDec");
    pOCINumberDiv			    = dlsym( RTLD_NEXT, "OCINumberDiv");
    pOCINumberExp			    = dlsym( RTLD_NEXT, "OCINumberExp");
    pOCINumberFloor			    = dlsym( RTLD_NEXT, "OCINumberFloor");
    pOCINumberFromInt			    = dlsym( RTLD_NEXT, "OCINumberFromInt");
    pOCINumberFromReal			    = dlsym( RTLD_NEXT, "OCINumberFromReal");
    pOCINumberFromText			    = dlsym( RTLD_NEXT, "OCINumberFromText");
    pOCINumberHypCos			    = dlsym( RTLD_NEXT, "OCINumberHypCos");
    pOCINumberHypSin			    = dlsym( RTLD_NEXT, "OCINumberHypSin");
    pOCINumberHypTan			    = dlsym( RTLD_NEXT, "OCINumberHypTan");
    pOCINumberInc			    = dlsym( RTLD_NEXT, "OCINumberInc");
    pOCINumberIntPower			    = dlsym( RTLD_NEXT, "OCINumberIntPower");
    pOCINumberIsInt			    = dlsym( RTLD_NEXT, "OCINumberIsInt");
    pOCINumberIsZero			    = dlsym( RTLD_NEXT, "OCINumberIsZero");
    pOCINumberLn			    = dlsym( RTLD_NEXT, "OCINumberLn");
    pOCINumberLog			    = dlsym( RTLD_NEXT, "OCINumberLog");
    pOCINumberMod			    = dlsym( RTLD_NEXT, "OCINumberMod");
    pOCINumberMul			    = dlsym( RTLD_NEXT, "OCINumberMul");
    pOCINumberNeg			    = dlsym( RTLD_NEXT, "OCINumberNeg");
    pOCINumberPower			    = dlsym( RTLD_NEXT, "OCINumberPower");
    pOCINumberPrec			    = dlsym( RTLD_NEXT, "OCINumberPrec");
    pOCINumberRound			    = dlsym( RTLD_NEXT, "OCINumberRound");
    pOCINumberSetPi			    = dlsym( RTLD_NEXT, "OCINumberSetPi");
    pOCINumberSetZero			    = dlsym( RTLD_NEXT, "OCINumberSetZero");
    pOCINumberShift			    = dlsym( RTLD_NEXT, "OCINumberShift");
    pOCINumberSign			    = dlsym( RTLD_NEXT, "OCINumberSign");
    pOCINumberSin			    = dlsym( RTLD_NEXT, "OCINumberSin");
    pOCINumberSqrt			    = dlsym( RTLD_NEXT, "OCINumberSqrt");
    pOCINumberSub			    = dlsym( RTLD_NEXT, "OCINumberSub");
    pOCINumberTan			    = dlsym( RTLD_NEXT, "OCINumberTan");
    pOCINumberToInt			    = dlsym( RTLD_NEXT, "OCINumberToInt");
    pOCINumberToReal			    = dlsym( RTLD_NEXT, "OCINumberToReal");
    pOCINumberToText			    = dlsym( RTLD_NEXT, "OCINumberToText");
    pOCINumberTrunc			    = dlsym( RTLD_NEXT, "OCINumberTrunc");
    pOCIObjectAlwaysLatest		    = dlsym( RTLD_NEXT, "OCIObjectAlwaysLatest");
    pOCIObjectArrayPin			    = dlsym( RTLD_NEXT, "OCIObjectArrayPin");
    pOCIObjectCopy			    = dlsym( RTLD_NEXT, "OCIObjectCopy");
    pOCIObjectExists			    = dlsym( RTLD_NEXT, "OCIObjectExists");
    pOCIObjectFlush			    = dlsym( RTLD_NEXT, "OCIObjectFlush");
    pOCIObjectFlushRefresh		    = dlsym( RTLD_NEXT, "OCIObjectFlushRefresh");
    pOCIObjectFree			    = dlsym( RTLD_NEXT, "OCIObjectFree");
    pOCIObjectGetInd			    = dlsym( RTLD_NEXT, "OCIObjectGetInd");
    pOCIObjectGetObjectRef		    = dlsym( RTLD_NEXT, "OCIObjectGetObjectRef");
    pOCIObjectGetPrimaryKeyTypeRef	    = dlsym( RTLD_NEXT, "OCIObjectGetPrimaryKeyTypeRef");
    pOCIObjectGetProperty		    = dlsym( RTLD_NEXT, "OCIObjectGetProperty");
    pOCIObjectGetTypeRef		    = dlsym( RTLD_NEXT, "OCIObjectGetTypeRef");
    pOCIObjectIsDirtied			    = dlsym( RTLD_NEXT, "OCIObjectIsDirtied");
    pOCIObjectIsDirty			    = dlsym( RTLD_NEXT, "OCIObjectIsDirty");
    pOCIObjectIsLoaded			    = dlsym( RTLD_NEXT, "OCIObjectIsLoaded");
    pOCIObjectIsLocked			    = dlsym( RTLD_NEXT, "OCIObjectIsLocked");
    pOCIObjectLock			    = dlsym( RTLD_NEXT, "OCIObjectLock");
    pOCIObjectLockNoWait		    = dlsym( RTLD_NEXT, "OCIObjectLockNoWait");
    pOCIObjectMakeObjectRef		    = dlsym( RTLD_NEXT, "OCIObjectMakeObjectRef");
    pOCIObjectMarkDelete		    = dlsym( RTLD_NEXT, "OCIObjectMarkDelete");
    pOCIObjectMarkDeleteByRef		    = dlsym( RTLD_NEXT, "OCIObjectMarkDeleteByRef");
    pOCIObjectMarkUpdate		    = dlsym( RTLD_NEXT, "OCIObjectMarkUpdate");
    pOCIObjectNew			    = dlsym( RTLD_NEXT, "OCIObjectNew");
    pOCIObjectNotAlwaysLatest		    = dlsym( RTLD_NEXT, "OCIObjectNotAlwaysLatest");
    pOCIObjectPin			    = dlsym( RTLD_NEXT, "OCIObjectPin");
    pOCIObjectPinCountReset		    = dlsym( RTLD_NEXT, "OCIObjectPinCountReset");
    pOCIObjectPinTable			    = dlsym( RTLD_NEXT, "OCIObjectPinTable");
    pOCIObjectRefresh			    = dlsym( RTLD_NEXT, "OCIObjectRefresh");
    pOCIObjectUnmark			    = dlsym( RTLD_NEXT, "OCIObjectUnmark");
    pOCIObjectUnmarkByRef		    = dlsym( RTLD_NEXT, "OCIObjectUnmarkByRef");
    pOCIObjectUnpin			    = dlsym( RTLD_NEXT, "OCIObjectUnpin");
    pOCIParamGet			    = dlsym( RTLD_NEXT, "OCIParamGet");
    pOCIParamSet			    = dlsym( RTLD_NEXT, "OCIParamSet");
    pOCIPasswordChange			    = dlsym( RTLD_NEXT, "OCIPasswordChange");
    pOCIPicklerFdoFree			    = dlsym( RTLD_NEXT, "OCIPicklerFdoFree");
    pOCIPicklerFdoInit			    = dlsym( RTLD_NEXT, "OCIPicklerFdoInit");
    pOCIPicklerImageAddNullScalar	    = dlsym( RTLD_NEXT, "OCIPicklerImageAddNullScalar");
    pOCIPicklerImageAddScalar		    = dlsym( RTLD_NEXT, "OCIPicklerImageAddScalar");
    pOCIPicklerImageCollAddScalar	    = dlsym( RTLD_NEXT, "OCIPicklerImageCollAddScalar");
    pOCIPicklerImageCollBegin		    = dlsym( RTLD_NEXT, "OCIPicklerImageCollBegin");
    pOCIPicklerImageCollBeginScan	    = dlsym( RTLD_NEXT, "OCIPicklerImageCollBeginScan");
    pOCIPicklerImageCollEnd		    = dlsym( RTLD_NEXT, "OCIPicklerImageCollEnd");
    pOCIPicklerImageCollGetScalar	    = dlsym( RTLD_NEXT, "OCIPicklerImageCollGetScalar");
    pOCIPicklerImageCollGetScalarSize	    = dlsym( RTLD_NEXT, "OCIPicklerImageCollGetScalarSize");
    pOCIPicklerImageFree		    = dlsym( RTLD_NEXT, "OCIPicklerImageFree");
    pOCIPicklerImageGenerate		    = dlsym( RTLD_NEXT, "OCIPicklerImageGenerate");
    pOCIPicklerImageGetScalar		    = dlsym( RTLD_NEXT, "OCIPicklerImageGetScalar");
    pOCIPicklerImageGetScalarSize	    = dlsym( RTLD_NEXT, "OCIPicklerImageGetScalarSize");
    pOCIPicklerImageInit		    = dlsym( RTLD_NEXT, "OCIPicklerImageInit");
    pOCIPicklerTdsAddAttr		    = dlsym( RTLD_NEXT, "OCIPicklerTdsAddAttr");
    pOCIPicklerTdsCreateElement		    = dlsym( RTLD_NEXT, "OCIPicklerTdsCreateElement");
    pOCIPicklerTdsCreateElementChar	    = dlsym( RTLD_NEXT, "OCIPicklerTdsCreateElementChar");
    pOCIPicklerTdsCreateElementNumber	    = dlsym( RTLD_NEXT, "OCIPicklerTdsCreateElementNumber");
    pOCIPicklerTdsCreateElementRaw	    = dlsym( RTLD_NEXT, "OCIPicklerTdsCreateElementRaw");
    pOCIPicklerTdsCreateElementVarchar	    = dlsym( RTLD_NEXT, "OCIPicklerTdsCreateElementVarchar");
    pOCIPicklerTdsCtxFree		    = dlsym( RTLD_NEXT, "OCIPicklerTdsCtxFree");
    pOCIPicklerTdsCtxInit		    = dlsym( RTLD_NEXT, "OCIPicklerTdsCtxInit");
    pOCIPicklerTdsFree			    = dlsym( RTLD_NEXT, "OCIPicklerTdsFree");
    pOCIPicklerTdsGenerate		    = dlsym( RTLD_NEXT, "OCIPicklerTdsGenerate");
    pOCIPicklerTdsGetAttr		    = dlsym( RTLD_NEXT, "OCIPicklerTdsGetAttr");
    pOCIPicklerTdsInit			    = dlsym( RTLD_NEXT, "OCIPicklerTdsInit");
    pOCIRawAllocSize			    = dlsym( RTLD_NEXT, "OCIRawAllocSize");
    pOCIRawAssignBytes			    = dlsym( RTLD_NEXT, "OCIRawAssignBytes");
    pOCIRawAssignRaw			    = dlsym( RTLD_NEXT, "OCIRawAssignRaw");
    pOCIRawPtr				    = dlsym( RTLD_NEXT, "OCIRawPtr");
    pOCIRawResize			    = dlsym( RTLD_NEXT, "OCIRawResize");
    pOCIRawSize				    = dlsym( RTLD_NEXT, "OCIRawSize");
    pOCIRefAssign			    = dlsym( RTLD_NEXT, "OCIRefAssign");
    pOCIRefClear			    = dlsym( RTLD_NEXT, "OCIRefClear");
    pOCIRefFromHex			    = dlsym( RTLD_NEXT, "OCIRefFromHex");
    pOCIRefHexSize			    = dlsym( RTLD_NEXT, "OCIRefHexSize");
    pOCIRefIsEqual			    = dlsym( RTLD_NEXT, "OCIRefIsEqual");
    pOCIRefIsNull			    = dlsym( RTLD_NEXT, "OCIRefIsNull");
    pOCIRefToHex			    = dlsym( RTLD_NEXT, "OCIRefToHex");
    pOCIReset				    = dlsym( RTLD_NEXT, "OCIReset");
    pOCIResultSetToStmt			    = dlsym( RTLD_NEXT, "OCIResultSetToStmt");
    pOCISecurityAbortIdentity		    = dlsym( RTLD_NEXT, "OCISecurityAbortIdentity");
    pOCISecurityClosePersona		    = dlsym( RTLD_NEXT, "OCISecurityClosePersona");
    pOCISecurityCloseWallet		    = dlsym( RTLD_NEXT, "OCISecurityCloseWallet");
    pOCISecurityCreateIdentity		    = dlsym( RTLD_NEXT, "OCISecurityCreateIdentity");
    pOCISecurityCreatePersona		    = dlsym( RTLD_NEXT, "OCISecurityCreatePersona");
    pOCISecurityCreateWallet		    = dlsym( RTLD_NEXT, "OCISecurityCreateWallet");
    pOCISecurityDeEnvelope		    = dlsym( RTLD_NEXT, "OCISecurityDeEnvelope");
    pOCISecurityDecrypt			    = dlsym( RTLD_NEXT, "OCISecurityDecrypt");
    pOCISecurityDestroyWallet		    = dlsym( RTLD_NEXT, "OCISecurityDestroyWallet");
    pOCISecurityEncrypt			    = dlsym( RTLD_NEXT, "OCISecurityEncrypt");
    pOCISecurityEncryptExpansion	    = dlsym( RTLD_NEXT, "OCISecurityEncryptExpansion");
    pOCISecurityEnvelope		    = dlsym( RTLD_NEXT, "OCISecurityEnvelope");
    pOCISecurityFreeIdentity		    = dlsym( RTLD_NEXT, "OCISecurityFreeIdentity");
    pOCISecurityGetIdentity		    = dlsym( RTLD_NEXT, "OCISecurityGetIdentity");
    pOCISecurityGetProtection		    = dlsym( RTLD_NEXT, "OCISecurityGetProtection");
    pOCISecurityHash			    = dlsym( RTLD_NEXT, "OCISecurityHash");
    pOCISecurityHashExpansion		    = dlsym( RTLD_NEXT, "OCISecurityHashExpansion");
    pOCISecurityInitBlock		    = dlsym( RTLD_NEXT, "OCISecurityInitBlock");
    pOCISecurityInitialize		    = dlsym( RTLD_NEXT, "OCISecurityInitialize");
    pOCISecurityKeyedHash		    = dlsym( RTLD_NEXT, "OCISecurityKeyedHash");
    pOCISecurityKeyedHashExpansion	    = dlsym( RTLD_NEXT, "OCISecurityKeyedHashExpansion");
    pOCISecurityOpenPersona		    = dlsym( RTLD_NEXT, "OCISecurityOpenPersona");
    pOCISecurityOpenWallet		    = dlsym( RTLD_NEXT, "OCISecurityOpenWallet");
    pOCISecurityPKDecrypt		    = dlsym( RTLD_NEXT, "OCISecurityPKDecrypt");
    pOCISecurityPKEncryptExpansion	    = dlsym( RTLD_NEXT, "OCISecurityPKEncryptExpansion");
    pOCISecurityPurgeBlock		    = dlsym( RTLD_NEXT, "OCISecurityPurgeBlock");
    pOCISecurityRandomBytes		    = dlsym( RTLD_NEXT, "OCISecurityRandomBytes");
    pOCISecurityRandomNumber		    = dlsym( RTLD_NEXT, "OCISecurityRandomNumber");
    pOCISecurityRemoveIdentity		    = dlsym( RTLD_NEXT, "OCISecurityRemoveIdentity");
    pOCISecurityRemovePersona		    = dlsym( RTLD_NEXT, "OCISecurityRemovePersona");
    pOCISecurityReuseBlock		    = dlsym( RTLD_NEXT, "OCISecurityReuseBlock");
    pOCISecuritySeedRandom		    = dlsym( RTLD_NEXT, "OCISecuritySeedRandom");
    pOCISecuritySetBlock		    = dlsym( RTLD_NEXT, "OCISecuritySetBlock");
    pOCISecuritySetProtection		    = dlsym( RTLD_NEXT, "OCISecuritySetProtection");
    pOCISecuritySign			    = dlsym( RTLD_NEXT, "OCISecuritySign");
    pOCISecuritySignDetExpansion	    = dlsym( RTLD_NEXT, "OCISecuritySignDetExpansion");
    pOCISecuritySignDetached		    = dlsym( RTLD_NEXT, "OCISecuritySignDetached");
    pOCISecuritySignExpansion		    = dlsym( RTLD_NEXT, "OCISecuritySignExpansion");
    pOCISecurityStorePersona		    = dlsym( RTLD_NEXT, "OCISecurityStorePersona");
    pOCISecurityStoreTrustedIdentity	    = dlsym( RTLD_NEXT, "OCISecurityStoreTrustedIdentity");
    pOCISecurityTerminate		    = dlsym( RTLD_NEXT, "OCISecurityTerminate");
    pOCISecurityValidate		    = dlsym( RTLD_NEXT, "OCISecurityValidate");
    pOCISecurityVerify			    = dlsym( RTLD_NEXT, "OCISecurityVerify");
    pOCISecurityVerifyDetached		    = dlsym( RTLD_NEXT, "OCISecurityVerifyDetached");
    pOCISecurity_PKEncrypt		    = dlsym( RTLD_NEXT, "OCISecurity_PKEncrypt");
    pOCIServerAttach			    = dlsym( RTLD_NEXT, "OCIServerAttach");
    pOCIServerDetach			    = dlsym( RTLD_NEXT, "OCIServerDetach");
    pOCIServerVersion			    = dlsym( RTLD_NEXT, "OCIServerVersion");
    pOCISessionBegin			    = dlsym( RTLD_NEXT, "OCISessionBegin");
    pOCISessionEnd			    = dlsym( RTLD_NEXT, "OCISessionEnd");
    pOCIStmtExecute			    = dlsym( RTLD_NEXT, "OCIStmtExecute");
    pOCIStmtFetch			    = dlsym( RTLD_NEXT, "OCIStmtFetch");
    pOCIStmtGetBindInfo			    = dlsym( RTLD_NEXT, "OCIStmtGetBindInfo");
    pOCIStmtGetPieceInfo		    = dlsym( RTLD_NEXT, "OCIStmtGetPieceInfo");
    pOCIStmtPrepare			    = dlsym( RTLD_NEXT, "OCIStmtPrepare");
    pOCIStmtSetPieceInfo		    = dlsym( RTLD_NEXT, "OCIStmtSetPieceInfo");
    pOCIStringAllocSize			    = dlsym( RTLD_NEXT, "OCIStringAllocSize");
    pOCIStringAssign			    = dlsym( RTLD_NEXT, "OCIStringAssign");
    pOCIStringAssignText		    = dlsym( RTLD_NEXT, "OCIStringAssignText");
    pOCIStringPtr			    = dlsym( RTLD_NEXT, "OCIStringPtr");
    pOCIStringResize			    = dlsym( RTLD_NEXT, "OCIStringResize");
    pOCIStringSize			    = dlsym( RTLD_NEXT, "OCIStringSize");
    pOCISubscriptionDisable		    = dlsym( RTLD_NEXT, "OCISubscriptionDisable");
    pOCISubscriptionEnable		    = dlsym( RTLD_NEXT, "OCISubscriptionEnable");
    pOCISubscriptionPost		    = dlsym( RTLD_NEXT, "OCISubscriptionPost");
    pOCISubscriptionRegister		    = dlsym( RTLD_NEXT, "OCISubscriptionRegister");
    pOCISubscriptionUnRegister		    = dlsym( RTLD_NEXT, "OCISubscriptionUnRegister");
    pOCISvcCtxToLda			    = dlsym( RTLD_NEXT, "OCISvcCtxToLda");
    pOCITableDelete			    = dlsym( RTLD_NEXT, "OCITableDelete");
    pOCITableExists			    = dlsym( RTLD_NEXT, "OCITableExists");
    pOCITableFirst			    = dlsym( RTLD_NEXT, "OCITableFirst");
    pOCITableLast			    = dlsym( RTLD_NEXT, "OCITableLast");
    pOCITableNext			    = dlsym( RTLD_NEXT, "OCITableNext");
    pOCITablePrev			    = dlsym( RTLD_NEXT, "OCITablePrev");
    pOCITableSize			    = dlsym( RTLD_NEXT, "OCITableSize");
    pOCITerminate			    = dlsym( RTLD_NEXT, "OCITerminate");
    pOCIThreadClose			    = dlsym( RTLD_NEXT, "OCIThreadClose");
    pOCIThreadCreate			    = dlsym( RTLD_NEXT, "OCIThreadCreate");
    pOCIThreadHandleGet			    = dlsym( RTLD_NEXT, "OCIThreadHandleGet");
    pOCIThreadHndDestroy		    = dlsym( RTLD_NEXT, "OCIThreadHndDestroy");
    pOCIThreadHndInit			    = dlsym( RTLD_NEXT, "OCIThreadHndInit");
    pOCIThreadIdDestroy			    = dlsym( RTLD_NEXT, "OCIThreadIdDestroy");
    pOCIThreadIdGet			    = dlsym( RTLD_NEXT, "OCIThreadIdGet");
    pOCIThreadIdInit			    = dlsym( RTLD_NEXT, "OCIThreadIdInit");
    pOCIThreadIdNull			    = dlsym( RTLD_NEXT, "OCIThreadIdNull");
    pOCIThreadIdSame			    = dlsym( RTLD_NEXT, "OCIThreadIdSame");
    pOCIThreadIdSet			    = dlsym( RTLD_NEXT, "OCIThreadIdSet");
    pOCIThreadIdSetNull			    = dlsym( RTLD_NEXT, "OCIThreadIdSetNull");
    pOCIThreadInit			    = dlsym( RTLD_NEXT, "OCIThreadInit");
    pOCIThreadIsMulti			    = dlsym( RTLD_NEXT, "OCIThreadIsMulti");
    pOCIThreadJoin			    = dlsym( RTLD_NEXT, "OCIThreadJoin");
    pOCIThreadKeyDestroy		    = dlsym( RTLD_NEXT, "OCIThreadKeyDestroy");
    pOCIThreadKeyGet			    = dlsym( RTLD_NEXT, "OCIThreadKeyGet");
    pOCIThreadKeyInit			    = dlsym( RTLD_NEXT, "OCIThreadKeyInit");
    pOCIThreadKeySet			    = dlsym( RTLD_NEXT, "OCIThreadKeySet");
    pOCIThreadMutexAcquire		    = dlsym( RTLD_NEXT, "OCIThreadMutexAcquire");
    pOCIThreadMutexDestroy		    = dlsym( RTLD_NEXT, "OCIThreadMutexDestroy");
    pOCIThreadMutexInit			    = dlsym( RTLD_NEXT, "OCIThreadMutexInit");
    pOCIThreadMutexRelease		    = dlsym( RTLD_NEXT, "OCIThreadMutexRelease");
    pOCIThreadProcessInit		    = dlsym( RTLD_NEXT, "OCIThreadProcessInit");
    pOCIThreadTerm			    = dlsym( RTLD_NEXT, "OCIThreadTerm");
    pOCITransCommit			    = dlsym( RTLD_NEXT, "OCITransCommit");
    pOCITransDetach			    = dlsym( RTLD_NEXT, "OCITransDetach");
    pOCITransForget			    = dlsym( RTLD_NEXT, "OCITransForget");
    pOCITransPrepare			    = dlsym( RTLD_NEXT, "OCITransPrepare");
    pOCITransRollback			    = dlsym( RTLD_NEXT, "OCITransRollback");
    pOCITransStart			    = dlsym( RTLD_NEXT, "OCITransStart");
    pOCITypeArrayByName			    = dlsym( RTLD_NEXT, "OCITypeArrayByName");
    pOCITypeArrayByRef			    = dlsym( RTLD_NEXT, "OCITypeArrayByRef");
    pOCITypeAttrByName			    = dlsym( RTLD_NEXT, "OCITypeAttrByName");
    pOCITypeAttrNext			    = dlsym( RTLD_NEXT, "OCITypeAttrNext");
    pOCITypeAttrs			    = dlsym( RTLD_NEXT, "OCITypeAttrs");
    pOCITypeByName			    = dlsym( RTLD_NEXT, "OCITypeByName");
    pOCITypeByRef			    = dlsym( RTLD_NEXT, "OCITypeByRef");
    pOCITypeCollElem			    = dlsym( RTLD_NEXT, "OCITypeCollElem");
    pOCITypeCollExtTypeCode		    = dlsym( RTLD_NEXT, "OCITypeCollExtTypeCode");
    pOCITypeCollSize			    = dlsym( RTLD_NEXT, "OCITypeCollSize");
    pOCITypeCollTypeCode		    = dlsym( RTLD_NEXT, "OCITypeCollTypeCode");
    pOCITypeElemCharSetForm		    = dlsym( RTLD_NEXT, "OCITypeElemCharSetForm");
    pOCITypeElemCharSetID		    = dlsym( RTLD_NEXT, "OCITypeElemCharSetID");
    pOCITypeElemDefaultValue		    = dlsym( RTLD_NEXT, "OCITypeElemDefaultValue");
    pOCITypeElemExtTypeCode		    = dlsym( RTLD_NEXT, "OCITypeElemExtTypeCode");
    pOCITypeElemFlags			    = dlsym( RTLD_NEXT, "OCITypeElemFlags");
    pOCITypeElemLength			    = dlsym( RTLD_NEXT, "OCITypeElemLength");
    pOCITypeElemName			    = dlsym( RTLD_NEXT, "OCITypeElemName");
    pOCITypeElemNumPrec			    = dlsym( RTLD_NEXT, "OCITypeElemNumPrec");
    pOCITypeElemNumScale		    = dlsym( RTLD_NEXT, "OCITypeElemNumScale");
    pOCITypeElemParamMode		    = dlsym( RTLD_NEXT, "OCITypeElemParamMode");
    pOCITypeElemParameterizedType	    = dlsym( RTLD_NEXT, "OCITypeElemParameterizedType");
    pOCITypeElemType			    = dlsym( RTLD_NEXT, "OCITypeElemType");
    pOCITypeElemTypeCode		    = dlsym( RTLD_NEXT, "OCITypeElemTypeCode");
    pOCITypeIterFree			    = dlsym( RTLD_NEXT, "OCITypeIterFree");
    pOCITypeIterNew			    = dlsym( RTLD_NEXT, "OCITypeIterNew");
    pOCITypeIterSet			    = dlsym( RTLD_NEXT, "OCITypeIterSet");
    pOCITypeMethodByName		    = dlsym( RTLD_NEXT, "OCITypeMethodByName");
    pOCITypeMethodEncap			    = dlsym( RTLD_NEXT, "OCITypeMethodEncap");
    pOCITypeMethodFlags			    = dlsym( RTLD_NEXT, "OCITypeMethodFlags");
    pOCITypeMethodMap			    = dlsym( RTLD_NEXT, "OCITypeMethodMap");
    pOCITypeMethodName			    = dlsym( RTLD_NEXT, "OCITypeMethodName");
    pOCITypeMethodNext			    = dlsym( RTLD_NEXT, "OCITypeMethodNext");
    pOCITypeMethodOrder			    = dlsym( RTLD_NEXT, "OCITypeMethodOrder");
    pOCITypeMethodOverload		    = dlsym( RTLD_NEXT, "OCITypeMethodOverload");
    pOCITypeMethodParams		    = dlsym( RTLD_NEXT, "OCITypeMethodParams");
    pOCITypeMethods			    = dlsym( RTLD_NEXT, "OCITypeMethods");
    pOCITypeName			    = dlsym( RTLD_NEXT, "OCITypeName");
    pOCITypeParamByName			    = dlsym( RTLD_NEXT, "OCITypeParamByName");
    pOCITypeParamByPos			    = dlsym( RTLD_NEXT, "OCITypeParamByPos");
    pOCITypeParamPos			    = dlsym( RTLD_NEXT, "OCITypeParamPos");
    pOCITypeResult			    = dlsym( RTLD_NEXT, "OCITypeResult");
    pOCITypeSchema			    = dlsym( RTLD_NEXT, "OCITypeSchema");
    pOCITypeTypeCode			    = dlsym( RTLD_NEXT, "OCITypeTypeCode");
    pOCITypeVTInit			    = dlsym( RTLD_NEXT, "OCITypeVTInit");
    pOCITypeVTInsert			    = dlsym( RTLD_NEXT, "OCITypeVTInsert");
    pOCITypeVTSelect			    = dlsym( RTLD_NEXT, "OCITypeVTSelect");
    pOCITypeVersion			    = dlsym( RTLD_NEXT, "OCITypeVersion");
    pOCIUnicodeToCharSet		    = dlsym( RTLD_NEXT, "OCIUnicodeToCharSet");
    pOCIUserCallbackGet			    = dlsym( RTLD_NEXT, "OCIUserCallbackGet");
    pOCIUserCallbackRegister		    = dlsym( RTLD_NEXT, "OCIUserCallbackRegister");
    pOCIWideCharDisplayLength		    = dlsym( RTLD_NEXT, "OCIWideCharDisplayLength");
    pOCIWideCharInSizeToMultiByte	    = dlsym( RTLD_NEXT, "OCIWideCharInSizeToMultiByte");
    pOCIWideCharIsAlnum			    = dlsym( RTLD_NEXT, "OCIWideCharIsAlnum");
    pOCIWideCharIsAlpha			    = dlsym( RTLD_NEXT, "OCIWideCharIsAlpha");
    pOCIWideCharIsCntrl			    = dlsym( RTLD_NEXT, "OCIWideCharIsCntrl");
    pOCIWideCharIsDigit			    = dlsym( RTLD_NEXT, "OCIWideCharIsDigit");
    pOCIWideCharIsGraph			    = dlsym( RTLD_NEXT, "OCIWideCharIsGraph");
    pOCIWideCharIsLower			    = dlsym( RTLD_NEXT, "OCIWideCharIsLower");
    pOCIWideCharIsPrint			    = dlsym( RTLD_NEXT, "OCIWideCharIsPrint");
    pOCIWideCharIsPunct			    = dlsym( RTLD_NEXT, "OCIWideCharIsPunct");
    pOCIWideCharIsSingleByte		    = dlsym( RTLD_NEXT, "OCIWideCharIsSingleByte");
    pOCIWideCharIsSpace			    = dlsym( RTLD_NEXT, "OCIWideCharIsSpace");
    pOCIWideCharIsUpper			    = dlsym( RTLD_NEXT, "OCIWideCharIsUpper");
    pOCIWideCharIsXdigit		    = dlsym( RTLD_NEXT, "OCIWideCharIsXdigit");
    pOCIWideCharMultiByteLength		    = dlsym( RTLD_NEXT, "OCIWideCharMultiByteLength");
    pOCIWideCharStrCaseConversion	    = dlsym( RTLD_NEXT, "OCIWideCharStrCaseConversion");
    pOCIWideCharStrcat			    = dlsym( RTLD_NEXT, "OCIWideCharStrcat");
    pOCIWideCharStrchr			    = dlsym( RTLD_NEXT, "OCIWideCharStrchr");
    pOCIWideCharStrcmp			    = dlsym( RTLD_NEXT, "OCIWideCharStrcmp");
    pOCIWideCharStrcpy			    = dlsym( RTLD_NEXT, "OCIWideCharStrcpy");
    pOCIWideCharStrlen			    = dlsym( RTLD_NEXT, "OCIWideCharStrlen");
    pOCIWideCharStrncat			    = dlsym( RTLD_NEXT, "OCIWideCharStrncat");
    pOCIWideCharStrncmp			    = dlsym( RTLD_NEXT, "OCIWideCharStrncmp");
    pOCIWideCharStrncpy			    = dlsym( RTLD_NEXT, "OCIWideCharStrncpy");
    pOCIWideCharStrrchr			    = dlsym( RTLD_NEXT, "OCIWideCharStrrchr");
    pOCIWideCharToLower			    = dlsym( RTLD_NEXT, "OCIWideCharToLower");
    pOCIWideCharToMultiByte		    = dlsym( RTLD_NEXT, "OCIWideCharToMultiByte");
    pOCIWideCharToUpper			    = dlsym( RTLD_NEXT, "OCIWideCharToUpper");
    return 0;
}

/* Decode handle type */
static const char *handle_type(int htype)
{
    if (htype <= 0 
	|| (htype > OCI_HTYPE_LAST && htype < OCI_DTYPE_FIRST) 
	|| htype > OCI_DTYPE_LAST)
	return "Invalid handle";
    if (htype >= 50)
	return OCIDescrTypes[htype-50];
    else
	return OCIHandleTypes[htype];
}

/* Decode return code */
/* Borrowed from octopus */
static const char *retcode(sword rc) {
   switch (rc) {
   case OCI_SUCCESS:
       return "SUCCESS";
   case OCI_SUCCESS_WITH_INFO:
       return "SUCCESS WITH INFO";
   case OCI_NO_DATA:
       return "NO DATA";
   case OCI_ERROR:
       return "ERROR";
   case OCI_INVALID_HANDLE:
       return "INVALID HANDLE";
   case OCI_NEED_DATA:
       return "NEED DATA";
   case OCI_STILL_EXECUTING:
       return "STILL EXECUTING";
   case OCI_CONTINUE:
       return "CONTINUE";
   default:
       return "UNKNOWN";
   }
}

/* Decode duration */
static const char *duration(sword d) {
    switch(d) {
    case OCI_DURATION_CALL: return "OCI_DURATION_CALL";
    case OCI_DURATION_CALLOUT: return "OCI_DURATION_CALLOUT";
    case OCI_DURATION_DEFAULT: return "OCI_DURATION_DEFAULT";
    case OCI_DURATION_NEXT: return "OCI_DURATION_NEXT";
    case OCI_DURATION_NULL: return "OCI_DURATION_NULL";
    case OCI_DURATION_PROCESS: return "OCI_DURATION_PROCESS";
    case OCI_DURATION_SESSION: return "OCI_DURATION_SESSION";
    case OCI_DURATION_STATEMENT: return "OCI_DURATION_STATEMENT";
    case OCI_DURATION_TRANS: return "OCI_DURATION_TRANS";
    default: return "Unknown duration";
    }
}

/* Decode typecode */
static const char *typecode(sword tc) {
    switch(tc) {
    case OCI_TYPECODE_REF: return "OCI_TYPECODE_REF";
    case OCI_TYPECODE_DATE: return "OCI_TYPECODE_DATE";
    case OCI_TYPECODE_SIGNED8: return "OCI_TYPECODE_SIGNED8";
    case OCI_TYPECODE_SIGNED16: return "OCI_TYPECODE_SIGNED16";
    case OCI_TYPECODE_SIGNED32: return "OCI_TYPECODE_SIGNED32";
    case OCI_TYPECODE_REAL: return "OCI_TYPECODE_REAL";
    case OCI_TYPECODE_DOUBLE: return "OCI_TYPECODE_DOUBLE";
    case OCI_TYPECODE_FLOAT: return "OCI_TYPECODE_FLOAT";
    case OCI_TYPECODE_NUMBER: return "OCI_TYPECODE_NUMBER";
    case OCI_TYPECODE_DECIMAL: return "OCI_TYPECODE_DECIMAL";
    case OCI_TYPECODE_UNSIGNED8: return "OCI_TYPECODE_UNSIGNED8";
    case OCI_TYPECODE_UNSIGNED16: return "OCI_TYPECODE_UNSIGNED16";
    case OCI_TYPECODE_UNSIGNED32: return "OCI_TYPECODE_UNSIGNED32";
    case OCI_TYPECODE_OCTET: return "OCI_TYPECODE_OCTET";
    case OCI_TYPECODE_SMALLINT: return "OCI_TYPECODE_SMALLINT";
    case OCI_TYPECODE_INTEGER: return "OCI_TYPECODE_INTEGER";
    case OCI_TYPECODE_RAW: return "OCI_TYPECODE_RAW";
    case OCI_TYPECODE_PTR: return "OCI_TYPECODE_PTR";
    case OCI_TYPECODE_VARCHAR2: return "OCI_TYPECODE_VARCHAR2";
    case OCI_TYPECODE_CHAR: return "OCI_TYPECODE_CHAR";
    case OCI_TYPECODE_VARCHAR: return "OCI_TYPECODE_VARCHAR";
    case OCI_TYPECODE_MLSLABEL: return "OCI_TYPECODE_MLSLABEL";
    case OCI_TYPECODE_VARRAY: return "OCI_TYPECODE_VARRAY";
    case OCI_TYPECODE_TABLE: return "OCI_TYPECODE_TABLE";
    case OCI_TYPECODE_OBJECT: return "OCI_TYPECODE_OBJECT";
    case OCI_TYPECODE_OPAQUE: return "OCI_TYPECODE_OPAQUE";
    case OCI_TYPECODE_NAMEDCOLLECTION: return "OCI_TYPECODE_NAMEDCOLLECTION";
    case OCI_TYPECODE_BLOB: return "OCI_TYPECODE_BLOB";
    case OCI_TYPECODE_BFILE: return "OCI_TYPECODE_BFILE";
    case OCI_TYPECODE_CLOB: return "OCI_TYPECODE_CLOB";
    case OCI_TYPECODE_CFILE: return "OCI_TYPECODE_CFILE";
    case OCI_TYPECODE_OTMFIRST: return "OCI_TYPECODE_OTM/SYSFIRST";

    case OCI_TYPECODE_OTMLAST: return "OCI_TYPECODE_OTM/SYSLAST";
	/* case OCI_TYPECODE_ITABLE: return "OCI_TYPECODE_ITABLE"; */
	/* case OCI_TYPECODE_RECORD: return "OCI_TYPECODE_RECORD"; */
	/* case OCI_TYPECODE_BOOLEAN: return "OCI_TYPECODE_BOOLEAN"; */
    default: return "Unknown Type Code";
    }
}

/* Decode attribute name for describe handle */
static const char *describe_attr(sword attr) {
    switch(attr) {
    case OCI_ATTR_DATA_SIZE: return "DATA_SIZE";
    case OCI_ATTR_DATA_TYPE: return "DATA_TYPE";
    case OCI_ATTR_DISP_SIZE: return "DISP_SIZE";
    case OCI_ATTR_NAME: return "NAME";
    case OCI_ATTR_PRECISION: return "PRECISION";
    case OCI_ATTR_SCALE: return "SCALE";
    case OCI_ATTR_IS_NULL: return "IS_NULL";
    case OCI_ATTR_TYPE_NAME: return "TYPE_NAME";
    case OCI_ATTR_SCHEMA_NAME: return "SCHEMA_NAME";
    case OCI_ATTR_SUB_NAME: return "SUB_NAME";
    case OCI_ATTR_POSITION: return "POSITION";
    case OCI_ATTR_COMPLEXOBJECTCOMP_TYPE: return "COMPLEXOBJECTCOMP_TYPE";
    case OCI_ATTR_COMPLEXOBJECTCOMP_TYPE_LEVEL: return "COMPLEXOBJECTCOMP_TYPE_LEVEL";
    case OCI_ATTR_COMPLEXOBJECT_LEVEL: return "COMPLEXOBJECT_LEVEL";
    case OCI_ATTR_COMPLEXOBJECT_COLL_OUTOFLINE: return "COMPLEXOBJECT_COLL_OUTOFLINE";
    case OCI_ATTR_DISP_NAME: return "DISP_NAME";
    case OCI_ATTR_OVERLOAD: return "OVERLOAD";
    case OCI_ATTR_LEVEL: return "LEVEL";
    case OCI_ATTR_HAS_DEFAULT: return "HAS_DEFAULT";
    case OCI_ATTR_IOMODE: return "IOMODE";
    case OCI_ATTR_RADIX: return "RADIX";
    case OCI_ATTR_NUM_ARGS: return "NUM_ARGS";
    case OCI_ATTR_TYPECODE: return "TYPECODE";
    case OCI_ATTR_COLLECTION_TYPECODE: return "COLLECTION_TYPECODE";
    case OCI_ATTR_VERSION: return "VERSION";
    case OCI_ATTR_IS_INCOMPLETE_TYPE: return "IS_INCOMPLETE_TYPE";
    case OCI_ATTR_IS_SYSTEM_TYPE: return "IS_SYSTEM_TYPE";
    case OCI_ATTR_IS_PREDEFINED_TYPE: return "IS_PREDEFINED_TYPE";
    case OCI_ATTR_IS_TRANSIENT_TYPE: return "IS_TRANSIENT_TYPE";
    case OCI_ATTR_IS_SYSTEM_GENERATED_TYPE: return "IS_SYSTEM_GENERATED_TYPE";
    case OCI_ATTR_HAS_NESTED_TABLE: return "HAS_NESTED_TABLE";
    case OCI_ATTR_HAS_LOB: return "HAS_LOB";
    case OCI_ATTR_HAS_FILE: return "HAS_FILE";
    case OCI_ATTR_COLLECTION_ELEMENT: return "COLLECTION_ELEMENT";
    case OCI_ATTR_NUM_TYPE_ATTRS: return "NUM_TYPE_ATTRS";
    case OCI_ATTR_LIST_TYPE_ATTRS: return "LIST_TYPE_ATTRS";
    case OCI_ATTR_NUM_TYPE_METHODS: return "NUM_TYPE_METHODS";
    case OCI_ATTR_LIST_TYPE_METHODS: return "LIST_TYPE_METHODS";
    case OCI_ATTR_MAP_METHOD: return "MAP_METHOD";
    case OCI_ATTR_ORDER_METHOD: return "ORDER_METHOD";
    case OCI_ATTR_NUM_ELEMS: return "NUM_ELEMS";
    case OCI_ATTR_ENCAPSULATION: return "ENCAPSULATION";
    case OCI_ATTR_IS_SELFISH: return "IS_SELFISH";
    case OCI_ATTR_IS_VIRTUAL: return "IS_VIRTUAL";
    case OCI_ATTR_IS_INLINE: return "IS_INLINE";
    case OCI_ATTR_IS_CONSTANT: return "IS_CONSTANT";
    case OCI_ATTR_HAS_RESULT: return "HAS_RESULT";
    case OCI_ATTR_IS_CONSTRUCTOR: return "IS_CONSTRUCTOR";
    case OCI_ATTR_IS_DESTRUCTOR: return "IS_DESTRUCTOR";
    case OCI_ATTR_IS_OPERATOR: return "IS_OPERATOR";
    case OCI_ATTR_IS_MAP: return "IS_MAP";
    case OCI_ATTR_IS_ORDER: return "IS_ORDER";
    case OCI_ATTR_IS_RNDS: return "IS_RNDS";
    case OCI_ATTR_IS_RNPS: return "IS_RNPS";
    case OCI_ATTR_IS_WNDS: return "IS_WNDS";
    case OCI_ATTR_IS_WNPS: return "IS_WNPS";
    case OCI_ATTR_DESC_PUBLIC: return "DESC_PUBLIC";
    case OCI_ATTR_CACHE_CLIENT_CONTEXT: return "CACHE_CLIENT_CONTEXT";
    case OCI_ATTR_UCI_CONSTRUCT: return "UCI_CONSTRUCT";
    case OCI_ATTR_UCI_DESTRUCT: return "UCI_DESTRUCT";
    case OCI_ATTR_UCI_COPY: return "UCI_COPY";
    case OCI_ATTR_UCI_PICKLE: return "UCI_PICKLE";
    case OCI_ATTR_UCI_UNPICKLE: return "UCI_UNPICKLE";
    case OCI_ATTR_UCI_REFRESH: return "UCI_REFRESH";
    case OCI_ATTR_IS_SUBTYPE: return "IS_SUBTYPE";
    case OCI_ATTR_SUPERTYPE_SCHEMA_NAME: return "SUPERTYPE_SCHEMA_NAME";
    case OCI_ATTR_SUPERTYPE_NAME: return "SUPERTYPE_NAME";
    case OCI_ATTR_LIST_OBJECTS: return "LIST_OBJECTS";
    case OCI_ATTR_NCHARSET_ID: return "NCHARSET_ID";
    case OCI_ATTR_LIST_SCHEMAS: return "LIST_SCHEMAS";
    case OCI_ATTR_MAX_PROC_LEN: return "MAX_PROC_LEN";
    case OCI_ATTR_MAX_COLUMN_LEN: return "MAX_COLUMN_LEN";
    case OCI_ATTR_CURSOR_COMMIT_BEHAVIOR: return "CURSOR_COMMIT_BEHAVIOR";
    case OCI_ATTR_MAX_CATALOG_NAMELEN: return "MAX_CATALOG_NAMELEN";
    case OCI_ATTR_CATALOG_LOCATION: return "CATALOG_LOCATION";
    case OCI_ATTR_SAVEPOINT_SUPPORT: return "SAVEPOINT_SUPPORT";
    case OCI_ATTR_NOWAIT_SUPPORT: return "NOWAIT_SUPPORT";
    case OCI_ATTR_AUTOCOMMIT_DDL: return "AUTOCOMMIT_DDL";
    case OCI_ATTR_LOCKING_MODE: return "LOCKING_MODE";
    default: return 0;
    }
}

void attr_decode_get(
    int attrtype,
    void *attrp,
    unsigned int *atrszp)
{
    switch (attrtype) {
    case OCI_ATTR_CACHE_MAX_SIZE:
    case OCI_ATTR_CACHE_OPT_SIZE:
    case OCI_ATTR_HEAPALLOC:
    case OCI_ATTR_SHARED_HEAPALLOC:
    case OCI_ATTR_DML_ROW_OFFSET:
    case OCI_ATTR_NUM_DML_ERRORS:
    case OCI_ATTR_ROW_COUNT:
    case OCI_ATTR_PARAM_COUNT:
    case OCI_ATTR_ROWS_RETURNED:
    case OCI_ATTR_PARAM:
    case OCI_ATTR_OBJ_ID:
	printf("\t\t\t    %s = %d\n", OCIAttrs[attrtype], *(ub4 *)attrp);
	break;

    case OCI_ATTR_NUM_ATTRS:
    case OCI_ATTR_NUM_PARAMS:
    case OCI_ATTR_NUM_COLS:
    case OCI_ATTR_OVERLOAD_ID:
	printf("\t\t\t    %s = %d\n", OCIAttrs[attrtype], *(ub2 *)attrp);
	break;

    case OCI_ATTR_NONBLOCKING_MODE:
    case OCI_ATTR_CHARSET_FORM:
    case OCI_ATTR_SUBSCR_PAYLOAD:
    case OCI_ATTR_DIRPATH_NOLOG:
    case OCI_ATTR_DIRPATH_PARALLEL:
	printf("\t\t\t    %s = %d\n", OCIAttrs[attrtype], *(ub1 *)attrp);
	break;

    case OCI_ATTR_FNCODE:
	/* NB the type here is a guess based on the fact that OCI_ATTR_SQLFNCODE
	   is a pointer to 2 byte int */
	printf("\t\t\t%s = %s\n", OCIAttrs[attrtype], OCIFNCode[*(unsigned short *)attrp]);
	break;
    case OCI_ATTR_OBJECT:
	printf("\t\t\t    %s = %s\n", OCIAttrs[attrtype], (*(boolean *)attrp) ? "true" : "false");
	break;
    case OCI_ATTR_ENV:
    case OCI_ATTR_SERVER:
    case OCI_ATTR_SESSION:
    case OCI_ATTR_TRANS:
    case OCI_ATTR_TDO:
	printf("\t\t\t    %s = %p\n", OCIAttrs[attrtype], *(void **)attrp);
	break;
    case OCI_ATTR_SQLFNCODE:
      {
	short fncode = *(unsigned short *)attrp;
	if (fncode > 0 && fncode < 100)
	    printf("\t\t\t    %s = %s\n", OCIAttrs[attrtype], SQLFCode1[fncode]);
	else if (fncode >= 157 && fncode < 184)
	    printf("\t\t\t    %s = %s\n", OCIAttrs[attrtype], SQLFCode2[(fncode) - 157]);
	else
	    printf("\t\t\t    %s = Invalid(%d)\n", OCIAttrs[attrtype], fncode);
	break;
      }
    case OCI_ATTR_PREFETCH_ROWS:
    case OCI_ATTR_PREFETCH_MEMORY:
    case OCI_ATTR_NESTED_PREFETCH_ROWS:   /* I'm guessing that these are as */
    case OCI_ATTR_NESTED_PREFETCH_MEMORY: /* the two cases above */
    case OCI_ATTR_CHAR_COUNT:
    case OCI_ATTR_LOBEMPTY:
	printf("\t\t\t    %s = Write Only\n", OCIAttrs[attrtype]);
	break;
    case OCI_ATTR_PDSCL:
	break;
    case OCI_ATTR_PDPRC:
	break;
    case OCI_ATTR_ROWID:
	break;
    case OCI_ATTR_CHARSET:
	break;
    case OCI_ATTR_NCHAR:
	break;
    case OCI_ATTR_USERNAME:
    case OCI_ATTR_PASSWORD:
    case OCI_ATTR_LINK:
	/* Text/string attributes */
	printf("\t\t\t    %s = %s\n", OCIAttrs[attrtype], *(char **)attrp);
	break;
    case OCI_ATTR_STMT_TYPE:
	break;
    case OCI_ATTR_INTERNAL_NAME:
	break;
    case OCI_ATTR_EXTERNAL_NAME:
	break;
    case OCI_ATTR_XID:
	break;
    case OCI_ATTR_TRANS_LOCK:
	break;
    case OCI_ATTR_TRANS_NAME:
	break;
    case OCI_ATTR_CHARSET_ID:
	break;
    case OCI_ATTR_MAXDATA_SIZE:
	break;
    case OCI_ATTR_PINOPTION:
    {
	char *pinopt;
	switch (*(OCIPinOpt *)attrp) {
	case OCI_PIN_DEFAULT: pinopt = "OCI_PIN_DEFAULT"; break;
	case OCI_PIN_ANY:     pinopt = "OCI_PIN_ANY"; break;
	case OCI_PIN_RECENT:  pinopt = "OCI_PIN_RECENT"; break;
	case OCI_PIN_LATEST:  pinopt = "OCI_PIN_LATEST"; break;
	default:              pinopt = "<invalid>"; break;
	}
	printf("\t\t\t    %s = %s", OCIAttrs[attrtype], pinopt);

	break;
    }
    case OCI_ATTR_ALLOC_DURATION:
	break;
    case OCI_ATTR_PIN_DURATION:
	break;
    case OCI_ATTR_FDO:
	break;
    case OCI_ATTR_POSTPROCESSING_CALLBACK:
	break;
    case OCI_ATTR_POSTPROCESSING_CONTEXT:
	break;
    case OCI_ATTR_FOCBK:
	break;
    case OCI_ATTR_IN_V8_MODE:
	break;
    case OCI_ATTR_SESSLANG:
	break;
    case OCI_ATTR_VISIBILITY:
	break;
    case OCI_ATTR_RELATIVE_MSGID:
	break;
    case OCI_ATTR_SEQUENCE_DEVIATION:
	break;
    case OCI_ATTR_CONSUMER_NAME:
	break;
    case OCI_ATTR_DEQ_MODE:
	break;
    case OCI_ATTR_NAVIGATION:
	break;
    case OCI_ATTR_WAIT:
	break;
    case OCI_ATTR_DEQ_MSGID:
	break;
    case OCI_ATTR_PRIORITY:
	break;
    case OCI_ATTR_DELAY:
	break;
    case OCI_ATTR_EXPIRATION:
	break;
    case OCI_ATTR_CORRELATION:
	break;
    case OCI_ATTR_ATTEMPTS:
	break;
    case OCI_ATTR_RECIPIENT_LIST:
	break;
    case OCI_ATTR_EXCEPTION_QUEUE:
	break;
    case OCI_ATTR_ENQ_TIME:
	break;
    case OCI_ATTR_MSG_STATE:
	break;
    case OCI_ATTR_AGENT_NAME:
	break;
    case OCI_ATTR_AGENT_ADDRESS:
	break;
    case OCI_ATTR_AGENT_PROTOCOL:
	break;
    case OCI_ATTR_SENDER_ID:
	break;
    case OCI_ATTR_ORIGINAL_MSGID:
	break;
    case OCI_ATTR_QUEUE_NAME:
	break;
    case OCI_ATTR_NFY_MSGID:
	break;
    case OCI_ATTR_MSG_PROP:
	break;
    case OCI_ATTR_DATEFORMAT:
	break;
    case OCI_ATTR_BUF_ADDR:
	break;
    case OCI_ATTR_BUF_SIZE:
	break;
    case OCI_ATTR_DIRPATH_MODE:
	break;
    case OCI_ATTR_NUM_ROWS:
	break;
    case OCI_ATTR_COL_COUNT:
	break;
    case OCI_ATTR_STREAM_OFFSET:
	break;
    case OCI_ATTR_SERVER_GROUP:
	break;
    case OCI_ATTR_MIGSESSION:
	break;
    case OCI_ATTR_NOCACHE:
	break;
    case OCI_ATTR_MEMPOOL_SIZE:
	break;
    case OCI_ATTR_MEMPOOL_INSTNAME:
	break;
    case OCI_ATTR_MEMPOOL_APPNAME:
	break;
    case OCI_ATTR_MEMPOOL_HOMENAME:
	break;
    case OCI_ATTR_MEMPOOL_MODEL:
	break;
    case OCI_ATTR_MODES:
	break;
    case OCI_ATTR_SUBSCR_NAME:
	break;
    case OCI_ATTR_SUBSCR_CALLBACK:
	break;
    case OCI_ATTR_SUBSCR_CTX:
	break;
    case OCI_ATTR_SUBSCR_NAMESPACE:
	break;
    case OCI_ATTR_PROXY_CREDENTIALS:
	break;
    case OCI_ATTR_INITIAL_CLIENT_ROLES:
	break;
    case OCI_ATTR_UNK:
	break;
    case OCI_ATTR_LIST_COLUMNS:
	break;
    case OCI_ATTR_RDBA:
	break;
    case OCI_ATTR_CLUSTERED:
	break;
    case OCI_ATTR_PARTITIONED:
	break;
    case OCI_ATTR_INDEX_ONLY:
	break;
    case OCI_ATTR_LIST_ARGUMENTS:
	break;
    case OCI_ATTR_LIST_SUBPROGRAMS:
	break;
    case OCI_ATTR_REF_TDO:
	break;
    case OCI_ATTR_MIN:
	break;
    case OCI_ATTR_MAX:
	break;
    case OCI_ATTR_INCR:
	break;
    case OCI_ATTR_CACHE:
	break;
    case OCI_ATTR_ORDER:
	break;
    case OCI_ATTR_HW_MARK:
	break;
    case OCI_ATTR_TYPE_SCHEMA:
	break;
    case OCI_ATTR_TIMESTAMP:
	break;
    case OCI_ATTR_OBJID:
	break;
    case OCI_ATTR_PTYPE:
	break;
    case OCI_ATTR_TABLESPACE:
	break;
    case OCI_ATTR_LTYPE:
	break;
    case OCI_ATTR_PARSE_ERROR_OFFSET:
	break;
    case OCI_ATTR_IS_TEMPORARY:
	break;
    case OCI_ATTR_IS_TYPED:
	break;
    case OCI_ATTR_DURATION:
	break;
    case OCI_ATTR_IS_INVOKER_RIGHTS:
	break;
    case OCI_ATTR_OBJ_NAME:
	break;
    case OCI_ATTR_OBJ_SCHEMA:
	break;
    case OCI_ATTR_SQLCODE:
	/* Not documented */
	break;
    }
}

void attr_decode_set(
    int attrtype,
    void *attrp,
    int  atrsz)
{
    switch (attrtype) {
    case OCI_ATTR_SERVER:
    case OCI_ATTR_SESSION:
    case OCI_ATTR_TRANS:
	/* Handles & pointers */
	printf("\t\t\t    %s = %p\n", OCIAttrs[attrtype], attrp);
	break;
    case OCI_ATTR_SQLFNCODE:
    case OCI_ATTR_OBJECT:
    case OCI_ATTR_HEAPALLOC:
    case OCI_ATTR_SHARED_HEAPALLOC:
    case OCI_ATTR_DML_ROW_OFFSET:
    case OCI_ATTR_NUM_DML_ERRORS:
    case OCI_ATTR_ROW_COUNT:
    case OCI_ATTR_PARAM_COUNT:
    case OCI_ATTR_ROWS_RETURNED:
    case OCI_ATTR_PARAM:
    case OCI_ATTR_FNCODE:
    case OCI_ATTR_ENV:
    case OCI_ATTR_TDO: /* I've guessed that this is read only */
	printf("\t\t\t    %s = Read Only\n", OCIAttrs[attrtype]);
	break;
    case OCI_ATTR_PREFETCH_ROWS:
    case OCI_ATTR_NESTED_PREFETCH_ROWS:
    case OCI_ATTR_PREFETCH_MEMORY:
    case OCI_ATTR_NESTED_PREFETCH_MEMORY:
    case OCI_ATTR_CACHE_MAX_SIZE:
    case OCI_ATTR_CACHE_OPT_SIZE:
    case OCI_ATTR_CHAR_COUNT:
    case OCI_ATTR_LOBEMPTY:
	printf("\t\t\t    %s = %d\n", OCIAttrs[attrtype], *(unsigned int *)attrp);
	break;
    case OCI_ATTR_NONBLOCKING_MODE:
    case OCI_ATTR_CHARSET_FORM:
    case OCI_ATTR_SUBSCR_PAYLOAD:
    case OCI_ATTR_DIRPATH_NOLOG:
    case OCI_ATTR_DIRPATH_PARALLEL:
	printf("\t\t\t    %s = %d\n", OCIAttrs[attrtype], *(unsigned char *)attrp);
	break;
    case OCI_ATTR_PDSCL:
	break;
    case OCI_ATTR_PDPRC:
	break;
    case OCI_ATTR_ROWID:
	break;
    case OCI_ATTR_CHARSET:
	break;
    case OCI_ATTR_NCHAR:
	break;
    case OCI_ATTR_USERNAME:
    case OCI_ATTR_PASSWORD:
	/* Text/string attributes */
	printf("\t\t\t    %s = %s\n", OCIAttrs[attrtype], (char *)attrp);
	break;
    case OCI_ATTR_STMT_TYPE:
	break;
    case OCI_ATTR_INTERNAL_NAME:
	break;
    case OCI_ATTR_EXTERNAL_NAME:
	break;
    case OCI_ATTR_XID:
	break;
    case OCI_ATTR_TRANS_LOCK:
	break;
    case OCI_ATTR_TRANS_NAME:
	break;
    case OCI_ATTR_CHARSET_ID:
	break;
    case OCI_ATTR_MAXDATA_SIZE:
	break;
    case OCI_ATTR_PINOPTION:
    {
	char *pinopt;
	switch ((OCIPinOpt)attrp) {
	case OCI_PIN_DEFAULT: pinopt = "OCI_PIN_DEFAULT"; break;
	case OCI_PIN_ANY:     pinopt = "OCI_PIN_ANY"; break;
	case OCI_PIN_RECENT:  pinopt = "OCI_PIN_RECENT"; break;
	case OCI_PIN_LATEST:  pinopt = "OCI_PIN_LATEST"; break;
	default:              pinopt = "<invalid>"; break;
	}
	printf("\t\t\t    %s = %s", OCIAttrs[attrtype], pinopt);

	break;
    }
    case OCI_ATTR_ALLOC_DURATION:
	break;
    case OCI_ATTR_PIN_DURATION:
	break;
    case OCI_ATTR_FDO:
	break;
    case OCI_ATTR_POSTPROCESSING_CALLBACK:
	break;
    case OCI_ATTR_POSTPROCESSING_CONTEXT:
	break;
    case OCI_ATTR_FOCBK:
	break;
    case OCI_ATTR_IN_V8_MODE:
	break;
    case OCI_ATTR_SESSLANG:
	break;
    case OCI_ATTR_VISIBILITY:
	break;
    case OCI_ATTR_RELATIVE_MSGID:
	break;
    case OCI_ATTR_SEQUENCE_DEVIATION:
	break;
    case OCI_ATTR_CONSUMER_NAME:
	break;
    case OCI_ATTR_DEQ_MODE:
	break;
    case OCI_ATTR_NAVIGATION:
	break;
    case OCI_ATTR_WAIT:
	break;
    case OCI_ATTR_DEQ_MSGID:
	break;
    case OCI_ATTR_PRIORITY:
	break;
    case OCI_ATTR_DELAY:
	break;
    case OCI_ATTR_EXPIRATION:
	break;
    case OCI_ATTR_CORRELATION:
	break;
    case OCI_ATTR_ATTEMPTS:
	break;
    case OCI_ATTR_RECIPIENT_LIST:
	break;
    case OCI_ATTR_EXCEPTION_QUEUE:
	break;
    case OCI_ATTR_ENQ_TIME:
	break;
    case OCI_ATTR_MSG_STATE:
	break;
    case OCI_ATTR_AGENT_NAME:
	break;
    case OCI_ATTR_AGENT_ADDRESS:
	break;
    case OCI_ATTR_AGENT_PROTOCOL:
	break;
    case OCI_ATTR_SENDER_ID:
	break;
    case OCI_ATTR_ORIGINAL_MSGID:
	break;
    case OCI_ATTR_QUEUE_NAME:
	break;
    case OCI_ATTR_NFY_MSGID:
	break;
    case OCI_ATTR_MSG_PROP:
	break;
    case OCI_ATTR_DATEFORMAT:
	break;
    case OCI_ATTR_BUF_ADDR:
	break;
    case OCI_ATTR_BUF_SIZE:
	break;
    case OCI_ATTR_DIRPATH_MODE:
	break;
    case OCI_ATTR_NUM_ROWS:
	break;
    case OCI_ATTR_COL_COUNT:
	break;
    case OCI_ATTR_STREAM_OFFSET:
	break;
    case OCI_ATTR_SERVER_GROUP:
	break;
    case OCI_ATTR_MIGSESSION:
	break;
    case OCI_ATTR_NOCACHE:
	break;
    case OCI_ATTR_MEMPOOL_SIZE:
	break;
    case OCI_ATTR_MEMPOOL_INSTNAME:
	break;
    case OCI_ATTR_MEMPOOL_APPNAME:
	break;
    case OCI_ATTR_MEMPOOL_HOMENAME:
	break;
    case OCI_ATTR_MEMPOOL_MODEL:
	break;
    case OCI_ATTR_MODES:
	break;
    case OCI_ATTR_SUBSCR_NAME:
	break;
    case OCI_ATTR_SUBSCR_CALLBACK:
	break;
    case OCI_ATTR_SUBSCR_CTX:
	break;
    case OCI_ATTR_SUBSCR_NAMESPACE:
	break;
    case OCI_ATTR_PROXY_CREDENTIALS:
	break;
    case OCI_ATTR_INITIAL_CLIENT_ROLES:
	break;
    case OCI_ATTR_UNK:
	break;
    case OCI_ATTR_NUM_COLS:
	break;
    case OCI_ATTR_LIST_COLUMNS:
	break;
    case OCI_ATTR_RDBA:
	break;
    case OCI_ATTR_CLUSTERED:
	break;
    case OCI_ATTR_PARTITIONED:
	break;
    case OCI_ATTR_INDEX_ONLY:
	break;
    case OCI_ATTR_LIST_ARGUMENTS:
	break;
    case OCI_ATTR_LIST_SUBPROGRAMS:
	break;
    case OCI_ATTR_REF_TDO:
	break;
    case OCI_ATTR_LINK:
	break;
    case OCI_ATTR_MIN:
	break;
    case OCI_ATTR_MAX:
	break;
    case OCI_ATTR_INCR:
	break;
    case OCI_ATTR_CACHE:
	break;
    case OCI_ATTR_ORDER:
	break;
    case OCI_ATTR_HW_MARK:
	break;
    case OCI_ATTR_TYPE_SCHEMA:
	break;
    case OCI_ATTR_TIMESTAMP:
	break;
    case OCI_ATTR_NUM_ATTRS:
	break;
    case OCI_ATTR_NUM_PARAMS:
	break;
    case OCI_ATTR_OBJID:
	break;
    case OCI_ATTR_PTYPE:
	break;
    case OCI_ATTR_OVERLOAD_ID:
	break;
    case OCI_ATTR_TABLESPACE:
	break;
    case OCI_ATTR_LTYPE:
	break;
    case OCI_ATTR_PARSE_ERROR_OFFSET:
	break;
    case OCI_ATTR_IS_TEMPORARY:
	break;
    case OCI_ATTR_IS_TYPED:
	break;
    case OCI_ATTR_DURATION:
	break;
    case OCI_ATTR_IS_INVOKER_RIGHTS:
	break;
    case OCI_ATTR_OBJ_NAME:
	break;
    case OCI_ATTR_OBJ_SCHEMA:
	break;
    case OCI_ATTR_OBJ_ID:
	break;
    case OCI_ATTR_SQLCODE:
	/* Not documented */
	break;
    }
}

void attr_decode_get_describe(
    int attrtype,
    void *attrp,
    unsigned int *atrszp)
{
    switch(attrtype) {
    case OCI_ATTR_NAME: 
    case OCI_ATTR_VERSION: 
    case OCI_ATTR_TYPE_NAME: 
    case OCI_ATTR_SCHEMA_NAME: 
    case OCI_ATTR_SUB_NAME: 
    case OCI_ATTR_DISP_NAME: 
	printf("\t\t\t    %s = %s\n", describe_attr(attrtype), *(char **)attrp);
      break;

    case OCI_ATTR_DATA_SIZE: 
    case OCI_ATTR_DISP_SIZE: /* Guess at ub2 as type */
    case OCI_ATTR_POSITION: 
    case OCI_ATTR_LEVEL: 
	printf("\t\t\t    %s = %d\n", describe_attr(attrtype), *(ub2 *)attrp);
      break;
    case OCI_ATTR_DATA_TYPE: 
	printf("\t\t\t    %s = %s\n", describe_attr(attrtype), SQLTTypes[*(short *)attrp]);
      break;
    case OCI_ATTR_SCALE: 
	printf("\t\t\t    %s = %d\n", describe_attr(attrtype), *(sb1 *)attrp);
      break;
    case OCI_ATTR_AUTOCOMMIT_DDL: 
    case OCI_ATTR_CATALOG_LOCATION: 
    case OCI_ATTR_HAS_DEFAULT: 
    case OCI_ATTR_HAS_FILE: 
    case OCI_ATTR_HAS_LOB: 
    case OCI_ATTR_HAS_NESTED_TABLE: 
    case OCI_ATTR_HAS_RESULT: 
    case OCI_ATTR_IS_CONSTANT: 
    case OCI_ATTR_IS_CONSTRUCTOR: 
    case OCI_ATTR_IS_DESTRUCTOR: 
    case OCI_ATTR_IS_INCOMPLETE_TYPE: 
    case OCI_ATTR_IS_INLINE: 
    case OCI_ATTR_IS_MAP: 
    case OCI_ATTR_IS_NULL: 
    case OCI_ATTR_IS_OPERATOR: 
    case OCI_ATTR_IS_ORDER: 
    case OCI_ATTR_IS_PREDEFINED_TYPE: 
    case OCI_ATTR_IS_RNDS: 
    case OCI_ATTR_IS_RNPS: 
    case OCI_ATTR_IS_SELFISH: 
    case OCI_ATTR_IS_SYSTEM_GENERATED_TYPE: 
    case OCI_ATTR_IS_SYSTEM_TYPE: 
    case OCI_ATTR_IS_TRANSIENT_TYPE: 
    case OCI_ATTR_IS_VIRTUAL: 
    case OCI_ATTR_IS_WNDS: 
    case OCI_ATTR_IS_WNPS: 
    case OCI_ATTR_LOCKING_MODE: 
    case OCI_ATTR_MAX_CATALOG_NAMELEN: 
    case OCI_ATTR_NOWAIT_SUPPORT: 
    case OCI_ATTR_PRECISION: 
    case OCI_ATTR_RADIX: 
    case OCI_ATTR_SAVEPOINT_SUPPORT: 
	printf("\t\t\t    %s = %ud\n", describe_attr(attrtype), *(ub1  *)attrp);
      break;
    case OCI_ATTR_NUM_TYPE_ATTRS: 
    case OCI_ATTR_NUM_TYPE_METHODS: 
	printf("\t\t\t    %s = %ud\n", describe_attr(attrtype), *(ub4  *)attrp);
      break;

    case OCI_ATTR_TYPECODE: 
    case OCI_ATTR_COLLECTION_TYPECODE: 
	printf("\t\t\t    %s = %s\n", describe_attr(attrtype), typecode(*(OCITypeCode *)attrp));
      break;

    case OCI_ATTR_LIST_TYPE_ATTRS: 
    case OCI_ATTR_LIST_TYPE_METHODS: 
	printf("\t\t\t    %s = %p\n", describe_attr(attrtype), *(void **)attrp);
      break;

    case OCI_ATTR_COMPLEXOBJECTCOMP_TYPE: 
      break;
    case OCI_ATTR_COMPLEXOBJECTCOMP_TYPE_LEVEL: 
      break;
    case OCI_ATTR_COMPLEXOBJECT_LEVEL: 
      break;
    case OCI_ATTR_COMPLEXOBJECT_COLL_OUTOFLINE: 
      break;
    case OCI_ATTR_OVERLOAD: 
      break;
    case OCI_ATTR_IOMODE: 
      break;
    case OCI_ATTR_NUM_ARGS: 
      break;
    case OCI_ATTR_COLLECTION_ELEMENT: 
      break;
    case OCI_ATTR_MAP_METHOD: 
      break;
    case OCI_ATTR_ORDER_METHOD: 
      break;
    case OCI_ATTR_NUM_ELEMS: 
      break;
    case OCI_ATTR_ENCAPSULATION: 
      break;
    case OCI_ATTR_DESC_PUBLIC: 
      break;
    case OCI_ATTR_CACHE_CLIENT_CONTEXT: 
      break;
    case OCI_ATTR_UCI_CONSTRUCT: 
      break;
    case OCI_ATTR_UCI_DESTRUCT: 
      break;
    case OCI_ATTR_UCI_COPY: 
      break;
    case OCI_ATTR_UCI_PICKLE: 
      break;
    case OCI_ATTR_UCI_UNPICKLE: 
      break;
    case OCI_ATTR_UCI_REFRESH: 
      break;
    case OCI_ATTR_IS_SUBTYPE: 
      break;
    case OCI_ATTR_SUPERTYPE_SCHEMA_NAME: 
      break;
    case OCI_ATTR_SUPERTYPE_NAME: 
      break;
    case OCI_ATTR_LIST_OBJECTS: 
      break;
    case OCI_ATTR_NCHARSET_ID: 
      break;
    case OCI_ATTR_LIST_SCHEMAS: 
      break;
    case OCI_ATTR_MAX_PROC_LEN: 
      break;
    case OCI_ATTR_MAX_COLUMN_LEN: 
      break;
    case OCI_ATTR_CURSOR_COMMIT_BEHAVIOR: 
      break;
    }
}

void attr_decode_set_describe(
    int		  attrtype,
    void	  *attrp,
    unsigned int  atrszp)
{
    /* I think that all the describe handle attributes are read only */
}

sword
OCIInitialize(
    ub4		 mode,
    dvoid	 *ctxp,
    dvoid	 *(*malocfp)(dvoid *ctxp, size_t size),
    dvoid	 *(*ralocfp)(dvoid *ctxp, dvoid *memptr, size_t newsize),
    void	 (*mfreefp)(dvoid *ctxp, dvoid *memptr) )
{
    int		 retval;

    printf("OCIInitialize:\t\tmode = (");
    if (mode == 0)
	printf("OCI_DEFAULT)\n");
    else
	printf("%s%s%s%s%s%s)\n",
	       (mode & 0x01) ? "OCI_THREADED " : "",
	       (mode & 0x02) ? "OCI_OBJECT " : "",
	       (mode & 0x04) ? "OCI_EVENTS " : "",
	       (mode & 0x10) ? "OCI_SHARED " : "",
	       (mode & 0x40) ? "OCI_NO_UCB " : "",
	       (mode & 0x80) ? "OCI_NO_MUTEX" : "");
    printf("\t\t\tctxp = %p\n", ctxp);
    printf("\t\t\tmalocfp = %p\n", malocfp);
    printf("\t\t\tralocfp = %p\n", ralocfp);
    printf("\t\t\tmfreefp = %p\n", mfreefp);
    retval =  pOCIInitialize(mode, ctxp, malocfp, ralocfp, mfreefp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIEnvInit(
    OCIEnv	 **envp,
    ub4		 mode,
    size_t	 xtramemsz,
    dvoid	 **usrmempp )
{
    sword	 retval;

    printf("OCIEnvInit:\t\tenvp = %p\n", envp);
    printf("\t\t\tmode = (");
    if (mode == 0)
	printf("OCI_DEFAULT)\n");
    else
	printf("%s %s)\n",
	       (mode & 0x01) ? "OCI_ENV_NO_UCB" : "",
	       (mode & 0x08) ? "OCI_ENV_NO_MUTEX" : "");
    printf("\t\t\txtramemsz = %d\n", xtramemsz);
    printf("\t\t\tusrmempp = %p\n", usrmempp);
    retval = pOCIEnvInit(envp, mode, xtramemsz, usrmempp);
    printf("\t\t\tallocated env = %p\n", *envp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword 
OCISessionBegin(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp,
    OCISession	 *usrhp,
    ub4		 credt,
    ub4		 mode)
{
    sword	 retval;

    printf("OCISessionBegin:\tsvchp = %p\n", svchp);
    printf("\t\t\terrhp = %p\n", errhp);
    printf("\t\t\tusrhp = %p\n", usrhp);
    printf("\t\t\tcred = OCI_CRED_%s\n", OCICreds[credt]);
    printf("\t\t\tmode=(");
    if (mode == 0)
	printf("OCI_DEFAULT)\n");
    else
	printf("%s%s%s%s)\n",
	       (mode & 0x01) ? "OCI_MIGRATE " : "",
	       (mode & 0x02) ? "OCI_SYSDBA " : "",
	       (mode & 0x04) ? "OCI_SYSOPER " : "",
	       (mode & 0x08) ? "OCI_PRELIM_AUTH" : "");

    retval = pOCISessionBegin(svchp, errhp, usrhp, credt, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCISessionEnd(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp,
    OCISession	 *usrhp,
    ub4		 mode)
{
    sword	 retval;
    
    printf("OCISessionEnd:\t\tsvchp = %p\n", svchp);
    printf("\t\t\terrhp = %p\n", errhp);
    printf("\t\t\tusrhp = %p\n", usrhp);
    retval = pOCISessionEnd(svchp, errhp, usrhp, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIServerAttach(
    OCIServer	   *srvhp,
    OCIError	   *errhp,
    CONST OraText  *dblink,
    sb4		   dblink_len,
    ub4		   mode)
{
    sword	   retval;

    printf("OCIServerAttach:\tsrvhp = %p\n", srvhp);
    printf("\t\t\terrhp = %p\n", errhp);
    retval = pOCIServerAttach (srvhp, errhp, dblink,  dblink_len, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIServerDetach(
    OCIServer	 *srvhp, 
    OCIError	 *errhp, 
    ub4		 mode)
{
    sword	 retval;

    printf("OCIServerDetach:\tsrvhp = %p\n", srvhp);
    printf("\t\t\terrhp = %p\n", errhp);
    retval = pOCIServerDetach(srvhp, errhp, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIAttrGet(
    CONST dvoid	 *trgthndlp,
    ub4		 trghndltyp,
    dvoid	 *attributep,
    ub4		 *sizep,
    ub4		 attrtype,
    OCIError	 *errhp)
{
    sword	 retval;
    const char	 *htype	  = handle_type(trghndltyp);

    printf("OCIAttrGet:\t\ttrgthndlp = %s@%p\n", htype, trgthndlp);
    printf("\t\t\terrhp = %p\n", errhp);
    retval = pOCIAttrGet(trgthndlp, trghndltyp, attributep, sizep, attrtype, errhp);
    if (*htype == 'H') {
	printf("\t\t\tattrtype = OCI_ATTR_%s\n", OCIAttrs[attrtype]);
	attr_decode_get(attrtype, attributep, sizep);
    } else if (*htype == 'D') {
	const char *desc_attr = describe_attr(attrtype);

	if (desc_attr == 0)
	    desc_attr = OCIAttrs[attrtype];
	printf("\t\t\tattrtype = OCI_ATTR_%s\n", desc_attr);
	attr_decode_get_describe(attrtype, attributep, sizep);
    }
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIAttrSet(
    dvoid	 *trgthndlp,
    ub4		 trghndltyp,
    dvoid	 *attributep,
    ub4		 size,
    ub4		 attrtype,
    OCIError	 *errhp)
{
    sword	 retval;
    const char	 *htype	  = handle_type(trghndltyp);

    printf("OCIAttrSet:\t\ttrgthndlp = %s@%p\n", htype, trgthndlp);
    printf("\t\t\terrhp = %p\n", errhp);
    if (*htype == 'H') {
	printf("\t\t\tattrtype = OCI_ATTR_%s\n", OCIAttrs[attrtype]);
	attr_decode_set(attrtype, attributep, size);
    } else if (*htype == 'D') {
	const char *desc_attr = describe_attr(attrtype);

	if (desc_attr == 0)
	    desc_attr = OCIAttrs[attrtype];

	printf("\t\t\tattrtype = OCI_ATTR_%s\n", desc_attr);
	attr_decode_set_describe(attrtype, attributep, size);
    }
    retval = pOCIAttrSet(trgthndlp, trghndltyp, attributep, size, attrtype, errhp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITerminate(
    ub4		 mode)
{
    sword	 retval;

    printf("OCITerminate:\t\tenvp = %d\n", mode);
    retval = pOCITerminate(mode);
    return retval;
}

sword
OCIEnvCreate(
    OCIEnv	 **envp, 
    ub4		 mode, 
    dvoid	 *ctxp,
    dvoid	 *(*malocfp)(dvoid *ctxp, size_t size),
    dvoid	 *(*ralocfp)(dvoid *ctxp, dvoid *memptr, size_t newsize),
    void	 (*mfreefp)(dvoid *ctxp, dvoid *memptr),
    size_t	 xtramem_sz,
    dvoid	 **usrmempp)
{
    sword	 retval;

    printf("OCIEnvCreate:\t\tenvp = %p\n", envp);
    printf("\t\t\tctxp = %p\n", ctxp);
    printf("\t\t\tmalocfp = %p\n", malocfp);
    printf("\t\t\tralocfp = %p\n", ralocfp);
    printf("\t\t\tmfreefp = %p\n", mfreefp);
    retval = pOCIEnvCreate(envp, mode, ctxp, malocfp, ralocfp, mfreefp, xtramem_sz, usrmempp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIHandleAlloc(
    CONST dvoid	 *parenth,
    dvoid	 **hndlpp,
    ub4		 type,
    size_t	 xtramem_sz,
    dvoid	 **usrmempp)
{
    sword	 retval;

    printf("OCIHandleAlloc:\t\tparenth = %p\n", parenth);
    printf("\t\t\ttype = %s\n", handle_type(type));
    printf("\t\t\txtramem_sz = %d\n", xtramem_sz);
    printf("\t\t\tusrmempp = %p\n", usrmempp);
    retval = pOCIHandleAlloc(parenth, hndlpp, type, xtramem_sz, usrmempp);
    if (retval != OCI_ERROR)
	printf("\t\t\thandle allocated at %p\n", *hndlpp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIHandleFree(
    dvoid	 *hndlp,
    ub4		 type)
{
    sword	 retval;

    printf("OCIHandleFree:\t\thndlp = %p\n", hndlp);
    printf("\t\t\ttype = %s\n", handle_type(type));
    retval = pOCIHandleFree(hndlp, type);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCIDescriptorAlloc(
    CONST dvoid	 *parenth,
    dvoid	 **descpp,
    ub4		 type,
    size_t	 xtramem_sz,
    dvoid	 **usrmempp)
{
    sword	 retval;

    printf("OCIDescriptorAlloc:\n");
    retval = pOCIDescriptorAlloc(parenth, descpp, type, xtramem_sz, usrmempp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIDescriptorFree(
    dvoid	 *descp, 
    ub4		 type)
{
    sword	 retval;

    printf("OCIDescriptorFree:\n");
    retval = pOCIDescriptorFree(descp, type);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILogon (
    OCIEnv	   *envhp,
    OCIError	   *errhp,
    OCISvcCtx	   **svchp,
    CONST OraText  *username, 
    ub4		   uname_len,
    CONST OraText  *password, 
    ub4		   passwd_len,
    CONST OraText  *dbname, 
    ub4		   dbname_len)
{
    sword	   retval;

    printf("OCILogon:\n");
    retval = pOCILogon(envhp, errhp, svchp, username, uname_len, password, passwd_len, dbname, dbname_len);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILogoff(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp)
{
    sword	 retval;

    printf("OCILogoff:\n");
    retval = pOCILogoff(svchp, errhp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCIPasswordChange(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    CONST OraText  *user_name,
    ub4		   usernm_len,
    CONST OraText  *opasswd,
    ub4		   opasswd_len,
    CONST OraText  *npasswd,
    ub4		   npasswd_len,
    ub4		   mode)
{
    sword	   retval;

    printf("OCIPasswordChange:\n");
    retval = pOCIPasswordChange(svchp, errhp,
				user_name, usernm_len,
				opasswd, opasswd_len,
				npasswd, npasswd_len,
				mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIStmtPrepare(
    OCIStmt	   *stmtp,
    OCIError	   *errhp,
    CONST OraText  *stmt,
    ub4		   stmt_len,
    ub4		   language,
    ub4		   mode)
{
    sword	   retval;

    printf("OCIStmtPrepare:\t\tstmtp = %p\n", stmtp);
    printf("\t\t\tstmt = %s\n", stmt);
    retval = pOCIStmtPrepare(stmtp, errhp, stmt, stmt_len, language, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIBindByPos(
    OCIStmt	 *stmtp,
    OCIBind	 **bindp,
    OCIError	 *errhp,
    ub4		 position,
    dvoid	 *valuep,
    sb4		 value_sz,
    ub2		 dty,
    dvoid	 *indp,
    ub2		 *alenp,
    ub2		 *rcodep,
    ub4		 maxarr_len,
    ub4		 *curelep,
    ub4		 mode)
{
    sword	 retval;

    printf("OCIBindByPos:\tstmtp = %p\n", stmtp);
    printf("\t\t\tposition = %d\n", position);
    printf("\t\t\tdty = %s\n", SQLTTypes[dty]);
    retval = pOCIBindByPos(stmtp, bindp, errhp, position, valuep, value_sz,
			   dty, indp, alenp, rcodep, maxarr_len, curelep, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIBindByName(
    OCIStmt	   *stmtp,
    OCIBind	   **bindp,
    OCIError	   *errhp,
    CONST OraText  *placeholder,
    sb4		   placeh_len,
    dvoid	   *valuep,
    sb4		   value_sz,
    ub2		   dty,
    dvoid	   *indp,
    ub2		   *alenp,
    ub2		   *rcodep,
    ub4		   maxarr_len,
    ub4		   *curelep,
    ub4		   mode)
{
    sword	   retval;

    printf("OCIBindByName:\tstmtp = %p\n", stmtp);
    printf("\t\t\tname = %s\n", placeholder);
    printf("\t\t\tdty = %s\n", SQLTTypes[dty]);
    retval = pOCIBindByName(stmtp, bindp, errhp, placeholder, placeh_len,
			    valuep, value_sz, dty, indp, alenp, rcodep,
			    maxarr_len, curelep, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIBindObject(
    OCIBind	   *bindp,
    OCIError	   *errhp,
    CONST OCIType  *type,
    dvoid	   **pgvpp,
    ub4		   *pvszsp,
    dvoid	   **indpp,
    ub4		   *indszp)
{
    sword	   retval;

    printf("OCIBindObject:\n");
    retval = pOCIBindObject(bindp, errhp, type, pgvpp, pvszsp, indpp, indszp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIBindDynamic(
    OCIBind		*bindp,
    OCIError		*errhp,
    dvoid		*ictxp,
    OCICallbackInBind	icbfp,
    dvoid		*octxp,
    OCICallbackOutBind	ocbfp)
{
    sword		retval;
    
    printf("OCIBindDynamic:\n");
    retval = pOCIBindDynamic(bindp, errhp, ictxp, icbfp, octxp, ocbfp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIBindArrayOfStruct(
    OCIBind	 *bindp,
    OCIError	 *errhp,
    ub4		 pvskip, ub4 indskip,
    ub4		 alskip, ub4 rcskip)
{
    sword	 retval;

    printf("OCIBindArrayOfStruct:\n");
    retval = pOCIBindArrayOfStruct(bindp, errhp, pvskip, indskip, alskip, rcskip);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIStmtGetPieceInfo(
    OCIStmt	 *stmtp, 
    OCIError	 *errhp,
    dvoid	 **hndlpp, 
    ub4		 *typep,
    ub1		 *in_outp, 
    ub4		 *iterp, 
    ub4		 *idxp,
    ub1		 *piecep)
{
    sword	 retval;
    const char	 *chtype;

    printf("OCIStmtGetPieceInfo:\tstmtp = %p\n", stmtp);
    retval = pOCIStmtGetPieceInfo(stmtp, errhp, hndlpp, typep,
				  in_outp, iterp, idxp, piecep);

    chtype = handle_type(*typep);
    printf("\t\treturns hndlp = %s@%p\n", chtype, *hndlpp);
    printf("\t\treturns piece = %s\n", OCIPieceCodes[*piecep]);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIStmtSetPieceInfo(
    dvoid	 *hndlp, 
    ub4		 htype, 
    OCIError	 *errhp,
    CONST dvoid	 *bufp, 
    ub4		 *alenp, 
    ub1		 piece,
    CONST dvoid	 *indp, 
    ub2		 *rcodep)
{
    sword	 retval;

    const char	 *chtype = handle_type(htype);

    printf("OCIStmtSetPieceInfo:\thndlp = %s@%p\n", chtype, hndlp);
    printf("\t\t\tbufp = %p\n", bufp);
    printf("\t\t\tpiece = %s\n", OCIPieceCodes[piece]);
    printf("\t\t\talenp = %d@%p\n", *alenp, alenp);
    retval = pOCIStmtSetPieceInfo(hndlp, htype, errhp,
				  bufp, alenp, piece,
				  CONST indp, rcodep);
    printf("\t\treturns alen = %d\n", *alenp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIStmtExecute(
    OCISvcCtx	       *svchp, 
    OCIStmt	       *stmtp, 
    OCIError	       *errhp,
    ub4		       iters, 
    ub4		       rowoff, 
    CONST OCISnapshot  *snap_in,
    OCISnapshot	       *snap_out, 
    ub4		       mode)
{
    sword	       retval;

    printf("OCIStmtExecute:\t\tstmtp = %p\n", stmtp);
    printf("\t\t\titers = %d\n", iters);
    printf("\t\t\trowoff = %d\n", rowoff);
    printf("\t\t\tsnap_in = %p\n", snap_in);
    printf("\t\t\tsnap_out = %p\n", snap_out);
    printf("\t\t\tmode=(");
    if (mode == 0)
	printf("OCI_DEFAULT)\n");
    else
	printf("%s%s%s%s)\n",
	       (mode & 0x02) ? "OCI_EXACT_FETCH" : "",
	       (mode & 0x10) ? "OCI_DESCRIBE_ONLY" : "",
	       (mode & 0x20) ? "OCI_COMMIT_ON_SUCCESS" : "",
	       (mode & 0x80) ? "OCI_BATCH_ERRORS" : "");

    retval = pOCIStmtExecute(svchp, stmtp, errhp,
                         iters, rowoff, snap_in, snap_out, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIDefineByPos(
    OCIStmt	 *stmtp,
    OCIDefine	 **defnp, 
    OCIError	 *errhp,
    ub4		 position, 
    dvoid	 *valuep, 
    sb4		 value_sz, 
    ub2		 dty,
    dvoid	 *indp, 
    ub2		 *rlenp, 
    ub2		 *rcodep, 
    ub4		 mode)
{
    sword	 retval;

    printf("OCIDefineByPos:\tstmtp = %p\n", stmtp);
    printf("\t\t\tdefnp = %p@%p\n", *defnp, defnp);
    printf("\t\t\tposition = %d\n", position);
    printf("\t\t\tvaluep = %p\n", valuep);
    printf("\t\t\tvalue_sz = %d\n", value_sz);
    printf("\t\t\tdty = %s\n", SQLTTypes[dty]);
    printf("\t\t\tindp = %p\n", indp);
    printf("\t\t\trlenp = %p\n", rlenp);
    printf("\t\t\trcodep = %p\n", rcodep);
    if (mode == OCI_DYNAMIC_FETCH)
	printf("\t\t\tmode = DYNAMIC_FETCH\n");
    else
	printf("\t\t\tmode = %s\n", mode == 0 ? "DEFAULT" : "Invalid");
    retval = pOCIDefineByPos(stmtp, defnp, errhp, position, valuep, value_sz, dty,
			     indp, rlenp, rcodep, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIDefineObject(
    OCIDefine	   *defnp,
    OCIError	   *errhp,
    CONST OCIType  *type, 
    dvoid	   **pgvpp,
    ub4		   *pvszsp, 
    dvoid	   **indpp, 
    ub4		   *indszp)
{
    sword	   retval;

    printf("OCIDefineObject:\tdefnp = %p\n", defnp);
    printf("\t\t\ttype = %p\n", type);
    printf("\t\t\tpgvpp = %p\n", pgvpp);
    printf("\t\t\tpgvpp = %p\n", pgvpp);
    printf("\t\t\tpvszsp = %p\n", pvszsp);
    printf("\t\t\tindpp = %p\n", indpp);
    printf("\t\t\tindszp = %p\n", indszp);
    retval = pOCIDefineObject  (defnp, errhp, type, pgvpp,
				pvszsp, indpp, indszp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIDefineDynamic(
    OCIDefine	       *defnp,
    OCIError	       *errhp,
    dvoid	       *octxp,
    OCICallbackDefine  ocbfp)
{
    sword	       retval;

    printf("OCIDefineDynamic:\n");
    retval = pOCIDefineDynamic(defnp, errhp, octxp, ocbfp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIDefineArrayOfStruct(
    OCIDefine	 *defnp,
    OCIError	 *errhp,
    ub4		 pvskip,
    ub4		 indskip,
    ub4		 rlskip,
    ub4		 rcskip)
{
    sword	 retval;

    printf("OCIDefineArrayOfStruct:\n");
    retval = pOCIDefineArrayOfStruct(defnp, errhp, pvskip, indskip,
				     rlskip, rcskip);

    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIStmtFetch(
    OCIStmt	 *stmtp,
    OCIError	 *errhp,
    ub4		 nrows,
    ub2		 orientation,
    ub4		 mode)
{
    sword	 retval;

    printf("OCIStmtFetch:\t\tstmtp = %p\n", stmtp);
    printf("\t\t\tnrows = %d\n", nrows);
    printf("\t\t\torientation = %d(%s)\n", orientation,
	   orientation == OCI_FETCH_NEXT ? "OCI_FETCH_NEXT" : "Invalid");
    printf("\t\t\tmode=(");
    if (mode == 0)
	printf("OCI_DEFAULT)\n");
    else
	printf("Invalid(%d))\n", mode);

    retval = pOCIStmtFetch(stmtp, errhp, nrows, orientation, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIStmtGetBindInfo(
    OCIStmt	 *stmtp,
    OCIError	 *errhp,
    ub4		 size,
    ub4		 startloc,
    sb4		 *found, 
    OraText	 *bvnp[], 
    ub1		 bvnl[],
    OraText	 *invp[], 
    ub1		 inpl[], 
    ub1		 dupl[],
    OCIBind	 *hndl[])
{
    sword	 retval;

    printf("OCIStmtGetBindInfo:\tstmtp = %p\n", stmtp);
    retval = pOCIStmtGetBindInfo(stmtp, errhp, size, startloc, found, bvnp, bvnl, invp, inpl, dupl, hndl);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIDescribeAny(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp,
    dvoid	 *objptr,
    ub4		 objnm_len, 
    ub1		 objptr_typ, 
    ub1		 info_level,
    ub1		 objtyp, 
    OCIDescribe	 *dschp)
{
    sword	 retval;

    printf("OCIDescribeAny:\n");
    retval = pOCIDescribeAny(svchp, errhp, objptr, objnm_len, objptr_typ, info_level, objtyp, dschp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIParamGet(
    CONST dvoid	 *hndlp,
    ub4		 htype,
    OCIError	 *errhp,
    dvoid	 **parmdpp,
    ub4		 pos)
{
    sword	 retval;

    printf("OCIParamGet: hndlp = %s@%p\n", handle_type(htype), hndlp);
    printf("\t\t\tpos = %d\n", pos);
    retval = pOCIParamGet(hndlp, htype, errhp, parmdpp, pos);
    if (retval != OCI_ERROR && parmdpp != 0)
	printf("\t\treturns parmdp = %p\n", *parmdpp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIParamSet(
    dvoid	 *hdlp,
    ub4		 htyp,
    OCIError	 *errhp,
    CONST dvoid	 *dscp,
    ub4		 dtyp,
    ub4		 pos)
{
    sword	 retval;

    printf("OCIParamSet:\n");
    retval = pOCIParamSet(hdlp, htyp, errhp, dscp, dtyp, pos);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITransStart(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp,
    uword	 timeout,
    ub4		 flags )
{
    sword	 retval;

    printf("OCITransStart:\n");
    retval = pOCITransStart(svchp, errhp, timeout, flags);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITransDetach(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp,
    ub4		 flags )
{
    sword	 retval;

    printf("OCITransDetach:\n");
    retval = pOCITransDetach(svchp, errhp, flags);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITransCommit(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp,
    ub4		 flags)
{
    sword	 retval;

    printf("OCITransCommit:\n");
    retval = pOCITransCommit(svchp, errhp, flags);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITransRollback(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp,
    ub4		 flags)
{
    sword	 retval;

    printf("OCITransRollback:\n");
    retval = pOCITransRollback(svchp, errhp, flags);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITransPrepare(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp,
    ub4		 flags)
{
    sword	 retval;

    printf("OCITransPrepare:\n");
    retval = pOCITransPrepare(svchp, errhp, flags);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITransForget(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp,
    ub4		 flags)
{
    sword	 retval;

    printf("OCITransForget:\n");
    retval = pOCITransForget(svchp, errhp, flags);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIErrorGet(
    dvoid	 *hndlp,
    ub4		 recordno,
    OraText	 *sqlstate,
    sb4		 *errcodep,
    OraText	 *bufp,
    ub4		 bufsiz,
    ub4		 type)
{
    sword	 retval;

    printf("OCIErrorGet:\n");
    retval = pOCIErrorGet(hndlp, recordno, sqlstate, errcodep, bufp, bufsiz, type);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobAppend(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *dst_locp,
    OCILobLocator  *src_locp)
{
    sword	   retval;

    printf("OCILobAppend:\n");
    retval = pOCILobAppend(svchp, errhp, dst_locp, src_locp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobAssign(
    OCIEnv		 *envhp,
    OCIError		 *errhp,
    CONST OCILobLocator	 *src_locp,
    OCILobLocator	 **dst_locpp)
{
    sword		 retval;

    printf("OCILobAssign:\n");
    retval = pOCILobAssign(envhp, errhp, src_locp, dst_locpp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobCharSetForm(
    OCIEnv		 *envhp,
    OCIError		 *errhp,
    CONST OCILobLocator	 *locp,
    ub1			 *csfrm)
{
    sword		 retval;

    printf("OCILobCharSetForm:\n");
    retval = pOCILobCharSetForm(envhp, errhp, locp, csfrm);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobCharSetId(
    OCIEnv		 *envhp,
    OCIError		 *errhp,
    CONST OCILobLocator	 *locp,
    ub2			 *csid)
{
    sword		 retval;

    printf("OCILobCharSetId:\n");
    retval = pOCILobCharSetId(envhp, errhp, locp, csid);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobCopy(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *dst_locp,
    OCILobLocator  *src_locp,
    ub4		   amount,
    ub4		   dst_offset,
    ub4		   src_offset)
{
    sword	   retval;

    printf("OCILobCopy:\n");
    retval = pOCILobCopy(svchp, errhp, dst_locp, src_locp, amount, dst_offset, src_offset);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobCreateTemporary(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp,
    ub2		   csid,
    ub1		   csfrm,
    ub1		   lobtype,
    boolean	   cache,
    OCIDuration	   dur)
{
    sword	   retval;
    
    printf("OCILobCreateTemporary:\tlocp = %p\n", locp);
    printf("\t\t\tduration = %s\n", duration(dur));
    retval = pOCILobCreateTemporary(svchp, errhp, locp, csid, csfrm, lobtype, cache, dur);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCILobClose(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp )
{
    sword	   retval;

    printf("OCILobClose:\n");
    retval = pOCILobClose(svchp, errhp, locp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCILobDisableBuffering(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp)
{
    sword	   retval;

    printf("OCILobDisableBuffering:\n");
    retval = pOCILobDisableBuffering(svchp, errhp, locp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobEnableBuffering(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp)
{
    sword	   retval;

    printf("OCILobEnableBuffering:\n");
    retval = pOCILobEnableBuffering(svchp, errhp, locp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobErase(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp,
    ub4		   *amount,
    ub4		   offset)
{
    sword	   retval;
    
    printf("OCILobErase:\n");
    retval = pOCILobErase(svchp, errhp, locp, amount, offset);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobFileClose (
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *filep)
{
    sword	   retval;

    printf("OCILobFileClose:\n");
    retval = pOCILobFileClose(svchp, errhp, filep);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobFileCloseAll(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp)
{
    sword	 retval;

    printf("OCILobFileCloseAll:\n");
    retval = pOCILobFileCloseAll(svchp, errhp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobFileExists(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *filep,
    boolean	   *flag)
{
    sword	   retval;

    printf("OCILobFileExists:\n");
    retval = pOCILobFileExists(svchp, errhp, filep, flag);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobFileGetName(
    OCIEnv		 *envhp,
    OCIError		 *errhp,
    CONST OCILobLocator	 *filep,
    OraText		 *dir_alias,
    ub2			 *d_length,
    OraText		 *filename, 
    ub2			 *f_length)
{
    sword		 retval;

    printf("OCILobFileGetName:\n");
    retval = pOCILobFileGetName(envhp, errhp, filep, dir_alias, d_length, filename, f_length);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobFileIsOpen(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *filep,
    boolean	   *flag)
{
    sword	   retval;

    printf("OCILobFileIsOpen:\n");
    retval = pOCILobFileIsOpen(svchp, errhp, filep, flag);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobFileOpen(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *filep,
    ub1		   mode)
{
    sword	   retval;

    printf("OCILobFileOpen:\n");
    retval = pOCILobFileOpen(svchp, errhp, filep, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobFileSetName(
    OCIEnv	   *envhp,
    OCIError	   *errhp,
    OCILobLocator  **filepp,
    CONST OraText  *dir_alias, 
    ub2		   d_length,
    CONST OraText  *filename, 
    ub2		   f_length)
{
    sword	   retval;

    printf("OCILobFileSetName:\n");
    retval = pOCILobFileSetName(envhp, errhp, filepp, dir_alias, d_length, filename, f_length);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobFlushBuffer(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp,
    ub4		   flag)
{
    sword	   retval;

    printf("OCILobFlushBuffer:\n");
    retval = pOCILobFlushBuffer(svchp, errhp, locp, flag);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobFreeTemporary(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp)
{
    sword	   retval;

    printf("OCILobFreeTemporary:\n");
    retval = pOCILobFreeTemporary(svchp, errhp, locp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobGetChunkSize(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp,
    ub4		   *chunksizep)
{
    sword	   retval;

    printf("OCILobGetChunkSize:\n");
    retval = pOCILobGetChunkSize(svchp, errhp, locp, chunksizep);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobGetLength(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp,
    ub4		   *lenp)
{
    sword	   retval;

    printf("OCILobGetLength:\n");
    retval = pOCILobGetLength(svchp, errhp, locp, lenp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobIsEqual(
    OCIEnv		 *envhp,
    CONST OCILobLocator	 *x,
    CONST OCILobLocator	 *y,
    boolean		 *is_equal)
{
    sword		 retval;

    printf("OCILobIsEqual:\n");
    retval = pOCILobIsEqual(envhp, x, y, is_equal);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobIsOpen(
    OCISvcCtx     *svchp,
    OCIError      *errhp,
    OCILobLocator *locp,
    boolean       *flag)
{
    sword retval;

    printf("OCILobIsOpen:\n");
    retval = pOCILobIsOpen(svchp, errhp, locp, flag);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobIsTemporary(
    OCIEnv	   *envp,
    OCIError	   *errhp,
    OCILobLocator  *locp,
    boolean	   *is_temporary)
{
    sword	   retval;

    printf("OCILobIsTemporary:\n");
    retval = pOCILobIsTemporary(envp, errhp, locp, is_temporary);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobLoadFromFile(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *dst_locp,
    OCILobLocator  *src_filep,
    ub4		   amount, ub4 dst_offset,
    ub4		   src_offset)
{
    sword	   retval;

    printf("OCILobLoadFromFile:\n");
    retval = pOCILobLoadFromFile(svchp, errhp, dst_locp, src_filep, amount, dst_offset, src_offset);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobLocatorAssign(
    OCISvcCtx		 *svchp, 
    OCIError		 *errhp,
    CONST OCILobLocator	 *src_locp,
    OCILobLocator	 **dst_locpp)
{
    sword		 retval;

    printf("OCILobLocatorAssign:\n");
    retval = pOCILobLocatorAssign(svchp, errhp, src_locp, dst_locpp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCILobLocatorIsInit(
    OCIEnv		 *envhp,
    OCIError		 *errhp,
    CONST OCILobLocator	 *locp,
    boolean		 *is_initialized)
{
    sword		 retval;

    printf("OCILobLocatorIsInit:\n");
    retval = pOCILobLocatorIsInit(envhp, errhp, locp, is_initialized);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobOpen(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp,
    ub1		   mode )
{
    sword	   retval;

    printf("OCILobOpen:\n");
    retval = pOCILobOpen(svchp, errhp, locp, mode);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobRead(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp,
    ub4		   *amtp,
    ub4		   offset,
    dvoid	   *bufp,
    ub4		   bufl,
    dvoid	   *ctxp,
    sb4		   (*cbfp)(dvoid *ctxp, CONST dvoid *bufp, ub4 len, ub1 piece), 
    ub2		   csid, 
    ub1		   csfrm)
{
    sword	   retval;

    printf("OCILobRead:\n");
    retval = pOCILobRead(svchp, errhp, locp, amtp, offset, bufp, bufl, ctxp, cbfp, csid, csfrm);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobTrim(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp,
    ub4		   newlen)
{
    sword	   retval;

    printf("OCILobTrim:\n");
    retval = pOCILobTrim(svchp, errhp, locp, newlen);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobWrite(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *locp,
    ub4		   *amtp,
    ub4		   offset,
    dvoid	   *bufp,
    ub4		   buflen,
    ub1		   piece, dvoid *ctxp,
    sb4		   (*cbfp)(dvoid *ctxp, dvoid *bufp, ub4 *len, ub1 *piece),
    ub2		   csid,
    ub1		   csfrm)
{
    sword	   retval;

    printf("OCILobWrite:\n");
    retval = pOCILobWrite (svchp, errhp, locp, amtp, offset, bufp, buflen, piece, ctxp,
			   cbfp, csid, csfrm);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILobWriteAppend(
    OCISvcCtx	   *svchp,
    OCIError	   *errhp,
    OCILobLocator  *lobp,
    ub4		   *amtp,
    dvoid	   *bufp,
    ub4		   bufl,
    ub1		   piece,
    dvoid	   *ctxp,
    sb4		   (*cbfp)(dvoid *ctxp, dvoid *bufp, ub4 *len, ub1 *piece),
    ub2		   csid,
    ub1		   csfrm)
{
    sword	   retval;

    printf("OCILobWriteAppend:\n");
    retval = pOCILobWriteAppend(svchp, errhp, lobp, amtp, bufp, bufl, piece, ctxp,
				cbfp, csid, csfrm);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIBreak(
    dvoid	 *hndlp,
    OCIError	 *errhp)
{
    sword	 retval;

    printf("OCIBreak:\n");
    retval = pOCIBreak(hndlp, errhp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIReset (
    dvoid	 *hndlp,
    OCIError	 *errhp)
{
    sword	 retval;

    printf("OCIReset:\n");
    retval = pOCIReset(hndlp, errhp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIServerVersion(
    dvoid	 *hndlp,
    OCIError	 *errhp,
    OraText	 *bufp,
    ub4		 bufsz,
    ub1		 hndltype)
{
    sword	 retval;

    printf("OCIServerVersion:\n");
    retval = pOCIServerVersion(hndlp, errhp, bufp, bufsz, hndltype);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCISvcCtxToLda(
    OCISvcCtx	 *svchp,
    OCIError	 *errhp,
    Lda_Def	 *ldap)
{
    sword	 retval;

    printf("OCISvcCtxToLda:\n");
    retval = pOCISvcCtxToLda(svchp, errhp, ldap);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCILdaToSvcCtx(
    OCISvcCtx **svchpp, 
    OCIError *errhp, 
    Lda_Def *ldap)
{
    sword retval;

    printf("OCILdaToSvcCtx:\n");
    retval = pOCILdaToSvcCtx(svchpp, errhp, ldap);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIResultSetToStmt(
    OCIResult	 *rsetdp, 
    OCIError	 *errhp)
{
    sword	 retval;

    printf("OCIResultSetToStmt:\n");
    retval = pOCIResultSetToStmt(rsetdp, errhp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIFileClose(
    dvoid	   *hndl,
    OCIError	   *err,
    OCIFileObject  *filep )
{
    sword	   retval;

    printf("OCIFileClose:\n");
    retval = pOCIFileClose(hndl, err, filep);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIUserCallbackRegister(
    dvoid	     *hndlp, 
    ub4		     type, 
    dvoid	     *ehndlp,
    OCIUserCallback  callback, dvoid *ctxp,
    ub4		     fcode,
    ub4		     when)
{
    sword	     retval;

    printf("OCIUserCallbackRegister:\n");
    retval = pOCIUserCallbackRegister(hndlp, type, ehndlp, callback, ctxp, fcode, when);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIUserCallbackGet(
    dvoid	     *hndlp,
    ub4		     type,
    dvoid	     *ehndlp,
    ub4		     fcode,
    ub4		     when,
    OCIUserCallback  *callbackp,
    dvoid	     **ctxpp)
{
    sword            retval;

    printf("OCIUserCallbackGet:\n");
    retval = pOCIUserCallbackGet(hndlp, type, ehndlp, fcode, when, callbackp, ctxpp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIEnvCallback(
    OCIEnv	 *env, 
    ub4		 mode, 
    size_t	 xtramem_sz,
    dvoid	 *usrmemp)
{
    sword	 retval;

    printf("OCIEnvCallback:\n");
    retval = pOCIEnvCallback(env, mode, xtramem_sz, usrmemp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIFileExists(
    dvoid	 *hndl,
    OCIError	 *err,
    OraText	 *filename,
    OraText	 *path,
    ub1		 *flag )
{
    sword	 retval;

    printf("OCIFileExists:\n");
    retval = pOCIFileExists(hndl, err, filename, path, flag);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIFileFlush(
    dvoid	   *hndl,
    OCIError	   *err,
    OCIFileObject  *filep )
{
    sword	   retval;

    printf("OCIFileFlush:\n");
    retval = pOCIFileFlush(hndl, err, filep);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCIFileGetLength(
    dvoid	 *hndl, 
    OCIError	 *err, 
    OraText	 *filename,
    OraText	 *path, 
    ubig_ora	 *lenp )
{
    sword	 retval;

    printf("OCIFileGetLength:\n");
    retval = pOCIFileGetLength(hndl, err, filename, path, lenp);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIFileInit(
    dvoid	 *hndl,
    OCIError	 *err )
{
    sword	 retval;

    printf("OCIFileInit:\n");
    retval = pOCIFileInit(hndl, err);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIFileOpen(
    dvoid	   *hndl,
    OCIError	   *err,
    OCIFileObject  **filep,
    OraText	   *filename,
    OraText	   *path,
    ub4		   mode,
    ub4		   create,
    ub4		   type )
{
    sword	   retval;

    printf("OCIFileOpen:\n");
    retval = pOCIFileOpen(hndl, err, filep, filename, path, mode, create, type);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIFileRead(
    dvoid	   *hndl,
    OCIError	   *err,
    OCIFileObject  *filep,
    dvoid	   *bufp,
    ub4		   bufl,
    ub4		   *bytesread )
{
    sword	   retval;

    printf("OCIFileRead:\n");
    retval = pOCIFileRead(hndl, err, filep, bufp, bufl, bytesread);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIFileSeek(
    dvoid	   *hndl,
    OCIError	   *err,
    OCIFileObject  *filep,
    uword	   origin, 
    ubig_ora	   offset, 
    sb1		   dir )
{
    sword	   retval;

    printf("OCIFileSeek:\n");
    retval = pOCIFileSeek(hndl, err, filep, origin, offset, dir);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCIFileTerm(
    dvoid	 *hndl,
    OCIError	 *err )
{
    sword	 retval;

    printf("OCIFileTerm:\n");
    retval = pOCIFileTerm(hndl, err);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCIFileWrite(
    dvoid	   *hndl,
    OCIError	   *err,
    OCIFileObject  *filep,
    dvoid	   *bufp,
    ub4		   buflen,
    ub4		   *byteswritten )
{
    sword          retval;

    printf("OCIFileWrite:\n");
    retval = pOCIFileWrite(hndl, err, filep, bufp, buflen, byteswritten);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCITypeIterFree(
    OCIEnv	 *env,
    OCIError	 *err,
    OCITypeIter	 *iterator)
{
    sword	 retval;

    printf("OCITypeIterFree: iterator = %p\n", iterator);
    retval = pOCITypeIterFree(env, err, iterator);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITypeAttrNext(
    OCIEnv	 *env,
    OCIError	 *err,
    OCITypeIter	 *iterator,
    OCITypeElem	 **elem )
{
    sword	 retval;

    printf("OCITypeAttrNext:\titerator = %p\n", iterator);
    retval = pOCITypeAttrNext(env, err, iterator, elem);
    if (retval != OCI_ERROR)
	printf("\t\t\tnext elem at %p\n", *elem);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITypeByName(
    OCIEnv	     *env,
    OCIError	     *err,
    CONST OCISvcCtx  *svc,
    CONST text	     *schema_name,
    ub4		     s_length,
    CONST text	     *type_name,
    ub4		     t_length,
    CONST text	     *version_name,
    ub4		     v_length,
    OCIDuration	     pin_duration,
    OCITypeGetOpt    get_option,
    OCIType	     **tdo)
{
    sword	     retval;

    printf("OCITypeByName:\t\tsvc = %p\n", svc);
    printf("\t\t\tschema_name = %s\n", schema_name);
    printf("\t\t\ttype_name = %s\n", type_name);
    printf("\t\t\tversion_name = %s\n", version_name);
    printf("\t\t\tduration = %s\n", duration(pin_duration));
    printf("\t\t\tget_option = OCI_TYPEGET_%s\n",
	   get_option == OCI_TYPEGET_HEADER ? "HEADER" : "ALL");
    retval = pOCITypeByName(env, err, svc,
			    schema_name,  s_length,
			    type_name,  t_length,
			    version_name, v_length,
			    pin_duration, get_option, tdo);

    if (retval != OCI_ERROR)
	printf("\t\t\ttdo at %p\n", *tdo);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

text*
OCITypeName(
    OCIEnv	   *env,
    OCIError	   *err,
    CONST OCIType  *tdo,
    ub4		   *n_length)
{
    text	   *retval;

    printf("OCITypeName:\t\ttdo = %p\n", tdo);
    retval = pOCITypeName(env, err, tdo, n_length);
    printf("\t\treturns name = %s\n", retval);
    return retval;
}

text*
OCITypeElemName(
    OCIEnv	       *env,
    OCIError	       *err,
    CONST OCITypeElem  *elem,
    ub4		       *n_length )
{
    text	       *retval;

    printf("OCITypeElemName:\telem = %p\n", elem);
    retval = pOCITypeElemName(env, err, elem, n_length);
    printf("\t\treturns name = %s\n", retval);
    return retval;
}

OCITypeCode
OCITypeTypeCode(
    OCIEnv	   *env,
    OCIError	   *err,
    CONST OCIType  *tdo)
{
    OCITypeCode	   retval;

    printf("OCITypeTypeCode:\ttdo = %p\n", tdo);
    retval = pOCITypeTypeCode(env, err, tdo);
    printf("\t\treturns code = %s\n", typecode(retval));
    return retval;
}

OCITypeCode
OCITypeCollTypeCode(
    OCIEnv	   *env,
    OCIError	   *err,
    CONST OCIType  *tdo)
{
    OCITypeCode	   retval;

    printf("OCITypeCollTypeCode:\ttdo = %p\n", tdo);
    retval = pOCITypeCollTypeCode(env, err, tdo);
    printf("\t\treturns code = %s\n", typecode(retval));
    return retval;
}

OCITypeCode
OCITypeElemTypeCode(
    OCIEnv	       *env, 
    OCIError	       *err,
    CONST OCITypeElem  *elem)
{
    OCITypeCode	       retval;

    printf("OCITypeElemTypeCode:\telem = %p\n", elem);
    retval = pOCITypeElemTypeCode(env, err, elem);
    printf("\t\treturns code = %s\n", typecode(retval));
    return retval;
}

sword
OCITypeCollElem(
    OCIEnv	   *env, 
    OCIError	   *err, 
    CONST OCIType  *tdo,
    OCITypeElem	   **element)
{
    sword	   retval;

    printf("OCITypeCollElem:\ttdo = %p\n", tdo);
    retval = pOCITypeCollElem(env, err, tdo, element);
    if (retval != OCI_ERROR)
	printf("\t\treturns element at %p\n", *element);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITypeCollSize(
    OCIEnv	   *env, 
    OCIError	   *err, 
    CONST OCIType  *tdo, 
    ub4		   *num_elems)
{
    sword	   retval;

    printf("OCITypeCollElem:\t\ttdo = %p\n", tdo);
    retval = pOCITypeCollSize(env, err, tdo, num_elems);
    if (retval != OCI_ERROR)
	printf("\t\treturns num_elems = %d\n", *num_elems);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;

}

ub4
OCITypeElemLength(
    OCIEnv	       *env, 
    OCIError	       *err,
    CONST OCITypeElem  *elem)
{
    ub4		       retval;

    printf("OCITypeElemLength:\telem = %p\n", elem);
    retval = pOCITypeElemLength(env, err, elem);
    printf("\t\treturns length = %d\n", retval);

    return retval;
}

sword
OCITypeIterNew(
    OCIEnv	 *env, 
    OCIError	 *err, 
    OCIType	 *tdo, 
    OCITypeIter	 **iterator)
{
    sword	 retval;

    printf("OCITypeIterNew:\t\ttdo = %p\n", tdo);
    retval = pOCITypeIterNew(env, err, tdo, iterator);
    if (retval != OCI_ERROR)
	printf("\t\treturns iterator = %p\n", *iterator);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCITypeIterSet(
    OCIEnv	 *env, 
    OCIError	 *err, 
    OCIType	 *tdo, 
    OCITypeIter	 *iterator )
{
    sword	 retval;

    printf("OCITypeIterSet:\t\ttdo = %p\n", tdo);
    retval = pOCITypeIterSet(env, err, tdo, iterator);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITypeArrayByName(
    OCIEnv	     *env, 
    OCIError	     *err, 
    CONST OCISvcCtx  *svc,
    ub4		     array_len,
    CONST text	     *schema_name[], 
    ub4		     s_length[],
    CONST text	     *type_name[], 
    ub4		     t_length[],
    CONST text	     *version_name[], 
    ub4		     v_length[],
    OCIDuration	     pin_duration,
    OCITypeGetOpt    get_option, 
    OCIType	     *tdo[] )
{
    sword	     retval;

    printf("OCITypeArrayByName:");
    retval = pOCITypeArrayByName(env, err, svc, array_len,
				 schema_name, s_length,
				 type_name, t_length,
				 version_name, v_length,
				 pin_duration,
				 get_option, 
				 tdo);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCITypeByRef(
    OCIEnv	   *env, 
    OCIError	   *err,
    CONST OCIRef   *type_ref,
    OCIDuration	   pin_duration,
    OCITypeGetOpt  get_option, 
    OCIType	   **tdo )
{
    sword retval;

    printf("OCITypeByRef:\ttype_ref = %p\n", type_ref);
    printf("\t\t\tduration = %s\n", duration(pin_duration));
    retval = pOCITypeByRef(env, err, type_ref, pin_duration, get_option, tdo);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword
OCITypeArrayByRef(
    OCIEnv	   *env,
    OCIError	   *err,
    ub4		   array_len, 
    CONST OCIRef   *type_ref[],
    OCIDuration	   pin_duration,
    OCITypeGetOpt  get_option,
    OCIType	   *tdo[] )
{
    sword	   retval;

    printf("OCITypeArrayByRef:");
    retval = pOCITypeArrayByRef(env, err, array_len, type_ref,
				pin_duration, get_option, tdo);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
    
}

text*
OCITypeSchema(
    OCIEnv	   *env, 
    OCIError	   *err,
    CONST OCIType  *tdo, 
    ub4		   *n_length)
{
    text	   *retval;

    printf("OCITypeSchema:\ttdo = %p\n", tdo);
    retval = pOCITypeSchema(env, err, tdo, n_length);
    printf("\t\treturns name = %s\n", retval);
    return retval;
}

text*
OCITypeVersion(
    OCIEnv	   *env,
    OCIError	   *err, 
    CONST OCIType  *tdo, 
    ub4		   *v_length )
{
    text	   *retval;

    printf("OCITypeVersion:\ttdo = %p\n", tdo);
    retval = pOCITypeVersion(env, err, tdo, v_length);
    printf("\t\treturns name = %s\n", retval);
    return retval;
}

ub4 OCITypeAttrs(OCIEnv *env, 
		 OCIError *err,
		 CONST OCIType *tdo    )
{
    ub4 retval;

    printf("OCITypeAttrs:\ttdo = %p\n", tdo);
    retval = pOCITypeAttrs(env, err, tdo);
    printf("\t\treturns %d\n", retval);
    return retval;
}

ub4 OCITypeMethods(OCIEnv *env, 
		   OCIError *err,
		   CONST OCIType *tdo    )
{
    ub4 retval;

    printf("OCITypeMethods:\ttdo = %p\n", tdo);
    retval = pOCITypeMethods(env, err, tdo);
    printf("\t\treturns %d\n", retval);
    return retval;
}

sword OCITypeElemType(OCIEnv *env, 
		      OCIError *err, 
		      CONST OCITypeElem *elem, 
		      OCIType **elem_tdo)
{
    sword retval;

    printf("OCITypeElemType:\telem = %p\n", elem);
    retval = pOCITypeElemType(env, err, elem, elem_tdo);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

ub4 OCITypeElemFlags(    OCIEnv *env, OCIError *err,
                         CONST OCITypeElem *elem    )
{
    ub4 retval;

    printf("OCITypeElemFlags:\telem = %p\n", elem);
    retval = pOCITypeElemFlags(env, err, elem);
    printf("\t\treturns %d\n", retval);
    return retval;
}

ub1 OCITypeElemNumPrec(    OCIEnv *env, OCIError *err,
                           CONST OCITypeElem *elem    )
{
    ub1 retval;

    printf("OCITypeElemNumPrec:\telem = %p\n", elem);
    retval = pOCITypeElemNumPrec(env, err, elem);
    printf("\t\treturns %d\n", retval);
    return retval;
}

sb1 OCITypeElemNumScale(    OCIEnv *env, OCIError *err,
                            CONST OCITypeElem *elem    )
{
    sb1 retval;

    printf("OCITypeElemNumScale:\telem = %p\n", elem);
    retval = pOCITypeElemNumScale(env, err, elem);
    printf("\t\treturns %d\n", retval);
    return retval;
}

ub2 OCITypeElemCharSetID(    OCIEnv *env, OCIError *err,
                             CONST OCITypeElem *elem    )
{
    ub2 retval;

    printf("OCITypeElemCharSetID:\telem = %p\n", elem);
    retval = pOCITypeElemCharSetID(env, err, elem);
    printf("\t\treturns %d\n", retval);
    return retval;
}

ub2 OCITypeElemCharSetForm(    OCIEnv *env, OCIError *err,
                               CONST OCITypeElem *elem    )
{
    ub2 retval;

    printf("OCITypeElemCharSetForm:\telem = %p\n", elem);
    retval = pOCITypeElemCharSetForm(env, err, elem);
    printf("\t\treturns %d\n", retval);
    return retval;
}

sword OCITypeElemParameterizedType(    OCIEnv *env, OCIError *err,
                                       CONST OCITypeElem *elem, 
                                       OCIType **type_stored    )
{
    sword retval;

    printf("OCITypeElemParameterizedType:\telem = %p\n", elem);
    retval = pOCITypeElemParameterizedType(env, err, elem, type_stored);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


OCITypeCode OCITypeElemExtTypeCode(    OCIEnv *env, OCIError *err,
                                       CONST OCITypeElem *elem    )
{
    OCITypeCode retval;

    printf("OCITypeElemExtTypeCode:\telem = %p\n", elem);
    retval = pOCITypeElemExtTypeCode(env, err, elem);
    printf("\t\treturns code = %s\n", typecode(retval));
    return retval;
}

sword OCITypeAttrByName(    OCIEnv *env, OCIError *err, CONST OCIType *tdo, 
                            CONST text *name, ub4 n_length,
                            OCITypeElem **elem    )
{
    sword retval;

    printf("OCITypeAttrByName:\ttdo = %p\n", tdo);
    printf("\t\t\tname = %s\n", name);
    retval = pOCITypeAttrByName(env, err, tdo, name, n_length, elem);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword OCITypeCollExtTypeCode(OCIEnv *env, OCIError *err,
			     CONST OCIType *tdo, OCITypeCode *sqt_code    )
{
    sword retval;

    printf("OCITypeCollExtTypeCode:\ttdo = %p\n", tdo);
    retval = pOCITypeCollExtTypeCode(env, err, tdo, sqt_code);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

ub4 OCITypeMethodOverload(OCIEnv *env, 
			  OCIError *err, 
			  CONST OCIType *tdo, 
			  CONST text *method_name, 
			  ub4 m_length    )
{
    ub4 retval;

    printf("OCITypeMethodOverload:\ttdo = %p\n", tdo);
    printf("\t\t\tmethod_name = %s\n", method_name);
    retval = pOCITypeMethodOverload(env, err, tdo, method_name, m_length);
    printf("\t\treturns %d\n", retval);
    return retval;
}

sword OCITypeMethodByName(    OCIEnv *env, OCIError *err, CONST OCIType *tdo, 
                              CONST text *method_name, ub4 m_length,
                              OCITypeMethod **mdos    )
{
    sword retval;

    printf("OCITypeMethodByName:\ttdo = %p\n", tdo);
    printf("\t\t\tmethod_name = %s\n", method_name);
    retval = pOCITypeMethodByName(env, err, tdo, method_name, m_length, mdos);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword OCITypeMethodNext(OCIEnv *env, 
			OCIError *err,
			OCITypeIter *iterator,
			OCITypeMethod **mdo   )
{
    sword retval;

    printf("OCITypeMethodNext:\titerator = %p\n", iterator);
    retval = pOCITypeMethodNext(env, err, iterator, mdo);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

text *OCITypeMethodName(OCIEnv *env, 
			OCIError *err,
			CONST OCITypeMethod *mdo, 
			ub4 *n_length    )
{
    text *retval;

    printf("OCITypeMethodName:\tmdo = %p\n", mdo);
    retval = pOCITypeMethodName(env, err, mdo, n_length);
    printf("\t\treturns name = %s\n", retval);
    return retval;
}

OCITypeEncap OCITypeMethodEncap(OCIEnv *env, 
				OCIError *err,
				CONST OCITypeMethod *mdo)
{
    OCITypeEncap retval;

    printf("OCITypeMethodEncap:\tmdo = %p\n", mdo);
    retval = pOCITypeMethodEncap(env, err, mdo);
    printf("\t\treturns encap = %d\n", retval);
    return retval;
}

OCITypeMethodFlag OCITypeMethodFlags(OCIEnv *env, 
				     OCIError *err,
				     CONST OCITypeMethod *mdo    )
{
    OCITypeMethodFlag retval;

    printf("OCITypeMethodFlags:\tmdo = %p\n", mdo);
    retval = pOCITypeMethodFlags(env, err, mdo);
    printf("\t\treturns flag = %d\n", retval);
    return retval;
}

sword OCITypeMethodMap(    OCIEnv *env, 
			   OCIError *err, 
			   CONST OCIType *tdo, 
                           OCITypeMethod **mdo    )
{
    sword retval;

    printf("OCITypeMethodMap:\ttdo = %p\n", tdo);
    retval = pOCITypeMethodMap(env, err, tdo, mdo);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}


sword OCITypeMethodOrder(OCIEnv *env,
			 OCIError *err,
			 CONST OCIType *tdo, 
			 OCITypeMethod **mdo    )
{
    sword retval;

    printf("OCITypeMethodOrder:\ttdo = %p\n", tdo);
    retval = pOCITypeMethodOrder(env, err, tdo, mdo);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

ub4 OCITypeMethodParams(OCIEnv *env, 
			OCIError *err,
			CONST OCITypeMethod *mdo    )
{
    ub4 retval;

    printf("OCITypeMethodParams:\tmdo = %p\n", mdo);
    retval = pOCITypeMethodParams(env, err, mdo);
    printf("\t\treturns %d\n", retval);
    return retval;
}

sword OCITypeResult(OCIEnv *env,
		    OCIError *err,
		    CONST OCITypeMethod *mdo,
		    OCITypeElem **elem    )
{
    sword retval;

    printf("OCITypeResult:\tmdo = %p\n", mdo);
    retval = pOCITypeResult(env, err, mdo, elem);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword OCITypeParamByPos(OCIEnv *env, 
			OCIError *err,
			CONST OCITypeMethod *mdo, 
			ub4 position, 
			OCITypeElem **elem    )
{
    sword retval;

    printf("OCITypeParamByPos:\tmdo = %p\n", mdo);
    printf("\t\t\tpos = %d\n", position);
    retval = pOCITypeParamByPos(env, err, mdo, position, elem);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword OCITypeParamByName(OCIEnv *env, 
			 OCIError *err,
			 CONST OCITypeMethod *mdo, 
			 CONST text *name,
			 ub4 n_length,
			 OCITypeElem **elem    )
{
    sword retval;

    printf("OCITypeParamByName:\tmdo = %p\n", mdo);
    printf("\t\t\tname = %s\n", name);
    retval = pOCITypeParamByName(env, err, mdo, name, n_length, elem);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword OCITypeParamPos(OCIEnv *env, 
		      OCIError *err,
		      CONST OCITypeMethod *mdo, 
		      CONST text *name,
		      ub4 n_length, 
		      ub4 *position, 
		      OCITypeElem **elem)
{
    sword retval;

    printf("OCITypeParamPos:\tmdo = %p\n", mdo);
    printf("\t\t\tname = %s\n", name);
    retval = pOCITypeParamPos(env, err, mdo, name, n_length, position, elem);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

OCITypeParamMode
OCITypeElemParamMode(
    OCIEnv *env, 
    OCIError *err,
    CONST OCITypeElem *elem    )
{
    OCITypeParamMode retval;

    printf("OCITypeElemParamMode:\telem = %p\n", elem);
    retval = pOCITypeElemParamMode(env, err, elem);
    printf("\t\treturns = %d\n", retval);
    return retval;
}

text*
OCITypeElemDefaultValue(
    OCIEnv *env, 
    OCIError *err,
    CONST OCITypeElem *elem,
    ub4 *d_v_length    )
{
    text *retval;

    printf("OCITypeElemDefaultValue:\telem = %p\n", elem);
    retval = pOCITypeElemDefaultValue(env, err, elem, d_v_length);
    printf("\t\treturns = %s\n", retval);
    return retval;
}

sword OCITypeVTInit(OCIEnv *env, 
		    OCIError *err)
{
    sword retval;

    printf("OCITypeVTInit:\tenv = %p\n", env);
    retval = pOCITypeVTInit(env, err);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITypeVTInsert(
    OCIEnv *env,
    OCIError *err,
    CONST text *schema_name,
    ub4 s_n_length,
    CONST text *type_name,
    ub4 t_n_length,
    CONST text *user_version,
    ub4 u_v_length)
{
    sword	 retval;

    printf("OCITypeVTInsert:\tschema_name = %s\n", schema_name);
    printf("\t\t\ttype_name = %s\n", type_name);
    retval = pOCITypeVTInsert(env, err, schema_name, s_n_length,
			      type_name, t_n_length, user_version,
			      u_v_length);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
}

sword
OCITypeVTSelect(
    OCIEnv	 *env,
    OCIError	 *err,
    CONST text	 *schema_name,
    ub4		 s_n_length, 
    CONST text	 *type_name,
    ub4		 t_n_length, 
    text	 **user_version,
    ub4		 *u_v_length,
    ub2		 *version )
{
    sword	 retval;

    printf("OCITypeVTSelect:\tschema_name = %s\n", schema_name);
    printf("\t\t\ttype_name = %s\n", type_name);
    retval = pOCITypeVTSelect(env, err, schema_name, s_n_length,
			      type_name, t_n_length, user_version,
			      u_v_length, version);
    printf("\t\treturns %s\n", retcode(retval));
    return retval;
    
}
