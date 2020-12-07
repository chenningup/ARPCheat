
#include <QByteArray>
#include <QObject>
#include <QNetworkInterface> 
class PackInstallWorker:public QObject
{
public:
	PackInstallWorker();
	~PackInstallWorker();

	QByteArray createarppack(QString ethernetDstMac,QString ethernetSendMac,int type, QString dstMac, QString disIp, QString senderMac, QString senderIP);
	

	QString getHostIpAddress();

	QString getHostMacAddress();

	QString GetThreeIp(QString ip);

	void createcharip(QString ipstr, char *str);

	void createcharmac(QString macip, char *str);

	void  createherader(int type,char *str);

	bool isLinkLocalAddress(QHostAddress addr);
private:

};
