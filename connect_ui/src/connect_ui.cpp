#include <QFontDatabase>

#include "connect_ui.h"
#include "font_icon.h"

#include "./ui_connect_ui.h"

ConnectUI::ConnectUI(client_ptr ua_client, QWidget *parent)
    : QWidget(parent), __ua_client(ua_client), __ui(new Ui::ConnectUI)
{
    __ui->setupUi(this);

    initUI();
    initSignalSlots();
}

ConnectUI::~ConnectUI()
{
    delete __ui;
}

void ConnectUI::initUI()
{
    QFont font;
    int font_id = QFontDatabase::addApplicationFont(":/font/fontawesome-webfont.ttf");
    QStringList font_families = QFontDatabase::applicationFontFamilies(font_id);

    Q_ASSERT_X(font_families.size() == 1, "Font", "font is empty!");
    font.setFamily(font_families.front());

    font.setPointSize(11);
    __ui->label_ip->setFont(font);
    __ui->label_ip->setText(QString("%1%2").arg(icon(ICON_SITEMAP)).arg("  IP地址"));
    __ui->label_port->setFont(font);
    __ui->label_port->setText(QString("%1%2").arg(icon(ICON_DESKTOP)).arg("  端口号"));
    font.setPointSize(9);
    __ui->label_msg->setFont(font);

    font.setPointSize(12);
    __ui->button_clear->setFont(font);
    __ui->button_clear->setText(QString("%1%2").arg(icon(ICON_TRASH_O)).arg("  清 空"));
    __ui->button_connect->setFont(font);
    __ui->button_connect->setText(QString("%1%2").arg(icon(ICON_CHAIN)).arg("  连 接"));
}

void ConnectUI::initSignalSlots()
{
    connect(__ui->button_connect, &QPushButton::clicked, this,
            [&]()
            {
                if (__client != nullptr)
                {
                    // Connect
                    std::string edit_ip_txt = __ui->edit_ip->placeholderText().toStdString();
                    if (!__ui->edit_ip->text().isEmpty())
                        edit_ip_txt = __ui->edit_ip->text().toStdString();
                    std::string edit_port_txt = __ui->edit_port->placeholderText().toStdString();
                    if (!__ui->edit_port->text().isEmpty())
                        edit_port_txt = __ui->edit_port->text().toStdString();
                    std::string address = edit_ip_txt + ':' + edit_port_txt;

                    if (__ua_client->get().connect(address))
                    {
                        UA_NodeId server_id = __ua_client->get().findNodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                                                            1, "VisionServer");
                        __ua_client->setServerID(server_id);
                        __ui->label_msg->clear();
                        this->close();
                        __client->show();
                    }
                    else
                    {
                        __ui->label_msg->setStyleSheet("color: rgb(255, 0, 0)");
                        __ui->label_msg->setText(QString("%1%2").arg(icon(ICON_WARNING)).arg(" 连接失败"));
                        qWarning("Bad connection!");
                    }
                }
                else
                   qWarning("The client object is nullptr!");
            });
}

