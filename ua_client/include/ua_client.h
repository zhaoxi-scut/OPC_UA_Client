#ifndef UACLIENT_H
#define UACLIENT_H

#include <QObject>

#include <asmpro/opcua_cs/client.hpp>

class UaClient : public QObject
{
    Q_OBJECT

private:
    std::string __server_ip;
    ua::Client __client;
    UA_UInt32 __sub_id = 0U;
    UA_NodeId __server_id = UA_NODEID_NULL;

public:
    explicit UaClient(QObject *parent = nullptr);

    inline ua::Client &get() { return __client; }
    inline void setServerID(const UA_NodeId &server_id) { __server_id = server_id; }
    inline void setServerIP(const std::string &server_ip) { __server_ip = server_ip; }
    inline const UA_NodeId &getServerID() const { return __server_id; }
    inline const std::string &getServerIP() const { return __server_ip; }
    inline void setSubID(UA_UInt32 id) { __sub_id = id; }
    inline UA_UInt32 getSubID() { return __sub_id; }

    /**
     * @brief readCameraVariable
     *
     * @param node_id 对象节点ID
     * @retval exposure 曝光值
     * @retval gain 数字增益
     * @retval r_gain 红色增益
     * @retval g_gain 绿色增益
     * @retval b_gain 蓝色增益
     */
    std::tuple<UA_UInt16, UA_Double, UA_Double, UA_Double, UA_Double> readCameraVariable(const UA_NodeId &node_id);

    /**
     * @brief readLightControllerVariable
     *
     * @param node_id 对象节点ID
     * @return luminance 4个通道的亮度
     * @return delay 4个通道的延迟时间
     */
    std::tuple<std::array<UA_Byte, 4>, std::array<UA_UInt16, 4>> readLightControllerVariable(const UA_NodeId &node_id);

signals:

};

using client_ptr = std::shared_ptr<UaClient>;

#endif // UACLIENT_H
