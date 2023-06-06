#include "DebugProtocol.h"
#include "PacketProcessing.h"
#include <qjsonarray.h>
#include <qjsonvalue.h>
#include <ctime>

#include "DebugSerializer.h"
#include "DebugDeserializer.h"


const QString DebugProtocol::JSON_CLASS_NAME = "#JSON_CLASS_NAME";
const QString DebugProtocol::JSON_CLASS_SIZE = "#JSON_CLASS_SIZE";
const QString DebugProtocol::JSON_CLASS_INHERIT = "#JSON_INHERIT";
const QString DebugProtocol::JSON_NAME = "#JSON_NAME";
const QString DebugProtocol::JSON_VALUE = "#JSON_VALUE";
const QString DebugProtocol::JSON_UID = "#JSON_UID";
const QString DebugProtocol::JSON_UIDS = "#JSON_UIDS";
const QString DebugProtocol::JSON_VIEW = "#JSON_VIEW";
const QString DebugProtocol::JSON_OWNER = "#JSON_OWNER";
const QString DebugProtocol::JSON_OBJECT = "#JSON_OBJECT";
const QString DebugProtocol::JSON_PACKET = "#JSON_PACKET";
const QString DebugProtocol::JSON_OBJ_LIST = "#JSON_OBJ_LIST";
const QString DebugProtocol::JSON_OBJ_PROP = "#JSON_OBJ_PROPERTIES";
const QString DebugProtocol::JSON_OBJ_INST = "#JSON_OBJ_INSTANCE";
const QString DebugProtocol::JSON_VAR_INDEX = "#JSON_VAR_INDEX";
const QString DebugProtocol::JSON_VAR_TYPE = "#JSON_VAR_TYPE";
const QString DebugProtocol::JSON_READ_ONLY = "#JSON_READ_ONLY";
const QString DebugProtocol::JSON_LOG_MESSAGE = "#JSON_LOG_MESSAGE";
const QString DebugProtocol::JSON_LOG_SEVERITY = "#JSON_LOG_SEVERITY";
const QString DebugProtocol::JSON_LOCATION_FILE = "#JSON_LOC_FILE";
const QString DebugProtocol::JSON_LOCATION_FUN = "#JSON_LOC_FUN";
const QString DebugProtocol::JSON_LOCATION_LINE = "#JSON_LOC_LINE";
const QString DebugProtocol::JSON_DURATION = "#JSON_TIME";
const QString DebugProtocol::JSON_TIME = "#JSON_TIME";

DebugProtocol::DebugAnsType DebugProtocol::clientRead(const QByteArray& a_packet)
{
    DebugAnsType type = DebugAnsType::ans_unknown;
    QJsonParseError error;
    m_object = QJsonObject();
    QJsonDocument doc = QJsonDocument::fromJson(a_packet, &error);
    if (error.error == QJsonParseError::NoError)
    {
        m_object = doc.object();
        type = static_cast<DebugAnsType>(m_object.value(JSON_PACKET).toInt());
    }
    return type;
}

DebugProtocol::DebugCMDType DebugProtocol::serverRead(const QByteArray& a_packet)
{
    DebugCMDType type = DebugCMDType::cmd_unknown;
    QJsonParseError error;
    m_object = QJsonObject();
    QJsonDocument doc = QJsonDocument::fromJson(a_packet, &error);
    if (error.error == QJsonParseError::NoError)
    {
        m_object = doc.object();
        type = static_cast<DebugCMDType>(m_object.value(JSON_PACKET).toInt());
    }
    return type;
}


void DebugProtocol::readVariable(const QJsonObject& a_object, VariableData& a_varData)
{
    a_varData.m_sName = a_object.value(JSON_NAME).toString();
    a_varData.m_type = static_cast<Debugger::EVarType>(a_object.value(JSON_VAR_TYPE).toInt());
    a_varData.m_value = a_object.value(JSON_VALUE).toVariant();
    a_varData.m_bReadOnly = a_object.value(JSON_READ_ONLY).toBool();
}

QJsonObject DebugProtocol::writeObjectInfo(const std::string_view& a_objectName, const std::string_view& a_className,
    const int64_t& a_uid, const int64_t& a_owner)
{
    QJsonObject obj;
    obj.insert(JSON_NAME, QString::fromLatin1(a_objectName));
    obj.insert(JSON_CLASS_NAME, QString::fromLatin1(a_className));
    obj.insert(JSON_UID, QJsonValue(static_cast<int64_t>(a_uid)));
    obj.insert(JSON_OWNER, QJsonValue(static_cast<int64_t>(a_owner)));
    return obj;
}

QJsonObject DebugProtocol::writeObject(const std::string_view& a_objectName, const Debugger::ClassInfo& a_classInfo,
    const int64_t& a_uid, const int64_t& a_owner, const QJsonArray& a_properies)
{
    QJsonObject obj;
    obj.insert(JSON_NAME, QString::fromLatin1(a_objectName));
    obj.insert(JSON_CLASS_NAME, QString::fromLatin1(a_classInfo.m_className));
    obj.insert(JSON_CLASS_INHERIT, QString::fromLatin1(a_classInfo.m_inheritance));
    obj.insert(JSON_CLASS_SIZE, QJsonValue(static_cast<int64_t>(a_classInfo.m_classSize)));
    obj.insert(JSON_UID, QJsonValue(static_cast<int64_t>(a_uid)));
    obj.insert(JSON_OWNER, QJsonValue(static_cast<int64_t>(a_owner)));
    obj.insert(JSON_OBJ_PROP, a_properies);
    return obj;
}

QJsonObject DebugProtocol::writeObjectOwnerChange(const int64_t& a_uid, const int64_t& a_owner)
{
    QJsonObject obj;
    obj.insert(JSON_UID, QJsonValue(static_cast<int64_t>(a_uid)));
    obj.insert(JSON_OWNER, QJsonValue(static_cast<int64_t>(a_owner)));
    return obj;
}

DebugProtocol::Instance DebugProtocol::readObjectInfo(const QJsonObject& a_object)
{
    Instance instance;
    instance.m_sName = a_object.value(JSON_NAME).toString();
    instance.m_classInfo.m_className = a_object.value(JSON_CLASS_NAME).toString().toStdString();
    instance.m_classInfo.m_inheritance = a_object.value(JSON_CLASS_INHERIT).toString().toStdString();
    instance.m_classInfo.m_classSize = a_object.value(JSON_CLASS_SIZE).toVariant().toULongLong();
    instance.m_uid = a_object.value(JSON_UID).toVariant().toULongLong();
    instance.m_ownerUID = a_object.value(JSON_OWNER).toVariant().toULongLong();
    return instance;
}

void DebugProtocol::readObject(const QJsonObject& a_object, QString& a_objectName, Debugger::ClassInfo& a_classInfo,
    int64_t& a_uid, int64_t& a_owner, std::vector<VariableData>& a_variables)
{
    a_objectName = a_object.value(JSON_NAME).toString();
    a_classInfo.m_className = a_object.value(JSON_CLASS_NAME).toString().toStdString();
    a_classInfo.m_inheritance = a_object.value(JSON_CLASS_INHERIT).toString().toStdString();
    a_classInfo.m_classSize = a_object.value(JSON_CLASS_SIZE).toVariant().toULongLong();
    a_uid = a_object.value(JSON_UID).toVariant().toULongLong();
    a_owner = a_object.value(JSON_OWNER).toVariant().toULongLong();
    auto arrayVar = a_object.value(JSON_OBJ_PROP).toArray();
    a_variables.resize(arrayVar.size());
    int index = 0;
    for (const auto& variable : arrayVar)
    {
        readVariable(variable.toObject(), a_variables[index]);
        ++index;
    }
}

//---------------------------------------------------------------------------------------
QByteArray DebugProtocol::genOwnerChangePacket(const int64_t& a_uid, const int64_t& a_owner)
{
    QJsonObject packet = writeObjectOwnerChange(a_uid, a_owner);
    packet.insert(JSON_PACKET, static_cast<int>(DebugProtocol::DebugAnsType::ans_OwnerChange));
    QJsonDocument doc(packet);
    QByteArray baPacket = doc.toJson();
    PacketProcessing::addPacketSize(baPacket);
    return baPacket;
}

void DebugProtocol::readOwnerChangePacket(int64_t& a_uid, int64_t& a_owner)
{
    if (!m_object.isEmpty())
    {
        a_uid = m_object.value(JSON_UID).toVariant().toLongLong();
        a_owner = m_object.value(JSON_OWNER).toVariant().toLongLong();
    }
}

QByteArray DebugProtocol::genLogPacket(const Debugger::Log& a_log)
{
    using namespace std::literals;
    QJsonObject packet;
    packet.insert(JSON_PACKET, static_cast<int>(DebugProtocol::DebugAnsType::ans_Log));
    packet.insert(JSON_LOCATION_FILE, QString(a_log.m_location.file_name()));
    packet.insert(JSON_LOCATION_FUN, QString(a_log.m_location.function_name()));
    packet.insert(JSON_LOCATION_LINE, QJsonValue(static_cast<int>(a_log.m_location.line())));
    packet.insert(JSON_LOG_MESSAGE, QString::fromLatin1(a_log.m_logMessage));
    packet.insert(JSON_LOG_SEVERITY, static_cast<int>(a_log.m_severity));
    const std::time_t time_clock = std::chrono::system_clock::to_time_t(a_log.m_time - 24h);
    std::tm localTime = *std::localtime(&time_clock);
    packet.insert(JSON_TIME, QString("%1:%2.%3").arg(localTime.tm_hour).arg(localTime.tm_min).arg(localTime.tm_sec));

    QJsonDocument doc(packet);
    QByteArray baPacket = doc.toJson();
    PacketProcessing::addPacketSize(baPacket);
    return baPacket;
}

void DebugProtocol::readLogPacket(LogMessage& a_logMessage)
{
    if (!m_object.isEmpty())
    {
        a_logMessage.m_severity = static_cast<Debugger::LogSeverity>(m_object.value(JSON_LOG_SEVERITY).toInt());
        a_logMessage.m_sFileLocation = m_object.value(JSON_LOCATION_FILE).toString();
        a_logMessage.m_sFunctionLocation = m_object.value(JSON_LOCATION_FUN).toString();
        a_logMessage.m_iLineLocation = m_object.value(JSON_LOCATION_LINE).toInt();
        a_logMessage.m_logMessage = m_object.value(JSON_LOG_MESSAGE).toString();
        a_logMessage.m_sTime = m_object.value(JSON_TIME).toString();
    }
}

QByteArray DebugProtocol::askList(const int a_iView)
{
    QJsonObject packet;
    packet.insert(JSON_PACKET, static_cast<int>(DebugProtocol::DebugCMDType::cmd_List));
    packet.insert(JSON_VIEW, QJsonValue(a_iView));
    QJsonDocument doc(packet);
    QByteArray baPacket = doc.toJson();
    PacketProcessing::addPacketSize(baPacket);
    return baPacket;
}

void DebugProtocol::readAskList(int& a_iView)
{
    if (!m_object.isEmpty())
    {
        a_iView = m_object.value(JSON_VIEW).toInt();
    }
}

QByteArray DebugProtocol::genListPacket(const DebugDBSerializer& a_serializer, const int a_iView)
{
    QJsonObject packet;
    packet.insert(JSON_PACKET, static_cast<int>(DebugProtocol::DebugAnsType::ans_List));
    packet.insert(JSON_OBJ_LIST, a_serializer.list());
    packet.insert(JSON_VIEW, QJsonValue(a_iView));
    QJsonDocument doc(packet);
    QByteArray baPacket = doc.toJson();
    PacketProcessing::addPacketSize(baPacket);
    return baPacket;
}

void DebugProtocol::readListPacket(std::list<Instance>& a_lInstance, int& a_iView)
{
    if (!m_object.isEmpty())
    {
        a_iView = m_object.value(JSON_VIEW).toInt();
        auto arrayObj = m_object.value(JSON_OBJ_LIST).toArray();
        for (const auto& instRef : arrayObj)
        {
            auto inst = instRef.toObject();
            a_lInstance.emplace_back(readObjectInfo(inst));
        }
    }
}

QByteArray DebugProtocol::genPropPacket(const DebugSerializer& a_serializer, const int a_iView)
{
    QJsonObject packet;
    packet.insert(JSON_PACKET, static_cast<int>(DebugProtocol::DebugAnsType::ans_PropRead));
    packet.insert(JSON_UID, QJsonValue(static_cast<int64_t>(a_serializer.objectId())));
    packet.insert(JSON_OBJ_INST, a_serializer.object());
    packet.insert(JSON_VIEW, QJsonValue(a_iView));

    QJsonDocument doc(packet);
    QByteArray baPacket = doc.toJson();
    PacketProcessing::addPacketSize(baPacket);
    return baPacket;
}

QByteArray DebugProtocol::genPropComparePacket(const int64_t& a_uid0, const QJsonObject& a_data0,
    const int64_t& a_uid1, const QJsonObject& a_data1, const int a_iView)
{
    QJsonObject packet;
    packet.insert(JSON_PACKET, static_cast<int>(DebugProtocol::DebugAnsType::ans_Compare));
    QJsonArray arrayUID;
    QJsonObject uidData0;
    uidData0.insert(JSON_UID, QJsonValue(a_uid0));
    uidData0.insert(JSON_OBJ_INST, a_data0);
    arrayUID.append(uidData0);
    QJsonObject uidData1;
    uidData1.insert(JSON_UID, QJsonValue(a_uid1));
    uidData1.insert(JSON_OBJ_INST, a_data1);
    arrayUID.append(uidData1);
    packet.insert(JSON_UIDS, arrayUID);

    packet.insert(JSON_VIEW, QJsonValue(a_iView));

    QJsonDocument doc(packet);
    QByteArray baPacket = doc.toJson();
    PacketProcessing::addPacketSize(baPacket);
    return baPacket;
}

void DebugProtocol::read(const QJsonObject& a_object, ClassCompareData& a_data)
{
    a_data.m_uid = a_object.value(JSON_UID).toVariant().toLongLong();
    auto objInst = a_object.value(JSON_OBJ_INST).toObject();
    a_data.m_className = objInst.value(JSON_CLASS_NAME).toString().toStdString();
    a_data.m_inheritance = objInst.value(JSON_CLASS_INHERIT).toString().toStdString();
    a_data.m_classSize = objInst.value(JSON_CLASS_SIZE).toVariant().toLongLong();
    auto props = objInst.value(JSON_OBJ_PROP).toArray();
    a_data.m_variables.resize(props.size());
    int index = 0;
    for (const auto& cProp : props)
    {
        readVariable(cProp.toObject(), a_data.m_variables[index]);
        ++index;
    }
}

void DebugProtocol::reaPropComparePacket(ClassCompareData& a_data0, ClassCompareData& a_data1, int& a_iView)
{
    if (!m_object.isEmpty())
    {
        a_iView = m_object.value(JSON_VIEW).toInt();
        auto arrayUID = m_object.value(JSON_UIDS).toArray();
        if (arrayUID.size() == 2)
        {
            read(arrayUID[0].toObject(), a_data0);
            read(arrayUID[1].toObject(), a_data1);
        }
    }
}

QString DebugProtocol::toString()const
{
    QJsonDocument doc(m_object);
    return doc.toJson();
}

void DebugProtocol::readPropPacket(int64_t& a_uid, Debugger::ClassInfo& a_info, std::vector<VariableData>& a_variables, int& a_iView)
{
    if (!m_object.isEmpty())
    {
        a_iView = m_object.value(JSON_VIEW).toInt();
        a_uid = m_object.value(JSON_UID).toVariant().toLongLong();
        auto objInst = m_object.value(JSON_OBJ_INST).toObject();

        a_info.m_className = objInst.value(JSON_CLASS_NAME).toString().toStdString();
        a_info.m_inheritance = objInst.value(JSON_CLASS_INHERIT).toString().toStdString();
        a_info.m_classSize = objInst.value(JSON_CLASS_SIZE).toVariant().toLongLong();

        auto props = objInst.value(JSON_OBJ_PROP).toArray();
        a_variables.resize(props.size());
        int index = 0;
        for (const auto& cProp : props)
        {
            readVariable(cProp.toObject(), a_variables[index]);
            ++index;
        }
    }
}

QByteArray DebugProtocol::genRemovePacket(const int64_t& a_uid)
{
    QJsonObject packet;
    packet.insert(JSON_PACKET, static_cast<int>(DebugProtocol::DebugAnsType::ans_ObjRemoved));
    packet.insert(JSON_UID, QJsonValue(static_cast<int64_t>(a_uid)));
    QJsonDocument doc(packet);
    QByteArray baPacket = doc.toJson();
    PacketProcessing::addPacketSize(baPacket);
    return baPacket;
}

void DebugProtocol::readRemovePacket(int64_t& a_uid)
{
    if (!m_object.isEmpty())
        a_uid = m_object.value(JSON_UID).toVariant().toLongLong();
}

QByteArray DebugProtocol::genReadPropPacket(const int64_t& a_uid, const int a_iView)
{
    QJsonObject packet;
    packet.insert(JSON_PACKET, static_cast<int>(DebugProtocol::DebugCMDType::cmd_PropRead));
    packet.insert(JSON_UID, QJsonValue(static_cast<int64_t>(a_uid)));
    packet.insert(JSON_VIEW, QJsonValue(a_iView));
    QJsonDocument doc(packet);
    QByteArray baPacket = doc.toJson();
    PacketProcessing::addPacketSize(baPacket);
    return baPacket;
}

void DebugProtocol::readReadPropPacket(int64_t& a_uid, int& a_iView)
{
    a_iView = m_object.value(JSON_VIEW).toInt();
    if (!m_object.isEmpty())
        a_uid = m_object.value(JSON_UID).toVariant().toLongLong();
}
QByteArray DebugProtocol::genReadPropComparePacket(const int64_t& a_uid0, const int64_t& a_uid1, const int a_iView)
{
    QJsonObject packet;
    packet.insert(JSON_PACKET, static_cast<int>(DebugProtocol::DebugCMDType::cmd_Compare));
    QJsonArray arrayUID;
    arrayUID.append(QJsonValue(static_cast<int64_t>(a_uid0)));
    arrayUID.append(QJsonValue(static_cast<int64_t>(a_uid1)));
    packet.insert(JSON_UIDS, arrayUID);
    packet.insert(JSON_VIEW, QJsonValue(a_iView));
    QJsonDocument doc(packet);
    QByteArray baPacket = doc.toJson();
    PacketProcessing::addPacketSize(baPacket);
    return baPacket;
}

void DebugProtocol::readReadPropComparePacket(int64_t& a_uid0, int64_t& a_uid1, int& a_iView)
{
    a_uid0 = -1;
    a_uid1 = -1;
    a_iView = m_object.value(JSON_VIEW).toInt();
    if (!m_object.isEmpty())
    {
        auto arrayUID = m_object.value(JSON_UIDS).toArray();
        if (arrayUID.size() == 2)
        {
            a_uid0 = arrayUID[0].toVariant().toLongLong();
            a_uid1 = arrayUID[1].toVariant().toLongLong();
        }
    }
}

QByteArray DebugProtocol::genWritePropPacket(const int64_t& a_uid, const int a_index, const QVariant& a_value)
{
    QJsonObject packet;
    packet.insert(JSON_PACKET, static_cast<int>(DebugProtocol::DebugCMDType::cmd_PropWrite));
    packet.insert(JSON_UID, QJsonValue(static_cast<int64_t>(a_uid)));
    packet.insert(JSON_VAR_INDEX, a_index);
    packet.insert(JSON_VALUE, QJsonValue::fromVariant(a_value));
    QJsonDocument doc(packet);
    QByteArray baPacket = doc.toJson();
    PacketProcessing::addPacketSize(baPacket);
    return baPacket;
}

void DebugProtocol::readWritePropPacket(DebugDeserializer& a_deserialize, int64_t& a_uid, int& a_index)
{
    if (!m_object.isEmpty())
    {
        a_uid = m_object.value(JSON_UID).toVariant().toLongLong();
        a_index = m_object.value(JSON_VAR_INDEX).toInt();
        a_deserialize.setValue(m_object.value(JSON_VALUE).toVariant());
    }
}

QByteArray DebugProtocol::genInstrumentationPacket(const Debugger::InstrumentationData& a_instrumentation)
{
    QJsonObject packet;
    packet.insert(JSON_PACKET, static_cast<int>(DebugProtocol::DebugAnsType::ans_Instrument));
    packet.insert(JSON_NAME, QString::fromLatin1(a_instrumentation.m_name));
    packet.insert(JSON_DURATION, a_instrumentation.m_duration.count());
    QJsonDocument doc(packet);
    QByteArray baPacket = doc.toJson();
    PacketProcessing::addPacketSize(baPacket);
    return baPacket;
}

void DebugProtocol::readInstrumentationPacket(QString& a_name, uint& a_time)
{
    if (!m_object.isEmpty())
    {
        a_name = m_object.value(JSON_NAME).toString();
        a_time = m_object.value(JSON_DURATION).toVariant().toUInt();
    }
}