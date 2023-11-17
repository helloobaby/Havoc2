#ifndef HAVOC_CONNECTOR_HPP
#define HAVOC_CONNECTOR_HPP

#include <global.hpp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>
#include <QAbstractSocket>

#include <Havoc/Packager.hpp>

namespace HavocNamespace
{
    class Connector : public QTcpSocket
    {
    private:
        QWebSocket*           Socket     = nullptr;
        Util::ConnectionInfo* Teamserver = nullptr;
        HavocSpace::Packager* Packager   = nullptr;
        HavocNamespace::HavocSpace::DBManager* dbManager = nullptr;

    public:
        QString ErrorString = nullptr;

        Connector( Util::ConnectionInfo* );
        ~Connector() noexcept;

        bool Disconnect();

        void SendLogin();
        void SendPackage( Util::Packager::PPackage package );

        void PassDB(HavocNamespace::HavocSpace::DBManager* dbManager);
        HavocNamespace::HavocSpace::DBManager* GetDb();
    };
}

#endif //HAVOC_CONNECTOR_HPP
