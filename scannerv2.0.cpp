#include <iostream>
#include <SFML/Network.hpp>
#include <string>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

static bool port_is_open(const string & address, int port) {
	return(sf::TcpSocket().connect(address, port) == sf::Socket::Done);
}

string process(string const& s) {
    string::size_type pos = s.find('-');
    if (pos!= string::npos) {
        return s.substr(pos + 1, s.length());
    } else {
        return s;
    }
}

string getDate() {
    chrono::system_clock::time_point tp = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(tp);
    const char * tc = ctime(&t);
    string str = string {tc};
    str.pop_back();
    return str;
}

void USAGE(void);
void ShowPortNumberInfo(int port);

int main(int argc, char* argv[]) {
	if(argc < 2) {
		USAGE();
		return 0;
	}
		
	int z = 0;
	int portsDefault[] = {20, 21, 22, 23, 25, 53, 80, 110, 119, 123, 143, 161, 194, 443, 3306, 8080};
	int n = sizeof(portsDefault) / sizeof(portsDefault[0]);
	int portStart = 0;
	int portEnd = 0;
	string ip;
	string ports;
	
	auto start = high_resolution_clock::now();

	for(int i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			if(argv[i][1] == 'h') {
				ip = argv[i + 1];
			} else if(argv[i][1] == 'p') {
				portStart = atoi(argv[i + 1]);
				ports = process(argv[i + 1]);
				portEnd = stoi(ports);
			} else {
				USAGE();
				return 0;
			}
		} else if(argv[i][0] == '?') {
			USAGE();
			return 0;
		}
	}

	cout << "Starting uScan 1.0 at " << getDate() << " SE Asia Standard Time" << endl;
	cout << "uScan scan report for " << ip << endl;
	cout << "Host is up" << endl << endl;
	
	if(portStart == 0 & portEnd == 0) {
  		for(int i = 0; i < n; i++) {
			cout << "Port " << portsDefault[i] << " is ";

			if(port_is_open(ip, portsDefault[i])) {
				cout << "OPEN ";
				ShowPortNumberInfo(portsDefault[i]);
				cout << endl;
			} else {
				cout << "CLOSED" << endl;
			}
		}
  	} else {
 		for(int i = portStart; i <= portEnd; i++) {
			cout << "Port " << i << " is ";

			if(port_is_open(ip, i)) {
				cout << "OPEN ";
				ShowPortNumberInfo(i);
				cout << endl;
			} else {
				cout << "CLOSED" << endl;
			}
		}
  	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	cout << endl << "uScan done: 1 IP address (1 host up) scanned in " << duration.count() << " milliseconds";

	return 0;
}

void USAGE(void) {
	printf("How To Use: \n");
	printf(" scanner -h <host> -p [port, port range] -v.\n");
	printf(" \n");
	printf("Options: \n");
	printf(" -h <host>: the target host we want to check its ports. \n");
	printf(" -p [port, port range]: the ports that we want to check on the target host.\n");
	printf(" ?: show this help.\n");
	printf(" \n");
	printf("Example: \n");
	printf(" scanner -h site.com -p 80-500\n");
	printf(" Will scan from port 80 to port 500 on the site.com web site.\n");
	printf(" \n");
}

void ShowPortNumberInfo(int port) {
	switch(port) {
		case 	21:	printf("ftp"); break;
		case 	22:	printf("ssh"); break;
		case 	23:	printf("telnet"); break;
		case 	25:	printf("smtp"); break;
		case	53:	printf("dns"); break;
		case	80:	printf("http"); break;
		case	443:	printf("https"); break;
		case	445:	printf("microsoft-ds"); break;
		case	3306:	printf("mysql"); break;	

		default: printf("Unknown service"); break;
	}
}
