#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

// What is added after the folder name to designate it as this programs SORTING folder
#define FILE_POST "_AUTOFILESORT"

// Returns type of file given it's path, if its a folder, it returns the string "FOLDER"
std::string GetType(std::string path)
{
    if(fs::path(path).has_extension())
        return fs::path(path).extension().string();
    return "FOLDER";
}

int main()
{
    // replace the path folder with whatever directory you want to sort
    std::string path = "C:\\Users\\okay5\\OneDrive\\Desktop\\";
    int path_length = path.length();
    std::vector<std::string> extensions;
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(path))
    {
        auto& path = entry.path();
        std::string p = path.string();
        files.push_back(p);
        std::string file = p.substr(path_length);
        std::string ext = path.extension().string();

        // Dont include the file if its already part of autosort
        if (file.find(FILE_POST) != std::string::npos)
            continue;

        // New extenstion/type, add it!
        if (path.has_extension() && std::find(extensions.begin(), extensions.end(), ext) == extensions.end())
            extensions.push_back(path.extension().string());
        if (!path.has_extension() && std::find(extensions.begin(), extensions.end(), "FOLDER") == extensions.end())
            extensions.push_back("FOLDER");
    }
    // alr, now we have a list of files, and extensions, lets sort CAREFULLY,
    // otherwise, just sort as usual

    // SORT IT OUT
    for (auto& a : extensions)
    {
        std::string folder = path + a + FILE_POST;
        // only create folders that haven't been created
        if (!fs::exists(folder))
            fs::create_directory(folder);

        // now we know that that folder exists, move all relavent files into that folder
        for (auto& f : files)
        {
            // if correct type, sort!
            if (GetType(f) == a)
            {
                std::string from = f;
                std::string to = folder + "\\" + f.substr(path_length);
                fs::rename(from, to);   // by renaming we move the file to the correct spot
            }
        }
    }
    return 0;
}
