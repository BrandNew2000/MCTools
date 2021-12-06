// TODO: EssentialsX

#include<iostream>
#include<fstream>
#include <windows.h>
#include <filesystem>
#pragma comment(lib, "Urlmon.lib")


using namespace std;

string f1 = "paper.jar";
string f2 = "server.config";
string f3 = "paper.yml";
string f4 = "spigot.yml";
string mem = "2G";

// Checks if a file exists. Returns true or false.
bool exist(string file)
{
	ifstream ifile;
	ifile.open(file);
	if (ifile)
		return true;
	else
		return false;
}

// File writing. Taken from "https://stackoverflow.com/a/39462191".
static void appendLineToFile(string filepath, string line)
{
	std::ofstream file;
	//can't enable exception now because of gcc bug that raises ios_base::failure with useless message
	//file.exceptions(file.exceptions() | std::ios::failbit);
	file.open(filepath, std::ios::out | std::ios::app);
	if (file.fail())
		throw std::ios_base::failure(std::strerror(errno));

	//make sure write fails with exception if something is wrong
	file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);

	file << line << std::endl;
}

// Downloads a file.
int dwnld(string url, string file)
{

	LPSTR srcURL = const_cast<char*>(url.c_str());
	LPSTR destFile = const_cast<char*>(file.c_str());

	cout << "\n";
	// URLDownloadToFile returns S_OK on success 
	if (S_OK == URLDownloadToFile(NULL, srcURL, destFile, 0, NULL))
	{

		cout<<"Saved to ";
		cout<<file;

		return 0;

	}

	else
	{

		cout<<"Failed";

		return -1;

	}

}

// Download subroutine for Paperclip and EssentialsX.
void download() 
{
	string data;
	size_t x = 0;
	size_t found = 0;
	std::filesystem::create_directory("MCTools");
	cout << "Downloading......."; 
	if (dwnld("https://papermc.io/api/v2/projects/paper/versions/1.18/builds/54/downloads/paper-1.18-54.jar", "paper.jar") == -1)
	{
		cout << "\nDownload Failed. Check your internet connection and try again.";
		exit(0);
	}
	/* ifstream infile;
	infile.open("MCTools\\paperver.txt");
	infile >> data;
	while (found != string::npos)
	{
		found = data.find(',', x + 1);
		if (found != string::npos)
			x = found;
	}
	*/
	// if (dwnld("https://ci.ender.zone/job/EssentialsX/lastSuccessfulBuild/artifact/jars/*zip*/jars.zip", "MCTools\\essentialsx.zip") == -1)
	// {
	//		cout << "\nDownload Failed. Check your internet connection and try again.";
	//		exit(0);
	// }
}

// Checks if four predefined files (f1, f2, f3, f4) exist. Returns false if no and true otherwise.
bool corefiles()
{
	if (exist(f1) == false && exist(f2) == false && exist(f3) == false && exist(f4) == false)
		return false;
	else
		return true;
}

// Depending on the corefiles() value (runs if true), it starts the first time wizard, downloads the
// neccesary files using download(), and echos "eula=true" after a prompt from the user. 
void firstsetup()
{
	char x;
	if (corefiles() == false)
	{
		cout << "\nNo files detected. First Setup initialized.";
		cout << "\n\nIt seems you have started MCTools for the first time.";
		cout << "\nMCTools will now download Paperclip and Essentials and set everything up.\n";
		download();
		cout << "\nDo you agree to Minecraft's EULA (y/n): ";
		cin >> x;
		if (x == 'y') appendLineToFile("eula.txt", "eula=true");
		else exit(0);
	}
}

// Starts the server with a predefined amount of memory (mem). Used in conjunction with listop().
void start()
{
	string send = "java -Xmx" + mem + " -jar paper.jar --nogui";
	if (exist(f1) == true)
		cout << "Paper exits";
	else
	{
		cout << "Paper does not exist. Run update first.\nStopping...\n\n";
		exit(0);
	}
	cout << "\nStarting Server.....\n\n";
	system(send.c_str());
}

// Calls download() and adds a few messages. Used in conjunction with listop().
void update()
{
	cout << "Update initialized\n";
	download();
	cout << "\nUpdate completed\n";
}

// Displays information about the options in listop() and other helpful information.
void help()
{
	cout << "Help stuff here";
}

// Displays a list of options to start or update the server and a help file.
void listop()
{
	int x;
	cout << "Enter option: \n1) Start Server \n2) Update Server \n[Enter option number]: ";
	cin >> x;
	cout << "\n";
	switch (x)
	{
	case 1: start();
		break;
	case 2: update();
		break;
	case 3: help();
		break;

	default: cout << "Invalid input";
	}
}

// The main class (obviously).
int main()
{
	
	firstsetup();
	listop();
	return 0;
}