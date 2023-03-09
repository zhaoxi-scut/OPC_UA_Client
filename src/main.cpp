#include "connect_ui.h"
#include "client_ui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //!< OPC UA Client
    client_ptr ua_client = std::make_shared<UaClient>();
    //!< Client UI
    ClientUI client_ui(ua_client);
    //!< Connection UI
    ConnectUI connect_ui(ua_client);

    client_ui.bindConnect(&connect_ui);
    connect_ui.bindClient(&client_ui);

    connect_ui.show();

    return app.exec();
}
