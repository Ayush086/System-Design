/* Builder Design Pattern                                                                                                                        Date: 15/07/2026 */
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
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

    // private constructure (only be accessed by builder)
    HTTPRequest() {}

public:
    friend class HTTPRequestBuilder;

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

// responsibility is to create an object of above class based on inputs provided
class HTTPRequestBuilder {
private:
    HTTPRequest req;

public:
    // Method Chaining - seems like setters but it prevents mutability
    HTTPRequestBuilder& withURL(const string& u) {
        req.url = u;
        return *this;
    }

    HTTPRequestBuilder& withMethod(const string& method) {
        req.method = method;
        return *this;
    }

    HTTPRequestBuilder& withHeader(const string& key, const string& value) {
        req.headers[key] = value;
        return *this;
    }

    HTTPRequestBuilder& withQueryParams(const string& key, const string& val) {
        req.queryParams[key] = val;
        return *this;
    }

    HTTPRequestBuilder& withBody(const string& body) {
        req.body = body;
        return *this;
    }

    HTTPRequestBuilder& withTimeout(int to) {
        req.timeout = to;
        return *this;
    }

    // to create immutable request object
    HTTPRequest build() {
        // if needed, validation can be added here
        if(req.url.empty()) {
            throw runtime_error("URL can't be empty");
        }

        return req;
    }
};


int main() {
    HTTPRequest request = HTTPRequestBuilder().withURL("https://builder.example.com") // intermediate methods
                                              .withMethod("PATCH")
                                              .withHeader("Content-Type", "application/json")
                                              .withHeader("Accept", "application/json")
                                              .withQueryParams("key", "1254")
                                              .withBody("{\"name\": \"Rohan\"}")
                                              .withTimeout(34)
                                              .build(); // terminatory method
    
    request.execute();
    return 0;
}