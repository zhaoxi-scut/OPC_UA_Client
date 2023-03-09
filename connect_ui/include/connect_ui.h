#ifndef CONNECTUI_H
#define CONNECTUI_H

#include <QWidget>

#include "ua_client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ConnectUI; }
QT_END_NAMESPACE

class ConnectUI : public QWidget
{
    Q_OBJECT

    Ui::ConnectUI *__ui;
    client_ptr __ua_client = nullptr;
    QWidget *__client = nullptr;

public:
    ConnectUI(client_ptr ua_client, QWidget *parent = nullptr);
    ~ConnectUI();

    /**
     * @brief Bind client
     *
     * @param pClient Client pointer
     */
    inline void bindClient(QWidget *pClient) { __client = pClient; }

private:
    //! Initialize the UI
    void initUI();
    //! Initialize the signal and slots
    void initSignalSlots();
};
#endif // CONNECTUI_H
