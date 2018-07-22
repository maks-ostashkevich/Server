#include <iostream>
#include <string>
#include <unordered_map>

#include <random>

const long day = 1;
const long fiveDays = 5;
const long hundredMessages = 100;

std::string randomIP()
{
	std::random_device rndDevice;
	std::mt19937 eng(rndDevice());
	std::uniform_int_distribution<int> dist(0, 1000);

	std::string ip = "";
	for (int i = 0; i < 4; i++)
	{
		long num = dist(eng);
		if (i != 3)
			ip += std::to_string(num) + ".";
		else
			ip += std::to_string(dist(eng));
	}

	return ip;
}

class Client
{
public:
	Client(std::string ip) : ip(ip)
	{

	}

	std::string sendRequest() const
	{
		return ip;
	}
private:
	std::string ip;
};

class Server
{
public:
	void getRequest(std::string ip, long long time)
	{
		if (clients.find(ip) == clients.end())
		{
			clients.insert(std::make_pair(ip, std::make_pair(time, 1l)));
		}
		else
		{
			auto clientWithIP = clients.at(ip);
			if (time - clientWithIP.first >= fiveDays && clientWithIP.second >= hundredMessages) // to constant
			{
				blackList.insert(std::make_pair(ip, clientWithIP));
				clients.erase(ip);
			}
			else
			{
				auto newClientWithIP = std::make_pair(clientWithIP.first, clientWithIP.second + 1l);
				clients[ip] = newClientWithIP;
			}
		}
	}

	void clearBlackList()
	{
		blackList.clear();
	}

	void blackListIsEmpty()
	{
		std::cout << blackList.empty() << std::endl;
	}
private:
	std::unordered_map<std::string, std::pair<long long, long>> clients;
	std::unordered_map<std::string, std::pair<long long, long>> blackList;
};

void serverWork()
{
	Client cl1(randomIP());
	Client cl2(randomIP());
	Client cl3(randomIP());
	Client cl4(randomIP());

	Server srv;

	for (long long counter = 0; counter < 1'000'000'000'000; counter++)
	{
		std::cout << "counter: " << counter << std::endl;
		if (counter % (fiveDays) == 0)
		{
			srv.clearBlackList();
		}
		if (counter % day == 0)
		{
			srv.blackListIsEmpty();
			for (int i = 0; i < 101; i++)
				srv.getRequest(cl1.sendRequest(), counter);
			for (int i = 0; i < 101; i++)
				srv.getRequest(cl2.sendRequest(), counter);
			for (int i = 0; i < 101; i++)
				srv.getRequest(cl3.sendRequest(), counter);
			for (int i = 0; i < 101; i++)
				srv.getRequest(cl4.sendRequest(), counter);
		}
	}
}

int main()
{
	serverWork();
}