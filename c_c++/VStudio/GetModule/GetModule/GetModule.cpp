// GetModule.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define CURL_STATICLIB

#include <curl\curl.h>
//#include <openssl/ssl.h>
#include <iostream>

int main(int argc, char argv[])
{
    CURL* curl;

    char errorMsg[CURL_ERROR_SIZE];

    if (!(curl = curl_easy_init())) return 1;

    try{
        CURLcode code;
        if ((code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorMsg)) != CURLE_OK) throw (code);
        if ((code = curl_easy_setopt(curl, CURLOPT_URL, "https://ldap.serpro/")) != CURLE_OK) throw (code);
        //curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");

        if ((code = curl_easy_perform(curl)) != CURLE_OK) throw (code);
        curl_easy_cleanup(curl);
    }
    catch (CURLcode code)
    {
        std::cout << "Error " << code << "::"<< errorMsg << std::endl;
        curl_easy_cleanup(curl);
        return 2;
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file