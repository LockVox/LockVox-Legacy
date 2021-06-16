/**
 * @file qsslserver.h
 * @brief An extension of QTcpServer to support ssl. The counterpart to QSslSocket.
 * @author Skycoder42
 * @link https://github.com/Skycoder42/QSslServer
 * @version 1.1.0 updated by Valentin Monnot
 * @date 2021
 * @copyright BSD-3-Clause License
 */

#ifndef QSSLSERVER_H
#define QSSLSERVER_H

#include <QTcpServer>
#include <QSslSocket>
#include <QSslConfiguration>

class QSslServer : public QTcpServer
{
    /**
     * @class QSslServer qsslserver.h "qsslserver.h"
     * @brief An extension of QTcpServer to support ssl. The counterpart to QSslSocket.
     * @author Skycoder42
     * @link https://github.com/Skycoder42/QSslServer
     * @version 1.1.0 updated by Valentin Monnot
     * @date 2021
     * @copyright BSD-3-Clause License
     */

    Q_OBJECT

    public:
        explicit QSslServer(QObject *parent = nullptr);

        QSslSocket *nextPendingConnection() override;

        // SSL configuration
        QSslConfiguration sslConfiguration() const;
        void setSslConfiguration(QSslConfiguration configuration);

        // Certificate
        void setLocalCertificateChain(const QList<QSslCertificate> &localChain);
        void setLocalCertificateChain(const QString &fileName, QSsl::EncodingFormat format = QSsl::Pem, QSslCertificate::PatternSyntax syntax = QSslCertificate::PatternSyntax::FixedString);
        QList<QSslCertificate> localCertificateChain() const;

        void setLocalCertificate(const QSslCertificate &certificate);
        void setLocalCertificate(const QString &fileName,QSsl::EncodingFormat format = QSsl::Pem);
        QSslCertificate localCertificate() const;

        // Private keys
        void setPrivateKey(const QSslKey &key);
        void setPrivateKey(const QString &fileName,QSsl::KeyAlgorithm algorithm = QSsl::Rsa,QSsl::EncodingFormat format = QSsl::Pem,const QByteArray &passPhrase = QByteArray{});
        QSslKey privateKey() const;

        // CA settings
        bool addCaCertificates(const QString &path,QSsl::EncodingFormat format = QSsl::Pem,QSslCertificate::PatternSyntax syntax = QSslCertificate::PatternSyntax::FixedString);
        void addCaCertificate(const QSslCertificate &certificate);
        void addCaCertificates(const QList<QSslCertificate> &certificates);

        // PKCS#12
        bool importPkcs12(const QString &path,const QByteArray &passPhrase = QByteArray(),bool replaceCaCerts = false);

        QSsl::SslProtocol protocol() const;
        void setProtocol(QSsl::SslProtocol protocol);

    protected:
        void incomingConnection(qintptr handle) override;

    private:
        QSslConfiguration _configuration;
};
#endif // QSSLSERVER_H
