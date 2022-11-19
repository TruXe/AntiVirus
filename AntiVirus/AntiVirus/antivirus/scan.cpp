#include "include.h"
#include "scan.h"
#include "md5.h"

OPENFILENAME ofn;
DWORD error_value;

bool openFileDialog(TCHAR szFileName[])
{
	const TCHAR* FilterSpec = "All Files(.)\0*.*\0";
	const TCHAR* Title = "Open";

	const TCHAR* myDir = "C:\\c_plus_plus_trial";

	TCHAR szFileTitle[MAX_PATH] = { '\0' };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	*szFileName = 0;

	/* fill in non-variant fields of OPENFILENAME struct. */
	ofn.lStructSize = sizeof(OPENFILENAME);

	ofn.hwndOwner = GetFocus();
	ofn.lpstrFilter = FilterSpec;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = myDir;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrTitle = Title;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	// false if failed or cancelled
	if (GetOpenFileName(&ofn) == 1)
	{

		MessageBox(NULL, "Opening filepath", "SUCCESS !!!", MB_OK);

		return 1;
	}
	else
	{

		MessageBox(NULL, "error", "FAILURE !!!", MB_OK);
		return 0;
	}
}

std::string scan()
{
	std::string infected = "infected";
	std::string clean = "clean";
	TCHAR openFileName[MAX_PATH];
	openFileDialog(openFileName);

	//path to hashcodes.txt to compare if the file is infected or not with malware for more md5 hashes visit virusshare.com
	std::ifstream hash("D:\\Visual Studio\\C++\\AntiVirus\\AntiVirus\\hashcodes.txt");
	std::string line;
	std::string scan = "Scanning";
	std::string done = "Done";
	std::ofstream file;
	std::string result = "result.txt";
	bool found = false;
	while (std::getline(hash, line) && !found)
	{
		if (line.find(md5(openFileName)) != std::string::npos)
		{
			found = true;
		}
	}
	if (found)
	{
		file.open(result.c_str(), std::ios_base::app);
		file << openFileName << " " << md5(openFileName) << " Infected " << "\n";
		file.close();
		return "Infected";
	}
	else
	{
		file.open(result.c_str(), std::ios_base::app);
		file << openFileName << " " << md5(openFileName) << " Clean " << "\n";
		file.close();
		return "Clean";
	}
	return "NULL";

}
void fastScan()
{

}
void scanAll()
{
	std::string scan = "Scanning";
	std::string done = "Done";
	std::string infected = "infected";
	std::string clean = "clean";
	std::ofstream file;
	std::set<std::string> ignoredirs = { "system32","Program Files","Program Files (x86)","$Recycle.Bin","Windows","AppData","ProgramData" };
	std::string path = "C:\\";
	for (std::filesystem::recursive_directory_iterator entry(path), end; entry != end; ++entry)
	{
		if (ignoredirs.find(entry->path().filename().string()) != ignoredirs.end())
		{
			entry.disable_recursion_pending();
		}
		else if (!std::filesystem::is_directory(entry->path()) && entry->path().has_extension())
		{
			if (entry->path().extension() == ".exe")
			{
				std::ifstream hash("D:\\Visual Studio\\C++\\AntiVirus\\AntiVirus\\hashcodes.txt");
				std::string result = "result.txt";
				std::string line;
				bool found = false;
				while (std::getline(hash, line) && !found)
				{
					if (line.find(md5(entry->path().string())) != std::string::npos)
					{
						found = true;
					}
				}
				if (found)
				{
					file.open(result.c_str(), std::ios_base::app);
					file << entry->path().string().c_str() << " " << md5(entry->path().string()) << " Infected " << "\n";
					file.close();

				}
				else
				{
					file.open(result.c_str(), std::ios_base::app);
					file << entry->path().string().c_str() << " " << md5(entry->path().string()) << " Clean " << "\n";
					file.close();

				}
			}
		}


	}
}