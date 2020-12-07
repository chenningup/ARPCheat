#ifndef MY_TOOL_H
#define MY_TOOL_H

#include <QtWidgets/QMainWindow>
#include "ui_my_tool.h"
#include  "pcap.h"
#include "QHash"
//#include <cv.h>  
//#include <highgui.h>  
#include "packinstallworker.h"
#include "Module/AnalysisManager.h"
#include "Module/PackIngManager.h"
enum WangKaStatus
{
	unopen=0,
	opening,
	stop,
};
class my_tool : public QMainWindow
{
	Q_OBJECT
public:
	my_tool(QWidget *parent = 0);
	~my_tool();

	static my_tool* instance();

	void init();

	void setsolt();

	static void receiveData(QByteArray &data,void *pr);

	static void showarpdev(void *pr, void *data);

	void addItemContent(int row, int column, QString content);

	void contextMenuEvent(QContextMenuEvent *event);//ÓÒ¼üÉ¾³ý

	QString getWangKaGateway(QString ip);

	QMenu*													mMenu;

	QAction*												mStopNetAction;

	QAction*												mMonitorAction;

	QAction*												mStopAction;

	QAction*												mIcmpAction;

	QHash<QString, deviceInfo*>				devInfoHash;

	PackInstallWorker	*								mPackInstallWorker;

	WangKaStatus										mCurrentStatus;

	QMutex													tableMutex;

	QMutex													devInfoMutex;
private:
	Ui::my_toolClass									ui;

public slots:      
//void sniff();
void ShowCamera();

//void FinishSniff();
void on_sniffStartButton_clicked();

void on_OpenWangKaButton_clicked();

void on_cutOutPicButton_clicked();
//void UpdatingTips(QString);

void Getdata(QString data);

void ifprint(pcap_if_t *d);

char* ip6tos(struct sockaddr *sockaddr, char *address, int addrlen);

char *iptos(u_long in);

void stopNetActionSlot();

void mOnitorActionSlot();

void mStopActionSlot();

void mIcmpActionSlot();
};

#endif // MY_TOOL_H
