#ifndef CLIENT_UI_H
#define CLIENT_UI_H

#include <mutex>
#include <thread>
#include <condition_variable>

#include <QWidget>
#include <QLabel>

#include "connect_ui.h"
#include "ua_client.h"

namespace Ui {
class ClientUI;
}

class ClientUI : public QWidget
{
    Q_OBJECT

    enum class DeviceType : uint8_t
    {
        NONE = 0,
        CAMERA = 1,
        LIGHT = 2
    };
    enum class LogType : uint8_t
    {
        Info = 0,
        Warning = 1,
        Success = 2,
        Error = 3
    };

public:
    static QLabel *label_image;

private:
    //! 设备类型
    DeviceType __device_type = DeviceType::NONE;
    //! 目标订阅节点 ID
    UA_NodeId __target_id = UA_NODEID_NULL;
    //! 设备索引列表
    std::vector<QString> __devices_idx;
    //! OPC UA Client based on QObject
    client_ptr __ua_client = nullptr;
    //! ConnectUI 指针
    ConnectUI *__connect = nullptr;

    //! 互斥锁
    std::mutex __mutex;
    //! 条件变量
    std::condition_variable __cond;
    //! runIterate 发布线程
    std::thread __iterate_thread;

    Ui::ClientUI *ui;

public:
    explicit ClientUI(client_ptr ua_client, QWidget *parent = nullptr);
    ~ClientUI();

    /**
     * @brief Bind connection
     *
     * @param p_connect Connect pointer
     */
    inline void bindConnect(ConnectUI *p_connect) { __connect = p_connect; }

private:
    //! Initialize the signal and slots
    void initSignalSlots();
    //! Initialize the UI
    void initUI();

    /**
     * @brief log
     *
     * @param log_type Log 类型
     * @param str Log 信息
     */
    void log(LogType log_type, const QString &str);
    //! ua::Client::runIterate
    void runIterate();
    //! monitorClear
    void monitorClear();
    //! disconnectUI
    void disconnectUI();
};

inline QLabel *ClientUI::label_image = nullptr;

#endif // CLIENT_UI_H
