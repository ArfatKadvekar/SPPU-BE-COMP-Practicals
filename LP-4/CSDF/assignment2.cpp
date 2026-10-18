
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

string generateCaptcha(int n) {
    char chrs[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    string captcha = "";

    for (int i = 0; i < n; i++) {
        captcha += chrs[rand() % 62];
    }

    return captcha;
}

bool checkCaptcha(string captcha, string input) {
    return captcha == input;
}

int main() {
    srand(time(0));

    int n = 6;
    string captcha, input;

    captcha = generateCaptcha(n);

    cout << "Generated CAPTCHA: " << captcha << endl;
    cout << "Enter CAPTCHA: ";
    cin >> input;

    if (checkCaptcha(captcha, input)) {
        cout << "CAPTCHA verification successful!" << endl;
    } else {
        cout << "CAPTCHA verification failed!" << endl;
    }

    return 0;
}

