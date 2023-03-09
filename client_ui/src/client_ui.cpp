#include <thread>

#include <QPainter>
#include <QFontDatabase>

#include "sys_tick.h"
#include "font_icon.h"
#include "client_ui.h"
#include "ui_client_ui.h"

ClientUI::ClientUI(client_ptr ua_client, QWidget *parent) :
    QWidget(parent), __ua_client(ua_client), ui(new Ui::ClientUI)
{
    ui->setupUi(this);
    __devices_idx.reserve(32);
    // Initialize the UI
    initUI();
    // Initialize Signal-slot
    initSignalSlots();
}

ClientUI::~ClientUI()
{
    delete ui;
}

void ClientUI::initUI()
{
    QFont font;
    int font_id = QFontDatabase::addApplicationFont(":/font/fontawesome-webfont.ttf");
    QStringList font_families = QFontDatabase::applicationFontFamilies(font_id);

    Q_ASSERT_X(font_families.size() == 1, "Font", "font is empty!");
    font.setFamily(font_families.front());
    font.setPointSize(11);

    ui->group_box_device->setFont(font);
    ui->group_box_device->setTitle(QString("%1%2").arg(icon(ICON_LIST)).arg("  设备选取 "));
    ui->radio_camera->setFont(font);
    ui->radio_camera->setText(QString("%1%2").arg("相机  ").arg(icon(ICON_CAMERA)));
    ui->radio_light->setFont(font);
    ui->radio_light->setText(QString("%1%2").arg("光源  ").arg(icon(ICON_LIGHTBULB_O)));
    ui->button_scan->setFont(font);
    ui->button_scan->setText(QString("%1%2").arg(icon(ICON_ROTATE_LEFT)).arg("  扫描"));
    ui->button_sub->setFont(font);
    ui->button_sub->setText(QString("%1%2").arg(icon(ICON_PAPER_PLANE_O)).arg("  订阅"));

    ui->group_box_status->setFont(font);
    ui->group_box_status->setTitle(QString("%1%2").arg(icon(ICON_COG)).arg("  设备属性 "));
    ui->label_device_index->setFont(font);
    ui->label_device_index->setText(QString("%1%2").arg("设备索引  ").arg(icon(ICON_PAPERCLIP)));
    ui->label_device_ip->setFont(font);
    ui->label_device_ip->setText(QString("%1%2").arg("网络地址  ").arg(icon(ICON_WIFI)));
    ui->label_device_name->setFont(font);
    ui->label_device_name->setText(QString("%1%2").arg("设备名称  ").arg(icon(ICON_TAG)));
    ui->label_status_message->setFont(font);
    ui->label_status_message->setText(QString("%1%2").arg("设备状态  ").arg(icon(ICON_INFO_CIRCLE)));

    ui->group_box_log->setFont(font);
    ui->group_box_log->setTitle(QString("%1%2").arg(icon(ICON_FILE_TEXT_O)).arg("  日志 "));
    ui->button_log_export->setFont(font);
    ui->button_log_export->setText(QString("%1%2").arg(icon(ICON_DOWNLOAD)).arg("  导出"));
    ui->button_log_clear->setFont(font);
    ui->button_log_clear->setText(QString("%1%2").arg(icon(ICON_TRASH_O)).arg("  清空"));

    ui->group_box_image->setFont(font);
    ui->group_box_image->setTitle(QString("%1%2").arg(icon(ICON_IMAGE)).arg("  图像采集 "));
    ui->button_image_load->setFont(font);
    ui->button_image_load->setText(QString("%1%2").arg(icon(ICON_UPLOAD)).arg("  手动采集"));
    ui->button_image_export->setFont(font);
    ui->button_image_export->setText(QString("%1%2").arg(icon(ICON_DOWNLOAD)).arg("  导出图像"));
    ui->button_image_clear->setFont(font);
    ui->button_image_clear->setText(QString("%1%2").arg(icon(ICON_TRASH_O)).arg("  清除显示"));

    ui->group_box_light->setFont(font);
    ui->group_box_light->setTitle(QString("%1%2").arg(icon(ICON_LIGHTBULB_O)).arg("  光源控制 "));
    font.setPointSize(9);
    ui->label_sub_msg->setFont(font);
    ui->label_scan_msg->setFont(font);

    font.setPointSize(11);
    ui->button_disconnect->setFont(font);
    ui->button_disconnect->setText(QString("%1%2").arg(icon(ICON_CHAIN_BROKEN)).arg("  断开连接"));
    ui->button_exit->setFont(font);
    ui->button_exit->setText(QString("%1%2").arg(icon(ICON_SIGN_OUT)).arg("  退出程序"));
}

void ClientUI::log(LogType log_type, const QString &str)
{
    QString log_status;
    switch (log_type)
    {
    case LogType::Success:
        log_status = "(succ)";
        break;
    case LogType::Info:
        log_status = "(info)";
        break;
    case LogType::Warning:
        log_status = "(warn)";
        break;
    case LogType::Error:
        log_status = "(erro)";
        break;
    default:
        break;
    }
    ui->browser_log->append(SysTick::getStr() + log_status);
    ui->browser_log->append(QString("-- ") + str);
}

void ClientUI::monitor()
{

}

void imageChange(UA_Client *client, UA_UInt32 subId, void *subContext, UA_UInt32 monId, void *monContext, UA_DataValue *value)
{

}

void ClientUI::initSignalSlots()
{
    //! 设备选取按钮
    connect(ui->radio_camera, &QRadioButton::clicked, this, [this]() { __device_type = DeviceType::CAMERA; });
    connect(ui->radio_light, &QRadioButton::clicked, this, [this]() { __device_type = DeviceType::LIGHT; });

    //! 扫描
    connect(ui->button_scan, &QPushButton::clicked, this,
            [&]()
    {
        __target_id = UA_NODEID_NULL;
        ua::Client &client = __ua_client->get();
        // Clear
        ui->combo_box_device->clear();
        __devices_idx.clear();
        // 获取扫描对象，以设置设备前缀 (相机->Camera, 光源控制器->LightController)
        if (__device_type == DeviceType::NONE)
        {
            ui->label_scan_msg->setStyleSheet("color: rgb(255, 0, 0)");
            ui->label_scan_msg->setText(QString("%1%2").arg(icon(ICON_WARNING)).arg(" 未选择类型"));
            return;
        }
        std::string device_prefix = __device_type == DeviceType::CAMERA ? "Camera" : "LightController";
        // 逐一 findNode 添加至设备列表，上限 32
        for (int i = 0; i < 32; ++i)
        {
            std::string device_str = device_prefix + "[" + std::to_string(i) + "]";
            UA_NodeId device_id = client.findNodeId(__ua_client->getServerID(), 1, device_str);
            if (!UA_NodeId_isNull(&device_id))
                __devices_idx.emplace_back(device_str.c_str());
        }
        //
        for (const auto &idx : __devices_idx)
            ui->combo_box_device->addItem(idx);
        ui->combo_box_device->setCurrentIndex(-1);
        ui->label_scan_msg->setStyleSheet("color: rgb(0, 180, 0)");
        ui->label_scan_msg->setText(icon(ICON_CHECK));
    });

    //! 订阅按钮
    connect(ui->button_sub, &QPushButton::clicked, this,
            [&]()
    {
        // 初始化设置
        ua::Client &client = __ua_client->get();
        ui->label_sub_msg->setStyleSheet("color: rgb(255, 0, 0)");
        if (__device_type == DeviceType::NONE)
        {
            ui->label_sub_msg->setText(QString("%1%2").arg(icon(ICON_WARNING)).arg(" 未选择类型"));
            return;
        }
        std::string target_str = ui->combo_box_device->currentText().toStdString();
        if (target_str.empty())
        {
            ui->label_sub_msg->setText(QString("%1%2").arg(icon(ICON_WARNING)).arg(" 未选择设备"));
            return;
        }
        ui->label_sub_msg->setStyleSheet("color: rgb(0, 180, 0)");
        ui->label_sub_msg->setText(icon(ICON_CHECK));
        // 更新 Enable / Disable
        if (__device_type == DeviceType::CAMERA)
        {
            ui->group_box_image->setEnabled(true);
            ui->group_box_light->setDisabled(true);
        }
        else
        {
            ui->group_box_image->setDisabled(true);
            ui->group_box_light->setEnabled(true);
        }
        // 设置目标订阅节点 ID
        UA_NodeId target_id = client.findNodeId(__ua_client->getServerID(), 1, target_str);
        if (UA_NodeId_equal(&target_id, &__target_id))
            return;
        __target_id = target_id;
        // 根据设备类型执行对应变量的订阅请求
        if (__ua_client->getSubID() == 0U)
        {
            __ua_client->createSubscription();
            if (__ua_client->getSubID() == 0U)
            {
                log(LogType::Error, "Failed to create subscription.");
                return;
            }
            log(LogType::Success, "Subscribe successfully.");
        }
        ui->edit_device_index->setText();
        if (__device_type == DeviceType::CAMERA) // DeviceType::CAMERA
        {
            UA_NodeId image_id = client.findNodeId(__target_id, 1, "Image");
            if (!client.createVariableMonitor(__ua_client->getSubID(), image_id, imageChange))
            {
                log(LogType::Error, "Failed to create monitor.");
                return;
            }
            const auto &[exposure, gain, r_gain, g_gain, b_gain] = __ua_client->readCameraVariable(__target_id);
            ui->widget_exposure->setExposure(exposure);
            ui->widget_gain->setGain(gain);
            ui->widget_r_gain->setGain(r_gain);
            ui->widget_g_gain->setGain(g_gain);
            ui->widget_b_gain->setGain(b_gain);
        }
        else // DeviceType::LIGHT
        {
            const auto &[luminance, delay] = __ua_client->readLightControllerVariable(__target_id);
            ui->widget_light_ch1->setValue(luminance[0]);
            ui->widget_light_ch2->setValue(luminance[1]);
            ui->widget_light_ch3->setValue(luminance[2]);
            ui->widget_light_ch4->setValue(luminance[3]);
            ui->widget_delay_ch1->setValue(delay[0]);
            ui->widget_delay_ch2->setValue(delay[1]);
            ui->widget_delay_ch3->setValue(delay[2]);
            ui->widget_delay_ch4->setValue(delay[3]);
        }
    });

    //! 清空图像
    connect(ui->button_image_clear, &QPushButton::clicked, this,
            [&]()
    {
        ui->label_image->clear();
        ui->label_image->setText("No Image");
    });

    //! 导出图像
    connect(ui->button_image_export, &QPushButton::clicked, this,
            [&]()
    {
        if (!ui->label_image->pixmap().isNull())
        {
            qDebug("label_image->pixmap() isn't null");
        }
    });

    //! 断开连接
    connect(ui->button_disconnect, &QPushButton::clicked, this,
            [&]()
    {
        if (__connect != nullptr)
        {
            __devices_idx.clear();
            __device_type = DeviceType::NONE;
            ui->label_scan_msg->clear();
            ui->label_sub_msg->clear();
            ui->combo_box_device->clear();
            __ua_client->setServerID(UA_NODEID_NULL);
            __ua_client->get().disconnect();
            this->close();
            __connect->show();
        }
    });

    //! 滑动条触发服务器数据更新
    // SetExposure
    connect(ui->widget_exposure, &ExposureSpinSlider::valueChanged, this,
            [this](uint16_t val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetExposure");
        std::vector<ua::Variable> output;
        client.call(method_id, {val}, output, __target_id);
    });
    // SetGain
    connect(ui->widget_gain, &GainSpinSlider::valueChanged, this,
            [this](double val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetGain");
        std::vector<ua::Variable> output;
        client.call(method_id, {val}, output, __target_id);
    });
    // SetRedGain
    connect(ui->widget_r_gain, &GainSpinSlider::valueChanged, this,
            [this](double val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetRedGain");
        std::vector<ua::Variable> output;
        client.call(method_id, {val}, output, __target_id);
    });
    // SetGreenGain
    connect(ui->widget_g_gain, &GainSpinSlider::valueChanged, this,
            [this](double val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetGreenGain");
        std::vector<ua::Variable> output;
        client.call(method_id, {val}, output, __target_id);
    });
    // SetBlueGain
    connect(ui->widget_b_gain, &GainSpinSlider::valueChanged, this,
            [this](double val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetBlueGain");
        std::vector<ua::Variable> output;
        client.call(method_id, {val}, output, __target_id);
    });
    // SetLuminance
    connect(ui->widget_light_ch1, &LightSpinSlider::valueChanged, this,
            [this](uint8_t val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetLuminance");
        std::vector<ua::Variable> output;
        client.call(method_id, {0U, val}, output, __target_id);
    });
    connect(ui->widget_light_ch2, &LightSpinSlider::valueChanged, this,
            [this](uint8_t val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetLuminance");
        std::vector<ua::Variable> output;
        client.call(method_id, {1U, val}, output, __target_id);
    });
    connect(ui->widget_light_ch3, &LightSpinSlider::valueChanged, this,
            [this](uint8_t val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetLuminance");
        std::vector<ua::Variable> output;
        client.call(method_id, {2U, val}, output, __target_id);
    });
    connect(ui->widget_light_ch4, &LightSpinSlider::valueChanged, this,
            [this](uint8_t val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetLuminance");
        std::vector<ua::Variable> output;
        client.call(method_id, {3U, val}, output, __target_id);
    });
    // SetDelay
    connect(ui->widget_delay_ch1, &DelaySpinSlider::valueChanged, this,
            [this](uint16_t val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetDelay");
        std::vector<ua::Variable> output;
        client.call(method_id, {0U, val}, output, __target_id);
    });
    connect(ui->widget_delay_ch2, &DelaySpinSlider::valueChanged, this,
            [this](uint16_t val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetDelay");
        std::vector<ua::Variable> output;
        client.call(method_id, {1U, val}, output, __target_id);
    });
    connect(ui->widget_delay_ch3, &DelaySpinSlider::valueChanged, this,
            [this](uint16_t val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetDelay");
        std::vector<ua::Variable> output;
        client.call(method_id, {2U, val}, output, __target_id);
    });
    connect(ui->widget_delay_ch4, &DelaySpinSlider::valueChanged, this,
            [this](uint16_t val)
    {
        ua::Client &client = __ua_client->get();
        UA_NodeId method_id = client.findNodeId(__target_id, 1, "SetDelay");
        std::vector<ua::Variable> output;
        client.call(method_id, {3U, val}, output, __target_id);
    });
}
