#include  "pcap.h"
#include <QObject>
#include <QThread>
#include "QString"
#include <QNetworkInterface>
#include <stdlib.h>
#include <QHostInfo>
#include <windows.h>
#pragma comment(lib, "wsock32.lib")

class data_packing :public QThread
{
	Q_OBJECT
	public:
		pcap_t *fp;
		pcap_if_t *alldevs;

		char My_mac[6];
		char Des_mac[6];
		char Des_Broad_mac[6];
		char ArpType[2];
		quint32 My_Int_type_Ip;
		char My_Char_type_Ip[4];
		char Goal_Ip[4];

		int  data_packing_type;//


		QByteArray Goal_Arp_Reply;
		QByteArray My_Ping_Data;
		QByteArray Goal_Ping_Rely;

		QString tishi;
		void MakeArpData();
		QByteArray MakePingData();
		QHash<int, QByteArray>My_Sending_data;

		
		void ReadyForArpSeachOne();
		void ReadyForArpSeachAll();
		void ReadyForArpAttack();
		void ReadyForArpDisguise();
		void ReadyForPingOne();
		void ReadyForPingAll();
		int SendData();
		int geton_off();
		void seton_off(int i);
		virtual void run() override;

		

		data_packing(int type);
	    ~data_packing();
private:
	int on_or_off;
signals:
//void UpdateTips(QString);

};