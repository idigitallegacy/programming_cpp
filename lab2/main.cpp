//
// Created by Михаил on 22.02.2022.
//
#include "linker/linker.h"

int main() {
    Request request;
    Request::RequestOptions options;
    options.URL = "https://www.cbr-xml-daily.ru/latest.js";
    options.protocols = CURLPROTO_HTTPS;
    request.configure(options);
    request.send();

    Statistics stat(request, 2500, std::vector<std::string>{"USD", "EUR"}, true);

    std::string cmd;

    while (true) {
        std::cin >> cmd;

        std::cout << "\t\t\t" << cmd << "\n";
        if (cmd == "stop") {
            break;
        } else if (cmd == "print") {
            stat.print();
        } else if (cmd == "listener_stop") {
            stat.stop();
        } else if (cmd == "listener_start") {
            stat.listen();
        }
    }

    return 0;
}