#include <windows.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

// Simple Batch to EXE Converter
int main(int argc, char* argv[]) {
    std::cout << "Batch to EXE Converter\n";
    
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " input.bat output.exe\n";
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    
    // Read batch file
    std::ifstream batchIn(inputFile, std::ios::binary);
    if (!batchIn) {
        std::cerr << "Error: Cannot open " << inputFile << std::endl;
        return 1;
    }
    
    std::stringstream buffer;
    buffer << batchIn.rdbuf();
    std::string batchContent = buffer.str();
    batchIn.close();
    
    // Create C++ source with embedded batch
    std::string sourceFile = "temp_embed.cpp";
    std::ofstream source(sourceFile);
    
    if (!source) {
        std::cerr << "Error creating temporary source file\n";
        return 1;
    }
    
    // Write the source code
    source << "#include <windows.h>\n";
    source << "#include <fstream>\n";
    source << "#include <string>\n\n";
    
    // Add batch content as string (properly escaped)
    source << "const std::string batchScript = \"";
    for (char c : batchContent) {
        if (c == '\\') source << "\\\\";
        else if (c == '\"') source << "\\\"";
        else if (c == '\n') source << "\\n\"\n\"";
        else if (c == '\r') {} // Skip carriage return
        else source << c;
    }
    source << "\";\n\n";
    
    source << R"(int main() {
    // Create temporary file path
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    
    std::string tempFile = std::string(tempPath) + "temp_batch_script.bat";
    
    // Write batch content to temp file
    std::ofstream batchFile(tempFile);
    if (batchFile.is_open()) {
        batchFile << batchScript;
        batchFile.close();
        
        // Execute batch file
        STARTUPINFOA si = {0};
        PROCESS_INFORMATION pi = {0};
        si.cb = sizeof(si);
        
        std::string cmd = "cmd.exe /c \"" + tempFile + "\"";
        
        if (CreateProcessA(NULL, (LPSTR)cmd.c_str(), 
                          NULL, NULL, FALSE, 0, 
                          NULL, NULL, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        
        // Delete temporary file
        DeleteFileA(tempFile.c_str());
    }
    
    return 0;
}
)";
    
    source.close();
    
    // Compile using g++ (MinGW)
    std::string compileCmd = "g++ -static -o \"" + outputFile + "\" \"" + sourceFile + "\"";
    std::cout << "Compiling...\n";
    system(compileCmd.c_str());
    
    // Clean up
    remove(sourceFile.c_str());
    
    std::cout << "Executable created: " << outputFile << std::endl;
    return 0;
}
