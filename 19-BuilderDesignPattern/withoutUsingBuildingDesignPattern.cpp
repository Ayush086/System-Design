/* Without Building Design Pattern                                                                    Date: 14/07/2026 */
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

class HTTPRequest {
private:
    string url;
    string method;
    map<string, string> headers;
    map<string, string> queryParams;
    string body;
    int timeout;

public:
    // constructor with required params only
    HTTPRequest(const string& url) {
        this->url = url;
        // default values
        this->method = "GET";
        this->timeout = 30; 
    }

    // URL + method
    HTTPRequest(const string& url, string method) {
        this->url = url;
        this->method = method;
        this->timeout = 30;
    }

    // url + method + timeout
    HTTPRequest(const string& url, string method, int timeout) {
        this->url = url;
        this->method = method;
        this->timeout = timeout;
    }

    // url + method + timeout + headers
    HTTPRequest(const string& url, string method, int timeout, map<string, string> headers) {
        this->url = url;
        this->method = method;
        this->timeout = timeout;
        this->headers = headers;
    }

    // url + method + timeout + header + query params
    HTTPRequest(const string& url, string method, int timeout, map<string, string> headers, map<string, string> queryParams) {
        this->url = url;
        this->method = method;
        this->timeout = timeout;
        this->headers = headers;
        this->queryParams = queryParams;
    }

    // all
    HTTPRequest(const string& url, string method, int timeout, map<string, string> headers, map<string, string> queryParams, string body) {
        this->url = url;
        this->method = method;
        this->timeout = timeout;
        this->headers = headers;
        this->queryParams = queryParams;
        this->body = body;
    }

    // setters to avoid constructor overloading but it leads to mutability
    void setUrl(const string& url) {
        this->url = url;
    }

    void setMethod(string method) {
        this->method = method;
    }

    void addHeader(const string& key, const string& value) {
        this->headers[key] = value;
    }

    void addQueryParam(const string& key, const string& val) {
        this->queryParams[key] = val;
    }

    void setBody(const string& bdy) {
        this->body = bdy;
    }

    void setTimeout(int to) {
        this->timeout = to;
    }

    void execute() {
        cout << "Executing " << method << " request to " << url << endl;

        if(!queryParams.empty()) {
            cout << "Query Parameters: " << endl;
            for(const auto& param : queryParams) {
                cout << " " << param.first << "=" << param.second << endl;
            }
        }

        cout << "Headers: " << endl;
        for(const auto& header : headers) {
            cout << " " << header.first << ": " << header.second << endl;
        }

        if(!body.empty()) {
            cout << "Body: " << body << endl;
        }
        cout << "Timeout: " << timeout << " seconds" << endl;
        cout << "Request executed successfully!" << endl;
    }
};


int main() {
    HTTPRequest* req1 = new HTTPRequest("https://api.example.com");
    HTTPRequest* req2 = new HTTPRequest("https://api.example.com", "POST");
    HTTPRequest* req3 = new HTTPRequest("https://api.example.com", "PUT", 60);

    // using setters
    HTTPRequest* req4 = new HTTPRequest("https://api.setters.com");
    req4->setMethod("DELETE");
    req4->addHeader("Content-Type", "application/json");
    req4->addQueryParam("key", "124");
    req4->setBody("{\"name\": \"Rohan\"}");
    req4->setTimeout(100);

    req4->execute(); // what if we missed to set any import field before executing it ?
}