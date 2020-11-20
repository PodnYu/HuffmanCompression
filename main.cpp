#include "HuffmanCoding/HuffmanCompressor.hpp"


void printUsage();

int main(int argc, char *argv[]) {

    // Dummy command line arguments parsing.    
    if (argc >= 2 && std::string(argv[1]) == "-h") {
        printUsage();
        return 0;
    }

    if (argc < 3 || argc > 4) {
        if (argc < 3)
            std::cerr << "Not enough arguments, expected 2-3 but got " + std::to_string(argc - 1) << std::endl;
        if (argc > 4)
            std::cerr << "Too many arguments, expected 2-3 but got " + std::to_string(argc - 1) << std::endl;
        printUsage();
        return 1;
    }

    char comp = 0;
    char decomp = 0;

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (std::string(argv[i]) == "-c") {
                comp = i;
            } else if (std::string(argv[i]) == "-d") {
                decomp = i;
            } else {
                std::cerr << "Invalid key: " << argv[i] << "!" << std::endl;
                printUsage();
                return 1;
            }
            if ((comp != 0 && decomp != 0)) {
                std::cerr << "Too many keys!" << std::endl;
                printUsage();
                return 1;
            }
        }
    }
    if (comp == 0 && decomp == 0) {
        std::cerr << "No valid key provided. Should be 1!" << std::endl;
        printUsage();
        return 1;
    }

    char baseIndex = (comp == 0) ? (decomp + 1) : (comp + 1);

    if (baseIndex >= argc) {
        std::cerr << "No parameter for key " << argv[baseIndex - 1] << "!" << std::endl;
        printUsage();
        return 1;
    }

    std::string sourceFileName = argv[baseIndex];
    std::string destFileName = ((baseIndex + 1) >= argc) ? "" : argv[baseIndex + 1];



    if (destFileName == "") {
        int dotIndex = sourceFileName.find_first_of('.');
        if (dotIndex != -1) {
            destFileName = sourceFileName.substr(0, dotIndex) + ((comp == 0) ? "_decomp.huff" : "_comp.huff");
        } else {
            destFileName = sourceFileName + ((comp == 0) ? "_decomp.huff" : "_comp.huff");
        }
    }

    std::cout << "Source file: " << sourceFileName << std::endl;
    std::cout << "Destination file: " << destFileName << std::endl << std::endl;

    HuffmanCompressor compressor;
    int result = 0;

    if (comp != 0) {
        result = compressor.compress(sourceFileName, destFileName);
        if (result == 0)
            compressor.printCompressionInfo();
    } else if (decomp != 0) {
        result = compressor.decompress(sourceFileName, destFileName);
    }

    switch (result)
    {
        case HuffmanCompressor::ErrorCodes::SourceFileNotFound:
            std::cout << "Error: Source file not found: " << sourceFileName << "." << std::endl;
            break;
        case HuffmanCompressor::ErrorCodes::CannotCreateDestFile:
            std::cout << "Error: Cannot create destination file: " << destFileName << "." << std::endl;
            break;
        default:
            break;
    }

    return 0;
}

void printUsage() {

    std::cout << "Usage: HuffmanCompressor OPTION <source_file> [<dest_file>]" << std::endl;
    std::cout << "\t-h\tget usage instructions." << std::endl;
    std::cout << "\t-c\tcompress file. Usage: HuffmanCompressor -c <file_to_compress> [<output_file> (default: \"<file_to_compress>_comp.huff\")]." << std::endl;
    std::cout << "\t-d\tdecompress file. Usage: HuffmanCompressor -d <file_to_decompress> [<output_file> (default: \"<file_to_decompress>_decomp.huff\")]." << std::endl;

}