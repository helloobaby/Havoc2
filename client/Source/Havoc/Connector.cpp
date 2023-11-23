#include <Havoc/Connector.hpp>
#include <Havoc/Havoc.hpp>
#include <QCryptographicHash>
#include <QMap>
#include <QBuffer>
#include <QTimer>

Connector::Connector( Util::ConnectionInfo* ConnectionInfo )
{
    Teamserver   = ConnectionInfo;
    Socket       = new QWebSocket();
    auto Server =
        "wss://" + Teamserver->Host + ":" + this->Teamserver->Port + "/havoc/";
    auto SslConf = Socket->sslConfiguration();

    /* ignore annoying SSL errors */
    SslConf.setPeerVerifyMode( QSslSocket::VerifyNone );
    Socket->setSslConfiguration( SslConf );
    Socket->ignoreSslErrors();
    
    #ifdef _MSC_VER
    Socket->setProxy(QNetworkProxy::NoProxy);
    #endif

    QObject::connect( Socket, &QWebSocket::binaryMessageReceived, this, [&]( const QByteArray& Message )
    {
        auto Package = HavocSpace::Packager::DecodePackage( Message );

        if ( Package != nullptr )
        {
            if ( ! Packager )
                return;

            Packager->DispatchPackage( Package );

            return;
        }

        spdlog::critical( "Got Invalid json" );
    } );

    QObject::connect(Socket, &QWebSocket::connected, this, [&]() {
      this->Packager = new HavocSpace::Packager;
      this->Packager->setTeamserver(this->Teamserver->Name);
      auto dbMangager = this->GetDb();

      // 登录成功,修改数据库中对应的TeamSever的时间戳
      auto query = QSqlQuery();

      bool success = true;
      success |= query.prepare(
          "update Teamservers set TimeStamp=:TimeStamp where "
          "ProfileName=:ProfileName");
      query.bindValue(":TimeStamp", QDateTime::currentMSecsSinceEpoch());

      query.bindValue(":ProfileName",
                      this->Teamserver->Name.toStdString().c_str());

      if (!(success |= query.exec())) {
        spdlog::error("[DB] Failed to update profile timestamp {}",
                      query.lastError().text().toStdString());
      }

      SendLogin();
    });

    // ip不符合正常格式
    QObject::connect( Socket, &QWebSocket::disconnected, this, [&]()
    {
        MessageBox( "Teamserver error", Socket->errorString(), QMessageBox::Critical );

        Socket->close();

        Havoc::Exit();
    } );

    // Windows上构建的时候没有Qt版本对应的Openssl就会报这个错误
      QObject::connect(
        Socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
        [=](QAbstractSocket::SocketError error) { 
             MessageBox("Teamserver error", "Connect error",QMessageBox::Critical);
        }
      );
    

    Socket->open( QUrl( Server ) );
}

bool Connector::Disconnect()
{
    if ( this->Socket != nullptr )
    {
        this->Socket->disconnect();
        return true;
    }

    return false;
}

Connector::~Connector() noexcept
{
    delete this->Socket;
}

void Connector::SendLogin()
{
    Util::Packager::Package Package;

    Util::Packager::Head_t Head;
    Util::Packager::Body_t Body;

    Head.Event              = Util::Packager::InitConnection::Type;
    Head.User               = this->Teamserver->User.toStdString();
    Head.Time               = QTime::currentTime().toString( "hh:mm:ss" ).toStdString();

    Body.SubEvent           = Util::Packager::InitConnection::Login;
    Body.Info[ "User" ]     = this->Teamserver->User.toStdString();
    Body.Info[ "Password" ] = QCryptographicHash::hash( this->Teamserver->Password.toLocal8Bit(), QCryptographicHash::Sha3_256 ).toHex().toStdString();

    Package.Head = Head;
    Package.Body = Body;

    SendPackage( &Package );
}

void Connector::SendPackage( Util::Packager::PPackage Package )
{
    Socket->sendBinaryMessage( Packager->EncodePackage( *Package ).toJson( QJsonDocument::Compact ) );
}


void Connector::PassDB(HavocNamespace::HavocSpace::DBManager* dbManager) {

    this->dbManager = dbManager;
}

HavocNamespace::HavocSpace::DBManager* Connector::GetDb() {
  if (!this->dbManager) {
    assert(0 && "Used befor init");
  }
  return this->dbManager;
}