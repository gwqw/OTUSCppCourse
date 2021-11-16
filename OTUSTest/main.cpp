#include <iostream>

#include <opencv2/opencv.hpp>

#include "framedata_reader.h"
#include "file_utils.h"

using namespace std;

int main(int argc, char* argv[]) {
    try {
        // parse command line
        const cv::String keys =
                "{help h         |      | this screen          }"
                "{dir d          |      | Input directory with *.info.yaml.gz files }"
                "{start s        | -1   | episode number to start counting fps. Empty means from the first in directory }"
                "{end e          | -1   | episode number to stop counting fps. Empty means to the last in directory }"
                "{ o             |      | output filename (csv-table)       }"
        ;

        cv::CommandLineParser parser(argc, argv, keys);
        if (parser.has("help") || !parser.has("dir")) {
            parser.printMessage();
            return 0;
        }
        auto inputDir = parser.get<string>("dir");
        auto startEpisodeNumber = max(parser.get<int>("start"), -1);
        auto endEpisodeNumber = max(parser.get<int>("end"), -1);
        auto ouputFname = parser.get<string>("o");

        auto file_list = getFileList(inputDir, startEpisodeNumber, endEpisodeNumber);
        Episode episode;
        for (const auto& filename : file_list) {
            episode.addDataFromFile(filename);
        }
        auto res = episode.getFps();
        saveToCSV(res, ouputFname);

        return 0;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
}
