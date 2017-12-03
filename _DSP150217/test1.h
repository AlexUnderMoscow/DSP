#define PLATFORM_WINDOWS  1
#define PLATFORM_UNIX  2
 
#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#else
#define PLATFORM PLATFOR_UNIX
#endif

#if PLATFORM==PLATFORM_WINDOWS
#include <stdio.h>
#include <string.h>

#include <Windows.h>
#include "winsock.h"
//#pragma comment(lib,"libws2_32.a")
#pragma
#define HAVE_REMOTE
#include "pcap.h"
#include <iostream>
#include <conio.h>

#else

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <iostream>
//#include <errno.h>
//#include <time.h>
//#include <sys/un.h>
//#include <unistd.h>
#endif
using namespace std;
#ifndef TEST1_H
#define TEST1_H
#include <QString>
#include <QComboBox>
#include <QWidget>
#include "ui_test1.h"
#include "interface.h"
#include "struct.h"

//#define block 1024;

class test1Work
{
public:
    long long dataCounter;
    long long numPackets;
    unsigned int numOfLostPacket;
    unsigned short nextPackNumber;
    Interface *inf;

	test1Work(void* _inf, Ui::test1 *_ui)
	{
		inf = (Interface*)_inf;
		inf->ready = true;
		ui = _ui;
		dataCounter = 0;			
		overflow = false;
		nextPackNumber = 0;
		numOfLostPacket = 0;
		numPackets=0;
		ui->port->setRange(1,65535);
		ui->port->setValue(5001);
		numPackets =0;
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, nullptr, &alldevs, errbuf) == -1)
        {
            //fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
            return;
        }
        index = 0;
        ui->interfaces->clear();
        for(d=alldevs; d; d=d->next)
            {
                //printf("%d. %s", ++i, d->name);
            QString s(d->description);
            QString y = "'";
            int posStart = s.indexOf(y,0);
            int posEnd = s.indexOf(y,posStart+1);
            s=s.mid(posStart+1,posEnd-posStart-1);
            ui->interfaces->addItem(s);
            ui->interfaces->setCurrentIndex(index);
             ++index;
                //if (d->description)
                    //printf(" (%s)\n", d->description);
               // else
                   // printf(" (No description available)\n");
            }
            index--;

            if(index==0)
            {
                inf->ready = true;
                return;
            }

#if PLATFORM==PLATFORM_WINDOWS
#endif
	}

	~test1Work()
	{
#if PLATFORM==PLATFORM_WINDOWS

#else

#endif
	}
	void Do()
	{
		
#if PLATFORM==PLATFORM_WINDOWS

#endif
        int sz =0;
        u_short sport,dport;
        ip_header *ih;
        udp_header *uh;
        char* datatosend;
        u_int ip_len;
        while((res = pcap_next_ex( adhandle, &header, &pkt_data)) >= 0)
          {
            if(res == 0)
             /* Timeout elapsed */
             continue;
            if (inf->output.size()==0)
            {
                usleep(1);
                return;
            }
            ih = (ip_header *) (pkt_data + 14); //length of ethernet header
            if (ih->proto!=17)
            {
                return; //не UDP протокол
            }
            ip_len = (ih->ver_ihl & 0xf) * 4;
            uh = (udp_header *) ((u_char*)ih + ip_len);

            /* convert from network byte order to host byte order */
            sport = ntohs(uh->sport);
            dport = ntohs(uh->dport);
            if (dport!=tmpPort)
            {
                return;
            }
            datatosend = ((char*)(uh))+8; //10
            unsigned short udplen;
            unsigned short packNumber;

            char* l = (char*)&packNumber;
            //*l = *(((char*)(uh))+8);
            //*(l+1) = *(((char*)(uh))+8+1);
            if (packNumber!=nextPackNumber)
              {
                //numOfLostPacket+=packNumber-nextPackNumber;
              }
            else
              {
                nextPackNumber = packNumber+1;
              }


            l = (char*)&udplen;
            *l = *((char*)(&uh->len)+1);
            *(l+1) = *((char*)(&uh->len));

            if (udplen>1500)
            {
                return;
            }
            sz = inf->output.at(0)->write((char*)datatosend,1024); //
            if (sz==1024)
            {
                dataCounter+=1024;
                numPackets++;
            }
            else
            {
                if (inf->output.at(0)->halt) return;
                usleep(1);   //при перегрузке выходного буфера сон
            }

        }
        usleep(1);
		
	}
	void init()
	{
        numPackets =0;
		inf->ready = true;
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, nullptr, &alldevs, errbuf) == -1)
        {
            //fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
            return;
        }
            int inum = ui->interfaces->currentIndex()+1;
            if(inum < 1 || inum > index+1)
                {
                    //printf("\nInterface number out of range.\n");
                    /* Free the device list */
                    pcap_freealldevs(alldevs);
                    return;
                }
            for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);

               /* Open the device */
               if ( (adhandle= pcap_open(d->name,          // name of the device
                                         65536,            // portion of the packet to capture.
                                                           // 65536 guarantees that the whole packet will be captured on all the link layers
                                         PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
                                         1000,             // read timeout
                                         nullptr,             // authentication on the remote machine
                                         errbuf            // error buffer
                                         ) ) == nullptr)
               {
                  // fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
                   /* Free the device list */
                   pcap_freealldevs(alldevs);
                   return;
               }
               /* At this point, we don't need any more the device list. Free it */
                pcap_freealldevs(alldevs);
		QString s;
        tmpPort = (unsigned short)ui->port->value();
	}
private:

    pcap_if_t *alldevs;
    pcap_if_t *d;
    int inum;
    int i=0;
    pcap_t *adhandle;
    int res;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr *header;
    const u_char *pkt_data;
    unsigned short tmpPort;
    int index;
    bool overflow;
    Ui::test1 *ui;
};

class test1 : public Interface
{
	Q_OBJECT

public:
    test1(QDialog *parent,marray** _arr, unsigned int *_size);
	~test1();
	test1Work * work;

private:
    QTimer* timer;
	Ui::test1 ui;
private slots:
    void update();
};

#endif // TEST1_H
