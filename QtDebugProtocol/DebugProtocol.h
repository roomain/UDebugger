#pragma once
#include <QString>
#include <qjsonvalue.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <string>
#include <vector>
#include <list>
#include "TVarInfo.h"
#include "DebugLog.h"
#include "DebugInstrumentation.h"
#include "debugprotocol_global.h"
#include "classInfo.h"
#include "IDebugObject.h"

class DebugDBSerializer;
class DebugSerializer;
class DebugDeserializer;

class DEBUGPROTOCOL_EXPORT DebugProtocol
{
public:

    struct VariableData
    {
        QString m_sName;
        Debugger::EVarType m_type;
        QVariant m_value;
        bool m_bReadOnly;
    };

    struct ClassCompareData : Debugger::ClassInfo
    {
        int64_t m_uid;
        std::vector<VariableData> m_variables;
    };

private:
    static const QString JSON_CLASS_NAME;
    static const QString JSON_CLASS_SIZE;
    static const QString JSON_CLASS_INHERIT;
    static const QString JSON_NAME;
    static const QString JSON_VALUE;
    static const QString JSON_UID;
    static const QString JSON_UIDS;
    static const QString JSON_VIEW;
    static const QString JSON_OWNER;
    static const QString JSON_OBJECT;
    static const QString JSON_PACKET;
    static const QString JSON_OBJ_LIST;
    static const QString JSON_OBJ_PROP;
    static const QString JSON_OBJ_INST;
    static const QString JSON_VAR_INDEX;
    static const QString JSON_VAR_TYPE;
    static const QString JSON_READ_ONLY;
    static const QString JSON_LOG_MESSAGE;
    static const QString JSON_LOG_SEVERITY;
    static const QString JSON_LOCATION_FILE;
    static const QString JSON_LOCATION_FUN;
    static const QString JSON_LOCATION_LINE;
    static const QString JSON_DURATION;
    static const QString JSON_TIME;

    QJsonObject m_object;

    static void read(const QJsonObject& a_object, ClassCompareData& a_data);

public:
    enum class DebugCMDType
    {
        cmd_unknown,
        cmd_List,		// list all objects
        cmd_PropRead,	// read properties of an object
        cmd_PropWrite,	// write an object property
        cmd_Compare
    };

    enum class DebugAnsType
    {
        ans_unknown,
        ans_List,
        ans_PropRead,
        ans_PropWrite,
        ans_Log,
        ans_Instrument,
        ans_OwnerChange,
        ans_ObjRemoved,
        ans_Compare
    };

    DebugAnsType clientRead(const QByteArray& a_packet);
    DebugCMDType serverRead(const QByteArray& a_packet);


    template<typename Type>
    static QJsonValue toValue(const Type& a_value) { return QJsonValue; }

    template<>
    static QJsonValue toValue<>(const bool& a_value) { return QJsonValue(a_value); }

    template<>
    static QJsonValue toValue<>(const short& a_value) { return QJsonValue(a_value); }

    template<>
    static QJsonValue toValue<>(const unsigned short& a_value) { return QJsonValue(a_value); }

    template<>
    static QJsonValue toValue<>(const int& a_value) { return QJsonValue(a_value); }

    template<>
    static QJsonValue toValue<>(const unsigned int& a_value) { return QJsonValue(static_cast<int>(a_value)); }

    template<>
    static QJsonValue toValue<>(const float& a_value) { return QJsonValue(a_value); }

    template<>
    static QJsonValue toValue<>(const double& a_value) { return QJsonValue(a_value); }

    template<>
    static QJsonValue toValue<>(const std::string& a_value) { return QJsonValue(QString::fromLatin1(a_value)); }

    template<>
    static QJsonValue toValue<>(const int64_t& a_value)
    { return QJsonValue(static_cast<qint64>(a_value)); }

    template<typename Type>
    static QJsonObject writeVariable(const Debugger::VariableInfo<Type>& a_var)
    {
        QJsonObject variable;
        variable.insert(JSON_NAME, QString::fromLatin1(a_var.m_name));
        variable.insert(JSON_VALUE, toValue(a_var.m_value));
        variable.insert(JSON_READ_ONLY, a_var.m_bReadOnly);
        variable.insert(JSON_VAR_TYPE, QJsonValue(static_cast<int>(a_var.m_type)));
        return variable;
    }


    static void readVariable(const QJsonObject& a_object, VariableData& a_varData);
    QString toString()const;


    static QJsonObject writeObjectInfo(const std::string_view& a_objectName, const std::string_view& a_className,
        const int64_t& a_uid, const int64_t& a_owner);

    static QJsonObject writeObject(const std::string_view& a_objectName, const Debugger::ClassInfo& a_classInfo,
        const int64_t& a_uid, const int64_t& a_owner, const QJsonArray& a_properies);

    static QJsonObject writeObjectOwnerChange(const int64_t& a_uid, const int64_t& a_owner);

    struct Instance
    {
        QString m_sName;
        Debugger::ClassInfo m_classInfo;
        int64_t m_uid;
        int64_t m_ownerUID;
    };

    static Instance readObjectInfo(const QJsonObject& a_object);

    static void readObject(const QJsonObject& a_object, QString& a_objectName, Debugger::ClassInfo& a_classInfo,
        int64_t& a_uid, int64_t& a_owner, std::vector<VariableData>& a_variables);
    
    //---------------------------------------------------------------------------------------
    struct LogMessage
    {
        Debugger::LogSeverity m_severity = Debugger::LogSeverity::log_info;
        QString m_sTime;
        QString m_sFileLocation;
        QString m_sFunctionLocation;
        int m_iLineLocation;
        QString m_logMessage;
    };
    static QByteArray genOwnerChangePacket(const int64_t& a_uid, const int64_t& a_owner);
    void readOwnerChangePacket(int64_t& a_uid, int64_t& a_owner);

    static QByteArray genLogPacket(const Debugger::Log& a_log);
    void readLogPacket(LogMessage& a_logMessage);

    static QByteArray askList(const int a_iView);
    void readAskList(int& a_iView);
    static QByteArray genListPacket(const DebugDBSerializer& a_serializer, const int a_iView);
    void readListPacket(std::list<Instance>& a_lInstance, int& a_iView);

    static QByteArray genPropPacket(const DebugSerializer& a_serializer, const int a_iView);
    void readPropPacket(int64_t& a_uid, Debugger::ClassInfo& a_info, std::vector<VariableData>& a_variables, int& a_iView);

    static QByteArray genRemovePacket(const int64_t& a_uid);
    void readRemovePacket(int64_t& a_uid);

    static QByteArray genReadPropPacket(const int64_t& a_uid, const int a_iView);
    void readReadPropPacket(int64_t& a_uid, int& a_iView);


    static QByteArray genReadPropComparePacket(const int64_t& a_uid0, const int64_t& a_uid1, const int a_iView);
    void readReadPropComparePacket(int64_t& a_uid0, int64_t& a_uid1, int& a_iView);

    static QByteArray genPropComparePacket(const int64_t& a_uid0, const QJsonObject& a_data0,
        const int64_t& a_uid1, const QJsonObject& a_data1, const int a_iView);

    void reaPropComparePacket(ClassCompareData& a_data0, ClassCompareData& a_data1, int& a_iView);

    static QByteArray genWritePropPacket(const int64_t& a_uid, const int a_index, const QVariant& a_value);
    void readWritePropPacket(DebugDeserializer& a_deserialize, int64_t& a_uid, int& a_index);

    static QByteArray genInstrumentationPacket(const Debugger::InstrumentationData& a_instrumentation);
    void readInstrumentationPacket(QString& a_name, uint& a_time);
};
