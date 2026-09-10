#include <array>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_set>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric>

std::array<std::string, 9> codeExtensions = {
    ".py", ".cpp", ".hpp",
    ".h", ".c", ".js",
    ".ts", ".go", ".cs",
};

std::unordered_set<std::string> ignoredDirs = {
    "build", "node_modules", "__pycache__"
};

size_t lineCount(std::string path) {
    std::ifstream file(path);

    if (!file.is_open()) { return 1; }

    return std::count(std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>(),
                      '\n'
    );
}

int main() {
    std::vector<std::pair<std::string, size_t>> results;
    auto root = std::filesystem::current_path();

    for (auto it = std::filesystem::recursive_directory_iterator(root);
         it != std::filesystem::recursive_directory_iterator();
         ++it) {

        const auto& fileObj = *it;

        if (fileObj.is_directory()) {
            std::string fileName = fileObj.path().filename().string();
            if (ignoredDirs.count(fileName) || fileName[0] == '.') {
                it.disable_recursion_pending();
            }
            continue;
        }

        if (!fileObj.is_regular_file()) {
            continue;
        } 

        std::string path = fileObj.path().string();

        for (const auto& extension : codeExtensions) {
            if (path.length() < extension.length()) {
                continue;
            }

            std::string pathExtension = path.substr(path.length() - extension.length());

            if (pathExtension == extension) {
                results.emplace_back(std::filesystem::relative(fileObj.path(), root).generic_string(), lineCount(path));
                break;
            }
        }
    }
    std::sort(results.begin(), results.end(),
            [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

    std::size_t fileWidth = std::string("Path").length();
    std::size_t lineWidth = std::string("Lines").length();
    for (const auto& [name, lines] : results) {
        fileWidth = std::max(fileWidth, name.length());
        lineWidth = std::max(lineWidth, std::to_string(lines).length());
    }

    std::cout << std::left << std::setw(static_cast<int>(fileWidth) + 2) << "Path"
              << std::right << std::setw(static_cast<int>(lineWidth)) << "Lines" << "\n";
    std::cout << std::string(fileWidth + 2 + lineWidth, '-') << "\n";

    for (const auto& [name, lines] : results) {
        std::cout << std::left << std::setw(static_cast<int>(fileWidth) + 2) << name
                  << std::right << std::setw(static_cast<int>(lineWidth)) << lines << "\n";
    }
    size_t totalLines = 0;

    for (const auto& pair : results) {
        totalLines += pair.second;
    }

    std::cout << "\nFiles: " << results.size() << ", Total lines: " << totalLines << "\n";

    return 0;
}